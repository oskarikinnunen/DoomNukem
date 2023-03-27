/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize_texture.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 17:14:18 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 20:23:27 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	sample_pixel(t_point xy, t_lighting *lighting)
{
	uint32_t	light;
	uint32_t	clr;
	t_img		*img;

	xy.x = ft_clamp(xy.x, 0, lighting->map->size.x - 1);
	xy.y = ft_clamp(xy.y, 0, lighting->map->size.y - 1);
	if (lighting->overdraw[xy.y * lighting->map->size.x + xy.x])
		return ;
	else
		lighting->overdraw[xy.y * lighting->map->size.x + xy.x] = true;
	img = lighting->img;
	light = lighting->map->lightmap[xy.y * lighting->map->size.x + xy.x];
	clr = img->data[(xy.y % img->size.y) * img->size.x + (xy.x % img->size.x)];
	clr = update_pixel_brightness(light, clr);
	lighting->map->texture[xy.y * lighting->map->size.x + xy.x] = clr;
}

static void	scanline(t_screen_triangle triangle, t_lighting *lighting,
	t_v2_rectangle bounds, int y)
{
	int	x;

	x = bounds.min.x - 0.5f;
	while (x < bounds.max.x)
	{
		sample_pixel((t_point){x, y}, lighting);
		x++;
	}
}

void	rasterize_texture(t_screen_triangle triangle, t_lighting *lighting)
{
	t_v2_rectangle	bounds;
	t_vector2		offset;
	int				y;

	bounds = get_bounds(&triangle);
	y = bounds.min.y - 0.5f;
	while (y < bounds.max.y)
	{
		scanline(triangle, lighting, bounds, y);
		y++;
	}
}
