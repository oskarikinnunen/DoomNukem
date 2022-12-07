/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/12/07 06:47:29 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"

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
	join_text_boxed_to_surface(sdl, surfacetext, pos, padding);
	SDL_FreeSurface(surfacetext);
	return (rect);
}

SDL_Color color32_to_sdlcolor(uint32_t color)
{
	SDL_Color	sdl_color;
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;

	b = color & 0xFF;
	g = (color >> 8) & 0xFF;
	r = (color >> 16) & 0xFF;
	sdl_color = (SDL_Color){.r = r, .g = g, .b = b, .a = 255};
	return (sdl_color);
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
	join_text_to_surface(sdl->ui_surface, surfacetext, pos, sdl->font.color.a);
	SDL_FreeSurface(surfacetext);
	return (rect);
}

t_rectangle	print_text_colored(t_sdlcontext *sdl, const char *text, t_point pos, uint32_t color)
{
	t_rectangle	rect;
	SDL_Color	c;
	SDL_Color	prev;
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;

	
	b = color & 0xFF;
	g = (color >> 8) & 0xFF;
	r = (color >> 16) & 0xFF;
	c = (SDL_Color){.r = r, .g = g, .b = b, .a = 255};
	prev = sdl->font.color;
	sdl->font.color = c;
	rect = print_text(sdl, text, pos);
	sdl->font.color = prev;
	return (rect);
}