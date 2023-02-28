/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize_zbuffer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 07:34:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/21 14:57:11 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

inline static void sample_img(uint32_t x, uint32_t y, float z, t_lighting *lighting)
{
	int index;

	index = x + y * lighting->resolution.x;
	if (z > lighting->zbuffer[index])
	{
		lighting->zbuffer[index] = z;
	}
}

inline static void scanline(int ax, int bx, int y, t_vector2*p, t_texture *t, t_lighting *lighting)
{
	float		z;
	t_vector2	bary;
	t_vector2	left;
	t_vector2	right;
	float		steps;

	left = barycentric_coordinates(p, vector2_add_xy((t_vector2){ax, y}, 0.5f));
	right = barycentric_coordinates(p, vector2_add_xy((t_vector2){bx, y}, 0.5f));
	bary = left;
	int start = ax;
	steps = bx - start;
	while(ax < bx)
	{
		z = ft_flerp(t[0].w, t[1].w, bary.x) + ((t[2].w - t[0].w) * bary.y);
		sample_img(ax, y, z, lighting);
		ax++;
		bary = vector2_lerp(left, right, (float)(ax - start) / steps);
	}
}

static void fill_point_tri_top(t_lighting *lighting, t_point_triangle triangle)
{
	t_vector2			*p;
	t_texture		*t;
	int				y;
	float			delta;
	float			w1;
	float			w2;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	left.step = (p[0].x - p[1].x) / (p[0].y - p[1].y);
	right.step = (p[0].x - p[2].x) / (p[0].y - p[2].y);
	y = ceilf(p[2].y - 0.5f);
	endy = ceilf(p[0].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[1].y) + p[1].x;
		right.location = right.step * ((float)y + 0.5f - p[2].y) + p[2].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, p, t, lighting);
		y++;
	}
}

static void fill_point_tri_bot(t_lighting *lighting, t_point_triangle triangle)
{
	t_vector2			*p;
	t_texture		*t;
	int				y;
	float			delta;
	float			w1;
	float			w2;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	left.step = (p[1].x - p[0].x) / (p[1].y - p[0].y);
	right.step = (p[2].x - p[0].x) / (p[2].y - p[0].y);
	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[2].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		right.location = right.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, p, t, lighting);
		y++;
	}
}

void	rasterize_zbuffer(t_lighting *lighting, t_point_triangle triangle)
{
	t_vector2				p_split;
	t_texture			t_split;
	t_texture			t_temp;
	t_vector2				p_temp;
	t_vector2				*p;
	float				lerp;

	p = triangle.p;
	sort_point_uv_tri(triangle.p, triangle.t);
	if (p[1].y == p[2].y)
	{
		if (p[1].x > p[2].x)
		{
			ft_swap(&p[1], &p[2], sizeof(t_vector2));
			ft_swap(&triangle.t[1], &triangle.t[2], sizeof(t_texture));
		}
		fill_point_tri_top(lighting, triangle);
	}
	else if (p[0].y == p[1].y)
	{
		ft_swap(&p[0], &p[2], sizeof(t_vector2));
		ft_swap(&triangle.t[0], &triangle.t[2], sizeof(t_texture));
		if (p[1].x > p[2].x)
		{
			ft_swap(&p[1], &p[2], sizeof(t_vector2));
			ft_swap(&triangle.t[1], &triangle.t[2], sizeof(t_texture));
		}
		fill_point_tri_bot(lighting, triangle);
	}
	else
	{
		float alphasplit = (p[1].y - p[2].y) / (p[0].y - p[2].y);

		t_vector2 vi = vector2_mul(vector2_add(p[2], vector2_sub(p[0], p[2])), alphasplit);

		p_split.x = p[2].x + (alphasplit * (p[0].x - p[2].x));
		p_split.y = p[1].y;
		t_split.u = ft_flerp(triangle.t[2].u, triangle.t[0].u, alphasplit);
		t_split.v = ft_flerp(triangle.t[2].v, triangle.t[0].v, alphasplit);
		t_split.w = ft_flerp(triangle.t[2].w, triangle.t[0].w, alphasplit);
		if (p_split.x < p[1].x)
		{
			ft_swap(&p[1], &p_split, sizeof(t_vector2));
			ft_swap(&triangle.t[1], &t_split, sizeof(t_texture));
		}
		p_temp = p[2];
		t_temp = triangle.t[2];
		p[2] = p_split;
		triangle.t[2] = t_split;
		if (p[0].y != p[1].y && p[1].x != p[2].x)
			fill_point_tri_top(lighting, triangle);
		p[0] = p_temp;
		triangle.t[0] = t_temp;
		if (p[0].y != p[1].y && p[1].x != p[2].x)
			fill_point_tri_bot(lighting, triangle);
	}
}
