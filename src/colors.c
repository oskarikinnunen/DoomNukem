/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:59 by raho              #+#    #+#             */
/*   Updated: 2022/12/01 20:53:48 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "colors.h"

static t_argb	extract_color_components(uint32_t surface_pixel, SDL_PixelFormat *fmt)
{
	uint32_t	temp;
	uint32_t	pixel;
	t_argb		color;

	pixel = surface_pixel;
	
	/* Get Red component */
	temp = pixel & fmt->Rmask;  /* Isolate red component */
	temp = temp >> fmt->Rshift; /* Shift it down to 8-bit */
	temp = temp << fmt->Rloss;  /* Expand to a full 8-bit number */
	color.red = (Uint8)temp;

	/* Get Green component */
	temp = pixel & fmt->Gmask;  /* Isolate green component */
	temp = temp >> fmt->Gshift; /* Shift it down to 8-bit */
	temp = temp << fmt->Gloss;  /* Expand to a full 8-bit number */
	color.green = (Uint8)temp;

	/* Get Blue component */
	temp = pixel & fmt->Bmask;  /* Isolate blue component */
	temp = temp >> fmt->Bshift; /* Shift it down to 8-bit */
	temp = temp << fmt->Bloss;  /* Expand to a full 8-bit number */
	color.blue = (Uint8)temp;

	/* Get Alpha component */
	temp = pixel & fmt->Amask;  /* Isolate alpha component */
	temp = temp >> fmt->Ashift; /* Shift it down to 8-bit */
	temp = temp << fmt->Aloss;  /* Expand to a full 8-bit number */
	color.alpha = (Uint8)temp;

	return (color);
}

uint32_t	blend_colors_alpha(uint32_t bg, uint32_t fg, SDL_PixelFormat *fmt, uint8_t alpha)
{
	uint32_t	result;
/* 	t_argb		color_bg;
	t_argb		color_fg; */
	t_color		color_ubg;
	t_color		color_ufg;
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

	color_ubg.hexadecimal = bg;
	color_ufg.hexadecimal = fg;
	
	/* printf("color_ubg.hecadecimal:\na: %d\nr: %d\ng: %d\nb: %d\n", color_ubg.uargb.alpha, color_ubg.uargb.red, color_ubg.uargb.green, color_ubg.uargb.blue);
	printf("color_ufg.hecadecimal:\na: %d\nr: %d\ng: %d\nb: %d\n\n", color_ufg.uargb.alpha, color_ufg.uargb.red, color_ufg.uargb.green, color_ufg.uargb.blue); */

	color_ubg.uargb.alpha = (uint8_t)255;
	color_ufg.uargb.alpha = alpha;

	abg = ((float)color_ubg.uargb.alpha / 255.0f);
	afg = ((float)color_ufg.uargb.alpha / 255.0f);
	
	redbg = (float)color_ubg.uargb.red / 255.0f;
	greenbg = (float)color_ubg.uargb.green / 255.0f;
	bluebg = (float)color_ubg.uargb.blue / 255.0f;
	
	redfg = (float)color_ufg.uargb.red / 255.0f;
	greenfg = (float)color_ufg.uargb.green / 255.0f;
	bluefg = (float)color_ufg.uargb.blue / 255.0f;



	/* color_bg = extract_color_components(bg, fmt);
	color_fg = extract_color_components(fg, fmt); */

	/* printf("color_bg.hecadecimal:\na: %d\nr: %d\ng: %d\nb: %d\n", color_bg.alpha, color_bg.red, color_bg.green, color_bg.blue);
	printf("color_fg.hecadecimal:\na: %d\nr: %d\ng: %d\nb: %d\n\n", color_fg.alpha, color_fg.red, color_fg.green, color_fg.blue); */

	/* color_bg.alpha = (uint8_t)255;
	color_fg.alpha = alpha;

	abg = ((float)color_bg.alpha / 255.0f);
	afg = ((float)color_fg.alpha / 255.0f);
	
	redbg = (float)color_bg.red / 255.0f;
	greenbg = (float)color_bg.green / 255.0f;
	bluebg = (float)color_bg.blue / 255.0f;
	
	redfg = (float)color_fg.red / 255.0f;
	greenfg = (float)color_fg.green / 255.0f;
	bluefg = (float)color_fg.blue / 255.0f; */

	

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
