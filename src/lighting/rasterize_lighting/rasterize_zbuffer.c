/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize_zbuffer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 07:34:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/23 18:47:03 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

inline static void	sample_img(uint32_t x, uint32_t y, float z,
	t_lighting *lighting)
{
	int	index;

	index = x + y * lighting->resolution.x;
	if (z > lighting->zbuffer[index])
	{
		lighting->zbuffer[index] = z;
	}
}

inline static void	scanline(int ax, int bx, int y, t_lighting *l)
{
	t_vector2	bary;
	t_vector2	left;
	t_vector2	right;
	float		steps;
	float		index;

	left = barycentric_coordinates(
			l->triangle.p, vector2_add_xy((t_vector2){ax, y}, 0.5f));
	right = barycentric_coordinates(
			l->triangle.p, vector2_add_xy((t_vector2){bx, y}, 0.5f));
	bary = left;
	steps = bx - ax;
	index = 0.0f;
	while (ax < bx)
	{
		sample_img(ax, y,
			ft_flerp(l->triangle.t[0].z, l->triangle.t[1].z, bary.x) + \
		((l->triangle.t[2].z - l->triangle.t[0].z) * bary.y), l);
		index++;
		bary = vector2_lerp(left, right, index / steps);
		ax++;
	}
}

static void	fill_point_tri_top(t_lighting *l)
{
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	steps = l->triangle.p[0].y - l->triangle.p[1].y;
	left = make_slope(l->triangle.p[1].x, l->triangle.p[0].x, steps);
	right = make_slope(l->triangle.p[2].x, l->triangle.p[0].x, steps);
	y = ceilf(l->triangle.p[2].y - 0.5f);
	endy = ceilf(l->triangle.p[0].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * \
		((float)y + 0.5f - l->triangle.p[1].y) + l->triangle.p[1].x;
		right.location = right.step * \
		((float)y + 0.5f - l->triangle.p[2].y) + l->triangle.p[2].x;
		scanline(
			ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, l);
		y++;
	}
}

static void	fill_point_tri_bot(t_lighting *l)
{
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	steps = l->triangle.p[1].y - l->triangle.p[0].y;
	left = make_slope(l->triangle.p[0].x, l->triangle.p[1].x, steps);
	right = make_slope(l->triangle.p[0].x, l->triangle.p[2].x, steps);
	y = ceilf(l->triangle.p[0].y - 0.5f);
	endy = ceilf(l->triangle.p[2].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * \
		((float)y + 0.5f - l->triangle.p[0].y) + l->triangle.p[0].x;
		right.location = right.step * \
		((float)y + 0.5f - l->triangle.p[0].y) + l->triangle.p[0].x;
		scanline(
			ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, l);
		y++;
	}
}

void	rasterize_zbuffer(t_lighting *lighting)
{
	t_screen_triangle	tris[2];
	int					res;

	res = triangle_to_flat(lighting->triangle, tris);
	lighting->triangle = tris[0];
	if (res == 0)
		fill_point_tri_top(lighting);
	else if (res == 1)
		fill_point_tri_bot(lighting);
	else
	{
		fill_point_tri_top(lighting);
		lighting->triangle = tris[1];
		fill_point_tri_bot(lighting);
	}
}
