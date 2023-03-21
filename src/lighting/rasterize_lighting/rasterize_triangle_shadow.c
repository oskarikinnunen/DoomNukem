/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize_triangle_shadow.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 15:59:23 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/21 18:27:29 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	sample_img(t_lighting *lighting, int x, int y,
	t_point_triangle poly)
{
	t_vector3	loc;
	uint32_t	light_amount;

	x = ft_clamp(x, 0, lighting->map->size.x - 1);
	y = ft_clamp(y, 0, lighting->map->size.y - 1);
	if (lighting->overdraw[y * lighting->map->size.x + x])
		return ;
	else
		lighting->overdraw[y * lighting->map->size.x + x] = true;
	loc = texcoord_to_loc(poly.t, poly.p, (t_vector2){x, y});
	light_amount = lighting->map->lightmap[y * (lighting->map->size.x) + x];
	light_amount = get_lighting_for_pixel(lighting, light_amount, loc);
	lighting->map->lightmap[y * (lighting->map->size.x) + x] = light_amount;
}

static void	scanline(t_point_triangle triangle, t_lighting *lighting,
	t_v2rectangle bounds, int y)
{
	bool		row;
	int			x;
	t_vector2	offset;
	t_vector2	bary;

	offset = get_barycentric_offset(triangle.p);
	row = false;
	x = bounds.min.x;
	while (x < bounds.max.x)
	{
		bary = barycentric_coordinates(triangle.p, (t_vector2){x, y});
		if (bary.x >= offset.x && bary.y >= offset.y
			&& bary.x + bary.y <= 1.0f -(offset.x + offset.y))
		{
			if (row == false)
				sample_img(lighting, x - 1, y, triangle);
			row = true;
			sample_img(lighting, x, y, triangle);
		}
		else if (row == true)
		{
			sample_img(lighting, x, y, triangle);
			row = false;
		}
		x++;
	}
}

void	rasterize_light(t_point_triangle triangle, t_lighting *lighting)
{
	t_v2rectangle	bounds;
	t_vector2		offset;
	int				y;

	bounds = get_bounds(&triangle);
	lighting->triangle_normal = normal_calc(triangle.t);
	y = bounds.min.y;
	while (y < bounds.max.y)
	{
		scanline(triangle, lighting, bounds, y);
		y++;
	}
}
