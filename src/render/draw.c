/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfum <kfum@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 05:48:12 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 09:31:56 by kfum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"
#include "shapes.h"
#include "colors.h"

void	draw(t_sdlcontext sdl, t_point pos, uint32_t clr)
{
	if (pos.x < 0 || pos.x >= sdl.surface->w - 2
		|| pos.y < 0 || pos.y >= sdl.surface->h - 2)
		return ;
	((uint32_t *)sdl.surface->pixels)[pos.x + (pos.y * sdl.surface->w)] = clr;
}

void	draw_alpha(t_sdlcontext sdl, t_point pos, uint32_t clr)
{
	if (pos.x < 0 || pos.x >= sdl.window_w - 2
		|| pos.y < 0 || pos.y >= sdl.window_h - 2)
		return ;
	((uint32_t *)sdl.surface->pixels)[pos.x + (pos.y * sdl.window_w)] = \
			blend_colors_alpha(((uint32_t *)sdl.surface->pixels) \
					[pos.x + (pos.y * sdl.window_w)], clr, (clr >> 24));
}

static inline void	draw_image_alpha(t_sdlcontext sdl,
									t_point pos, t_img img, t_point scale)
{
	t_point		pixel;
	t_point		sample;
	t_vector2	scalar;
	uint32_t	color;

	scalar.x = ((float)img.size.x / scale.x);
	scalar.y = ((float)img.size.y / scale.y);
	pixel.y = 0;
	while (pixel.y < scale.y)
	{
		pixel.x = 0;
		while (pixel.x < scale.x)
		{
			sample.x = ft_clamp(pixel.x * scalar.x, 0, img.size.x - 1);
			sample.y = ft_clamp(pixel.y * scalar.y, 0, img.size.y - 1);
			color = img.data[sample.x + (sample.y * img.size.x)];
			draw_alpha(sdl, point_add(pos, pixel), color);
			pixel.x++;
		}
		pixel.y++;
	}
}

static inline void	draw_image_opaque(t_sdlcontext sdl,
									t_point pos, t_img img, t_point scale)
{
	t_point		pixel;
	t_point		sample;
	t_vector2	scalar;
	uint32_t	color;

	scalar.x = ((float)img.size.x / scale.x);
	scalar.y = ((float)img.size.y / scale.y);
	pixel.y = 0;
	while (pixel.y < scale.y)
	{
		pixel.x = 0;
		while (pixel.x < scale.x)
		{
			sample.x = ft_clamp(pixel.x * scalar.x, 0, img.size.x - 1);
			sample.y = ft_clamp(pixel.y * scalar.y, 0, img.size.y - 1);
			color = img.data[sample.x + (sample.y * img.size.x)];
			draw(sdl, point_add(pos, pixel), color);
			pixel.x++;
		}
		pixel.y++;
	}
}

void	draw_image(t_sdlcontext sdl, t_point pos, t_img img, t_point scale)
{
	if (img.size.x == 0 || img.size.y == 0 || img.length == 0)
		return ;
	if (img.transparency)
		draw_image_alpha(sdl, pos, img, scale);
	else
		draw_image_opaque(sdl, pos, img, scale);
}
