/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:59 by raho              #+#    #+#             */
/*   Updated: 2023/03/07 12:55:17 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "colors.h"

SDL_Color	color32_to_sdlcolor(uint32_t color)
{
	SDL_Color	sdl_color;
	t_rgb		temp;

	temp.blue = color & 0xFF;
	temp.green = (color >> 8) & 0xFF;
	temp.red = (color >> 16) & 0xFF;
	sdl_color = (SDL_Color){\
		.r = temp.red, .g = temp.green, .b = temp.blue, .a = 255};
	return (sdl_color);
}

uint32_t	flip_alpha(uint32_t clr)
{
	t_color		result;

	result.color = clr;
	result.bgra.alpha = 255 - result.bgra.alpha;
	return (result.color);
}

t_argbf	color_to_argbf(t_color color)
{
	t_argbf	new;

	new.a = ((float)color.bgra.alpha / 255.0f);
	new.r = (float)color.bgra.red / 255.0f;
	new.g = (float)color.bgra.green / 255.0f;
	new.b = (float)color.bgra.blue / 255.0f;
	return (new);
}

uint32_t	blend_colors_alpha(uint32_t background,
								uint32_t foreground, uint8_t alpha)
{
	t_argbf		new;
	t_argbf		bg;
	t_argbf		fg;
	t_color		color_bg;
	t_color		color_fg;

	color_bg.color = background;
	color_fg.color = foreground;
	color_bg.bgra.alpha = (uint8_t)255;
	color_fg.bgra.alpha = alpha;
	bg = color_to_argbf(color_bg);
	fg = color_to_argbf(color_fg);
	new.a = fg.a + bg.a * (1.0f - fg.a);
	new.r = ((fg.r * fg.a) + (bg.r * bg.a * (1.0f - fg.a))) / new.a;
	new.g = ((fg.g * fg.a) + (bg.g * bg.a * (1.0f - fg.a))) / new.a;
	new.b = ((fg.b * fg.a) + (bg.b * bg.a * (1.0f - fg.a))) / new.a;
	new.a *= 255.0f;
	new.r *= 255.0f;
	new.g *= 255.0f;
	new.b *= 255.0f;
	return (((uint32_t)new.a << 24) | \
			((uint32_t)new.r << 16) | \
			((uint32_t)new.g << 8) | \
			(uint32_t)new.b);
}
