/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/11/29 15:53:10 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

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
					if (((uint32_t *)new->pixels)[i.x + (i.y * new->w)] >> 24 != 0) // checking alpha
						((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = ((uint32_t *)new->pixels)[i.x + (i.y * new->w)];
					else
						((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = sdl->font.box_color;
				}
				else
					((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = sdl->font.box_color;
			}
			i.x++;
		}
		i.y++;
	}
}

typedef struct s_rgba
{
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
	uint8_t alpha;
}	t_rgba;

t_rgba	extract_color_components(uint32_t surface_pixel, SDL_PixelFormat *fmt)
{
	uint32_t	temp;
	uint32_t	pixel;
	t_rgba		color;
	
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

uint32_t	blend_colors(uint32_t bg, uint32_t fg, SDL_PixelFormat *fmt)
{
	uint32_t	result; 
	t_rgba		color_bg;
	t_rgba		color_fg;
	float		alpha;
	float		abg;
	float		afg;
	float		red;
	float		green;
	float		blue;
	
	color_bg = extract_color_components(bg, fmt);
	color_fg = extract_color_components(fg, fmt);
	
	abg = ((float)color_bg.alpha / 255.0f);
	afg = ((float)color_fg.alpha / 255.0f);
	alpha = 1.0f - (1.0f - abg) * (1.0f - afg);
	
	printf("color_bg.alpha: %hhu\n", color_bg.alpha);
	printf("abg: %f\n", abg);
	printf("color_bg.red: %hhu\n", color_bg.red);
	printf("color_bg.blue: %hhu\n", color_bg.blue);
	printf("color_bg.green: %hhu\n\n", color_bg.green);

	printf("color_fg.alpha: %hhu\n", color_fg.alpha);
	printf("afg: %f\n", afg);
	printf("color_fg.red: %hhu\n", color_fg.red);
	printf("color_fg.blue: %hhu\n", color_fg.blue);
	printf("color_fg.green: %hhu\n\n", color_fg.green);

	printf("alpha: %f\n", alpha);

	red = ((float)color_fg.red / 255.0f) * afg / alpha + ((float)color_bg.red / 255.0f) * abg * (1.0f - afg) / alpha;
	green = ((float)color_fg.green / 255.0f) * afg / alpha + ((float)color_bg.green / 255.0f) * abg * (1.0f - afg) / alpha;
	blue = ((float)color_fg.blue / 255.0f) * afg / alpha + ((float)color_bg.blue / 255.0f) * abg * (1.0f - afg) / alpha;

	printf("red: %f\n", red);
	printf("green: %f\n", green);
	printf("blue: %f\n\n", blue);

	result = result | ((int)alpha << 24);
	result = result | ((int)red << 16);
	result = result | ((int)green << 8);
	result = result | ((int)blue);
	
	printf("result: %0x\n\n", result);

	return (result);
}

static void	join_surfaces(SDL_Surface *base, SDL_Surface *new, t_point pos)
{
	t_point	i;
	SDL_PixelFormat *fmtbase, *fmtnew;
	uint32_t	temp, pixelbase, pixelnew;
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

				if (((uint32_t *)new->pixels)[i.x + (i.y * new->w)] >> 24 != 0) // checking alpha
					((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)] = \
					blend_colors(((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)], \
								((uint32_t *)new->pixels)[i.x + (i.y * new->w)], base->format);
				
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
