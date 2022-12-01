/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/12/01 18:44:53 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

typedef struct s_rgba
{
	uint8_t alpha;
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
}	t_rgba;

/* typedef union u_color
{
	uint32_t	hexadecimal;
	
	struct s_urgba
	{
		uint8_t alpha;
		uint8_t	red;
		uint8_t	green;
		uint8_t	blue;
	};
	
}	t_color; */

t_rgba	extract_color_components(uint32_t surface_pixel, SDL_PixelFormat *fmt)
{
	uint32_t	temp;
	uint32_t	pixel;
	t_rgba		color;
	
	// Do this with union and simply access every color

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
	t_rgba		color_bg;
	t_rgba		color_fg;
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
	
	color_bg = extract_color_components(bg, fmt);
	color_fg = extract_color_components(fg, fmt);
	
	color_bg.alpha = 255;
	color_fg.alpha = alpha;

	abg = ((float)color_bg.alpha / 255.0f);
	afg = ((float)color_fg.alpha / 255.0f);
	
	redbg = (float)color_bg.red / 255.0f;
	greenbg = (float)color_bg.green / 255.0f;
	bluebg = (float)color_bg.blue / 255.0f;
	
	redfg = (float)color_fg.red / 255.0f;
	greenfg = (float)color_fg.green / 255.0f;
	bluefg = (float)color_fg.blue / 255.0f;

	// OpenGL - transparency (alpha blending) Youtube
	new_alpha = afg + abg * (1.0f - afg);
	red = ((redfg * afg) + (redbg * abg * (1.0f - afg))) / new_alpha;
	green = ((greenfg * afg) + (greenbg * abg * (1.0f - afg))) / new_alpha;
	blue = ((bluefg * afg) + (bluebg * abg * (1.0f - afg))) / new_alpha;

	new_alpha *= 255.0f;
	red *= 255.0f;
	green *= 255.0f;
	blue *= 255.0f;

	result = ((int)new_alpha << 24) | ((int)red << 16) | ((int)green << 8) | (int)blue;

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
					{
						((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = \
								blend_colors_alpha(((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)], \
								((uint32_t *)new->pixels)[i.x + (i.y * new->w)], sdl->surface->format, sdl->font.color.a);
					}
					else
					{
						((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = \
								blend_colors_alpha(((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)], \
								sdl->font.box_color, sdl->surface->format, sdl->font.color.a);
					}
				}
				else
				{
					((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = \
							blend_colors_alpha(((uint32_t *)sdl->surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)], \
							sdl->font.box_color, sdl->surface->format, sdl->font.color.a);
				}
			}
			i.x++;
		}
		i.y++;
	}
}

static void	join_surfaces(SDL_Surface *base, SDL_Surface *new, t_point pos, uint8_t alpha)
{
	t_point	i;

	i.y = 0;
	while (i.y < new->h)
	{
		i.x = 0;
		while (i.x < new->w)
		{
			if (i.x + pos.x > 0 && i.x + pos.x < base->w && \
				i.y + pos.y > 0 && i.y + pos.y < base->h)
			{
				if ((((uint32_t *)new->pixels)[i.x + (i.y * new->w)] >> 24) > 0) // checking alpha
				{
					((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)] = \
							blend_colors_alpha(((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)], \
							((uint32_t *)new->pixels)[i.x + (i.y * new->w)], base->format, alpha);
				}
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
	join_surfaces(sdl->surface, surfacetext, pos, sdl->font.color.a);
	SDL_FreeSurface(surfacetext);
	return (rect);
}
