/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize_zbuffer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 07:34:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/07 07:42:33 by vlaine           ###   ########.fr       */
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

inline static void scanline(int ax, int bx, int y, t_vector2*p, t_vector3 *t, t_lighting *lighting)
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
		z = ft_flerp(t[0].z, t[1].z, bary.x) + ((t[2].z - t[0].z) * bary.y);
		sample_img(ax, y, z, lighting);
		ax++;
		bary = vector2_lerp(left, right, (float)(ax - start) / steps);
	}
}

static void fill_point_tri_top(t_lighting *lighting, t_point_triangle triangle)
{
	t_vector2		*p;
	t_vector3		*t;
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	steps = p[0].y - p[1].y;
	left = make_slope(p[1].x, p[0].x, steps);
	right = make_slope(p[2].x, p[0].x, steps);
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
	t_vector2		*p;
	t_vector3		*t;
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	steps = p[1].y - p[0].y;
	left = make_slope(p[0].x, p[1].x, steps);
	right = make_slope(p[0].x, p[2].x, steps);
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
	t_point_triangle	tris[2];
	int	res;

	res = triangle_to_flat(triangle, tris);
	if (res == 0)
		fill_point_tri_top(lighting, tris[0]);
	else if (res == 1)
		fill_point_tri_bot(lighting, tris[0]);
	else
	{
		fill_point_tri_top(lighting, tris[0]);
		fill_point_tri_bot(lighting, tris[1]);
	}
}
