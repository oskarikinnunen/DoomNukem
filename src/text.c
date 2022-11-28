/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/11/28 21:13:32 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

// background color could be sent as an argument
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
