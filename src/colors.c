/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:59 by raho              #+#    #+#             */
/*   Updated: 2023/03/03 19:04:36 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "colors.h"

uint32_t	flip_alpha(uint32_t clr)
{
	t_color		result;

	result.color = clr;
	result.argb.alpha = 255 - result.argb.alpha;
	return (result.color);
}

uint32_t	blend_colors_alpha(uint32_t bg, uint32_t fg, uint8_t alpha)
{
	uint32_t	result;
	
	float		new_alpha;
	float		new_red;
	float		new_green;
	float		new_blue;

	t_color		color_bg;
	t_color		color_fg;
	
	float		alpha_bg;
	float		red_bg;
	float		green_bg;
	float		blue_bg;
	
	float		alpha_fg;
	float		red_fg;
	float		green_fg;
	float		blue_fg;


	color_bg.color = bg;
	color_fg.color = fg;

	color_bg.argb.alpha = (uint8_t)255;
	color_fg.argb.alpha = alpha;

	alpha_bg = ((float)color_bg.argb.alpha / 255.0f);
	alpha_fg = ((float)color_fg.argb.alpha / 255.0f);
	
	red_bg = (float)color_bg.argb.red / 255.0f;
	green_bg = (float)color_bg.argb.green / 255.0f;
	blue_bg = (float)color_bg.argb.blue / 255.0f;
	
	red_fg = (float)color_fg.argb.red / 255.0f;
	green_fg = (float)color_fg.argb.green / 255.0f;
	blue_fg = (float)color_fg.argb.blue / 255.0f;

	// OpenGL - transparency (alpha blending) Youtube
	new_alpha = alpha_fg + alpha_bg * (1.0f - alpha_fg);
	new_red = ((red_fg * alpha_fg) + (red_bg * alpha_bg * (1.0f - alpha_fg))) / new_alpha;
	new_green = ((green_fg * alpha_fg) + (green_bg * alpha_bg * (1.0f - alpha_fg))) / new_alpha;
	new_blue = ((blue_fg * alpha_fg) + (blue_bg * alpha_bg * (1.0f - alpha_fg))) / new_alpha;

	new_alpha *= 255.0f;
	new_red *= 255.0f;
	new_green *= 255.0f;
	new_blue *= 255.0f;

	result = ((uint32_t)new_alpha << 24) | ((uint32_t)new_red << 16) | ((uint32_t)new_green << 8) | (uint32_t)new_blue;

	return (result);
}
