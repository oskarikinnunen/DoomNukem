/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2023/03/06 17:07:13 by raho             ###   ########.fr       */
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
		doomlog(LOG_EC_TTF_RENDERTEXTBLENDED, NULL);
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

t_rectangle	print_text(t_sdlcontext *sdl, const char *text, t_point pos)
{
	SDL_Surface	*surfacetext;
	t_rectangle	rect;

	surfacetext = TTF_RenderText_Blended(sdl->font.font, text, sdl->font.color);
	if (!surfacetext)
		doomlog(LOG_EC_TTF_RENDERTEXTBLENDED, NULL);
	rect.position = pos;
	rect.size.x = surfacetext->w;
	rect.size.y = surfacetext->h;
	join_text_to_surface(sdl->surface, surfacetext, pos, sdl->font.color.a);
	SDL_FreeSurface(surfacetext);
	return (rect);
}

t_rectangle	print_text_colored(t_sdlcontext *sdl, const char *text, \
								t_point pos, uint32_t color)
{
	t_rectangle	rect;
	SDL_Color	c;
	SDL_Color	prev;
	t_rgba		temp;

	c = color32_to_sdlcolor(color);
	prev = sdl->font.color;
	sdl->font.color = c;
	rect = print_text(sdl, text, pos);
	sdl->font.color = prev;
	return (rect);
}
