/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/11/30 22:08:06 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

typedef struct s_rgba
{
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
	uint8_t alpha;
}	t_rgba;

unsigned int	blend_alpha(unsigned int src, unsigned int dest, uint8_t alpha)
{
	int	aalpha;

	aalpha = 256 - alpha;
	return (
		((aalpha * (src >> 16 & 0xFF) + alpha * (dest >> 16 & 0xFF)) / 256)
		<< 16
		| ((aalpha * (src >> 8 & 0xFF) + alpha * (dest >> 8 & 0xFF)) / 256)
		<< 8
		| ((aalpha * (src & 0xFF) + alpha * (dest & 0xFF)) / 256));
}

t_rgba	extract_color_components(uint32_t surface_pixel, SDL_PixelFormat *fmt)
{
	uint32_t	temp;
	uint32_t	pixel;
	t_rgba		color;
	
	pixel = surface_pixel;
	
	//printf("pixel: %0x\n", pixel);
	
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


uint32_t	blend_colors(uint32_t bg, uint32_t fg, SDL_PixelFormat *fmt)
{
	uint32_t	result; 
	t_rgba		color_bg;
	t_rgba		color_fg;
	int			alpha;
	float		abg;
	float		afg;
	int			red;
	int			green;
	int			blue;
	float		redbg;
	float		greenbg;
	float		bluebg;
	float		redfg;
	float		greenfg;
	float		bluefg;
	
	color_bg = extract_color_components(bg, fmt);
	color_fg = extract_color_components(fg, fmt);
	
	if (color_bg.alpha == 0)
		color_bg.alpha = 255;

	abg = ((float)color_bg.alpha / 255.0f);
	afg = ((float)color_fg.alpha / 255.0f);
	
	redbg = (float)color_bg.red / 255.0f;
	greenbg = (float)color_bg.green / 255.0f;
	bluebg = (float)color_bg.blue / 255.0f;
	
	redfg = (float)color_fg.red / 255.0f;
	greenfg = (float)color_fg.green / 255.0f;
	bluefg = (float)color_fg.blue / 255.0f;

	/* printf("color_bg.alpha: %hhu\n", color_bg.alpha);
	printf("color_bg.red: %hhu\n", color_bg.red);
	printf("color_bg.green: %hhu\n", color_bg.green);
	printf("color_bg.blue: %hhu\n\n", color_bg.blue);

	printf("abg: %f\n", abg);
	printf("redbg: %f\n", redbg);
	printf("greenbg: %f\n", greenbg);
	printf("bluebg: %f\n\n", bluebg);

	printf("color_fg.alpha: %hhu\n", color_fg.alpha);
	printf("color_fg.red: %hhu\n", color_fg.red);
	printf("color_fg.green: %hhu\n", color_fg.green);
	printf("color_fg.blue: %hhu\n\n", color_fg.blue);

	printf("afg: %f\n", afg);
	printf("redfg: %f\n", redfg);
	printf("greenfg: %f\n", greenfg);
	printf("bluefg: %f\n\n", bluefg); */

/* 	// 1st
	alpha = 1.0f - (1.0f - abg) * (1.0f - afg);
	red = (redfg / 1.0f) * afg / alpha + (redbg / 1.0f) * abg * (1.0f - afg) / alpha;
	green = (greenfg / 1.0f) * afg / alpha + (greenbg / 1.0f) * abg * (1.0f - afg) / alpha;
	blue = (bluefg / 1.0f) * afg / alpha + (bluebg / 1.0f) * abg * (1.0f - afg) / alpha;
 */

	// OpenGL - transparency (alpha blending) Youtube
	alpha = afg + abg * (1.0f - afg);
	red = redfg * afg + redbg * abg * (1.0f - afg) / alpha;
	green = greenfg * afg + greenbg * abg * (1.0f - afg) / alpha;
	blue = bluefg * afg + bluebg * abg * (1.0f - afg) / alpha;

	alpha *= 255;
	red *= 255;
	green *= 255;
	blue *= 255;

	/* printf("alpha: %d\n", alpha);
	printf("red: %d\n", red);
	printf("green: %d\n", green);
	printf("blue: %d\n\n", blue); */

	result = (alpha << 24) | (red << 16) | (green << 8) | blue;
	
	/* printf("result: %0x\n\n", result); */

	return (result);
}

static void	join_surfaces_boxed(t_sdlcontext *sdl, SDL_Surface *new, t_point pos, t_point padding)
{
	t_point		i;

	i.y = 0 - padding.y;
	while (i.y < new->h + padding.y)
	{
		i.x = 0 - padding.x;
		while (i.x < new->w + padding.x)
		{
			if (i.x + pos.x > 0 && i.x + pos.x < sdl->surface->w && \
				i.y + pos.y > 0 && i.y + pos.y < sdl->surface->h)
			{
				if (i.x >= 0 && i.x < new->w && i.y >= 0 && i.y < new->h)
				{	
					if (((uint32_t *)new->pixels)[i.x + (i.y * new->w)] >> 24 > 0) // checking alpha
						((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = ((uint32_t *)new->pixels)[i.x + (i.y * new->w)];
					else
						((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = \
								blend_colors(((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)], \
								sdl->font.box_color, sdl->surface->format);
				}
				else
					((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = \
								blend_colors(((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)], \
								sdl->font.box_color, sdl->surface->format);
			}
			i.x++;
		}
		i.y++;
	}
}

static void	join_surfaces(SDL_Surface *base, SDL_Surface *new, t_point pos)
{
	t_point	i;
	SDL_PixelFormat *fmtbase, *fmtnew;
	uint32_t	temp, pixelbase, pixelnew, pixel;
	uint8_t		alphabase, alphanew;

	fmtbase = base->format;
	fmtnew = new->format;

	i.y = 0;
	while (i.y < new->h)
	{
		i.x = 0;
		while (i.x < new->w)
		{
			if (i.x + pos.x > 0 && i.x + pos.x < base->w && \
				i.y + pos.y > 0 && i.y + pos.y < base->h)
			{
				/* pixelbase = ((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)];
				pixelnew = ((uint32_t *)new->pixels)[i.x + (i.y * new->w)];
				
				temp = pixelbase & fmtbase->Amask;
				temp = temp >> fmtbase->Ashift;
				temp = temp << fmtbase->Aloss;
				alphabase = (uint8_t)temp;
				
				temp = pixelnew & fmtnew->Amask;
				temp = temp >> fmtnew->Ashift;
				temp = temp << fmtnew->Aloss;
				alphanew = (uint8_t)temp;

				printf("Base surface alpha: %d			TTF surface alpha: %d\n", alphabase, alphanew); */

				/* if ((((uint32_t *)new->pixels)[i.x + (i.y * new->w)] >> 24) != 0) // checking alpha
					((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)] = ((uint32_t *)new->pixels)[i.x + (i.y * new->w)]; */

				/* if ((((uint32_t *)new->pixels)[i.x + (i.y * new->w)] >> 24) > 0) // checking alpha
					((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)] = \
					blend_colors(((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)], \
								((uint32_t *)new->pixels)[i.x + (i.y * new->w)], base->format); */

				pixel = ((uint32_t *)new->pixels)[i.x + (i.y * new->w)];
				((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)] = \
					blend_alpha(((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)], pixel, \
								(uint8_t)(((pixel & base->format->Amask) >> base->format->Ashift) << base->format->Aloss));
				
				/* pixelbase = ((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)];
				
				temp = pixelbase & fmtbase->Amask;
				temp = temp >> fmtbase->Ashift;
				temp = temp << fmtbase->Aloss;
				alphabase = (uint8_t)temp;
				
				printf("Base surface alpha: %d\n", alphabase); */
			}
			i.x++;
		}
		i.y++;
	}
}

t_rectangle	print_text_boxed(t_sdlcontext *sdl, const char *text, t_point pos)
{
	SDL_Surface	*surfacetext;
	t_rectangle	rect;
	t_point		padding;

	surfacetext = TTF_RenderText_Blended(sdl->font.font, text, sdl->font.color);
	if (!surfacetext)
		error_log(EC_TTF_RENDERTEXTBLENDED);
	padding.x = 3;
	padding.y = 3;
	rect.size.x = surfacetext->w + padding.x * 2;
	rect.size.y = surfacetext->h + padding.y * 2;
	rect.position.x = pos.x - padding.x;
	rect.position.y = pos.y - padding.y;
	join_surfaces_boxed(sdl, surfacetext, pos, padding);
	SDL_FreeSurface(surfacetext);
	return (rect);
}

t_rectangle	print_text(t_sdlcontext *sdl, const char *text, t_point pos)
{
	SDL_Surface	*surfacetext;
	t_rectangle	rect;

	surfacetext = TTF_RenderText_Blended(sdl->font.font, text, sdl->font.color);
	if (!surfacetext)
		error_log(EC_TTF_RENDERTEXTBLENDED);
	rect.position = pos;
	rect.size.x = surfacetext->w;
	rect.size.y = surfacetext->h;
	join_surfaces(sdl->surface, surfacetext, pos);
	SDL_FreeSurface(surfacetext);
	return (rect);
}
