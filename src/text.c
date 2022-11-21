/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/11/21 21:22:30 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	join_surfaces_boxed(SDL_Surface *base, SDL_Surface *new, t_point pos)
{
	t_point		i;
	uint32_t	background;

	background = 0xFFFFFF;
	i.y = 0;
	while (i.y < new->h)
	{
		i.x = 0;
		while (i.x < new->w)
		{
			if (i.x + pos.x > 0 && i.x + pos.x < base->w && \
				i.y + pos.y > 0 && i.y + pos.y < base->h)
			{
				if (((uint32_t *)new->pixels)[i.x + (i.y * new->w)] >> 24 != 0) // checking alpha
					((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)] = ((uint32_t *)new->pixels)[i.x + (i.y * new->w)];
				else
					((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)] = background;
			}
			i.x++;
		}
		i.y++;
	}
}

static void	join_surfaces(SDL_Surface *base, SDL_Surface *new, t_point pos)
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
				if (((uint32_t *)new->pixels)[i.x + (i.y * new->w)] >> 24 != 0) // checking alpha
					((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)] = ((uint32_t *)new->pixels)[i.x + (i.y * new->w)];
			}
			i.x++;
		}
		i.y++;
	}
}

t_rectangle	print_ttftext_boxed(t_sdlcontext *sdl, const char *text, t_point pos)
{
	SDL_Surface	*surfacetext;
	t_rectangle	rect;

	surfacetext = TTF_RenderText_Blended(sdl->ttfont->font, text, sdl->ttfont->color);
	if (!surfacetext)
		error_log(EC_TTF_RENDERTEXTBLENDED);
	rect.position = pos;
	rect.size.x = surfacetext->w;
	rect.size.y = surfacetext->h;
	join_surfaces_boxed(sdl->surface, surfacetext, pos);
	SDL_FreeSurface(surfacetext);
	return (rect);
}

t_rectangle	print_ttftext(t_sdlcontext *sdl, const char *text, t_point pos)
{
	SDL_Surface	*surfacetext;
	t_rectangle	rect;

	surfacetext = TTF_RenderText_Blended(sdl->ttfont->font, text, sdl->ttfont->color);
	if (!surfacetext)
		error_log(EC_TTF_RENDERTEXTBLENDED);
	rect.position = pos;
	rect.size.x = surfacetext->w;
	rect.size.y = surfacetext->h;
	join_surfaces(sdl->surface, surfacetext, pos);
	SDL_FreeSurface(surfacetext);
	return (rect);
}
