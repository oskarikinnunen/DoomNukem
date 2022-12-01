/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:59 by raho              #+#    #+#             */
/*   Updated: 2022/12/01 20:55:47 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "colors.h"

uint32_t	blend_colors_alpha(uint32_t bg, uint32_t fg, SDL_PixelFormat *fmt, uint8_t alpha)
{
	uint32_t	result;
	t_color		color_bg;
	t_color		color_fg;
	float		new_alpha;
	float		abg;
	float		afg;
	float		red;
	float		green;
	float		blue;
	float		redbg;
	float		greenbg;
	float		bluebg;
	float		redfg;
	float		greenfg;
	float		bluefg;

	color_bg.hexadecimal = bg;
	color_fg.hexadecimal = fg;

	color_bg.uargb.alpha = (uint8_t)255;
	color_fg.uargb.alpha = alpha;

	abg = ((float)color_bg.uargb.alpha / 255.0f);
	afg = ((float)color_fg.uargb.alpha / 255.0f);
	
	redbg = (float)color_bg.uargb.red / 255.0f;
	greenbg = (float)color_bg.uargb.green / 255.0f;
	bluebg = (float)color_bg.uargb.blue / 255.0f;
	
	redfg = (float)color_fg.uargb.red / 255.0f;
	greenfg = (float)color_fg.uargb.green / 255.0f;
	bluefg = (float)color_fg.uargb.blue / 255.0f;

	// OpenGL - transparency (alpha blending) Youtube
	new_alpha = afg + abg * (1.0f - afg);
	red = ((redfg * afg) + (redbg * abg * (1.0f - afg))) / new_alpha;
	green = ((greenfg * afg) + (greenbg * abg * (1.0f - afg))) / new_alpha;
	blue = ((bluefg * afg) + (bluebg * abg * (1.0f - afg))) / new_alpha;

	new_alpha *= 255.0f;
	red *= 255.0f;
	green *= 255.0f;
	blue *= 255.0f;

	result = ((uint32_t)new_alpha << 24) | ((uint32_t)red << 16) | ((uint32_t)green << 8) | (uint32_t)blue;

	return (result);
}
