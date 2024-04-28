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
	t_argbf	newColor;

	newColor.a = ((float)color.bgra.alpha / 255.0f);
	newColor.r = (float)color.bgra.red / 255.0f;
	newColor.g = (float)color.bgra.green / 255.0f;
	newColor.b = (float)color.bgra.blue / 255.0f;
	return (newColor);
}

uint32_t	blend_colors_alpha(uint32_t background,
								uint32_t foreground, uint8_t alpha)
{
	t_argbf		newColor;
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
	newColor.a = fg.a + bg.a * (1.0f - fg.a);
	newColor.r = ((fg.r * fg.a) + (bg.r * bg.a * (1.0f - fg.a))) / newColor.a;
	newColor.g = ((fg.g * fg.a) + (bg.g * bg.a * (1.0f - fg.a))) / newColor.a;
	newColor.b = ((fg.b * fg.a) + (bg.b * bg.a * (1.0f - fg.a))) / newColor.a;
	newColor.a *= 255.0f;
	newColor.r *= 255.0f;
	newColor.g *= 255.0f;
	newColor.b *= 255.0f;
	return (((uint32_t)newColor.a << 24) | \
			((uint32_t)newColor.r << 16) | \
			((uint32_t)newColor.g << 8) | \
			(uint32_t)newColor.b);
}
