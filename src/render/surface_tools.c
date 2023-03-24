/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:37:04 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 19:25:47 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"

void	update_window_surface(t_sdlcontext *sdl)
{
	ft_memcpy(sdl->window_surface->pixels, sdl->surface->pixels, \
				sizeof(uint32_t) * sdl->window_w * sdl->window_h);
	if (SDL_UpdateWindowSurface(sdl->window) < 0)
		doomlog(LOG_EC_SDL_UPDATEWINDOWSURFACE, NULL);
}

static void	blend_text(t_sdlcontext *sdl, SDL_Surface *src, \
									t_point pos, t_point i)
{
	((uint32_t *)sdl->surface->pixels)[\
		(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = \
		blend_colors_alpha(((uint32_t *)sdl->surface->pixels)[\
		(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)], \
		((uint32_t *)src->pixels)[i.x + (i.y * src->w)], \
							sdl->font_default->color.a);
}

static void	blend_box(t_sdlcontext *sdl, SDL_Surface *src, \
									t_point pos, t_point i)
{
	((uint32_t *)sdl->surface->pixels)[\
		(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)] = \
		blend_colors_alpha(((uint32_t *)sdl->surface->pixels)[\
		(i.x + pos.x) + ((i.y + pos.y) * sdl->surface->w)], \
		sdl->font_default->box_color, sdl->font_default->color.a);
}

void	join_text_boxed_to_surface(t_sdlcontext *sdl, SDL_Surface *src, \
									t_point pos, t_point padding)
{
	t_point		i;

	i.y = 0 - padding.y;
	while (i.y < src->h + padding.y)
	{
		i.x = 0 - padding.x;
		while (i.x < src->w + padding.x)
		{
			if (i.x + pos.x > 0 && i.x + pos.x < sdl->surface->w && \
				i.y + pos.y > 0 && i.y + pos.y < sdl->surface->h)
			{
				if ((i.x >= 0 && i.x < src->w && i.y >= 0 && i.y < src->h) && \
						(((uint32_t *)src->pixels)[\
						i.x + (i.y * src->w)] >> 24 > 0))
					blend_text(sdl, src, pos, i);
				else
					blend_box(sdl, src, pos, i);
			}
			i.x++;
		}
		i.y++;
	}
}

void	join_text_to_surface(SDL_Surface *dest, \
							SDL_Surface *src, t_point pos, uint8_t alpha)
{
	t_point	i;

	i.y = 0;
	while (i.y < src->h)
	{
		i.x = 0;
		while (i.x < src->w)
		{
			if (i.x + pos.x > 0 && i.x + pos.x < dest->w && \
				i.y + pos.y > 0 && i.y + pos.y < dest->h)
			{
				if ((((uint32_t *)src->pixels)[i.x + (i.y * src->w)] >> 24) > 0)
				{
					((uint32_t *)dest->pixels)[(i.x + pos.x) + \
						((i.y + pos.y) * dest->w)] = \
						blend_colors_alpha(((uint32_t *)dest->pixels)[\
						(i.x + pos.x) + ((i.y + pos.y) * dest->w)], \
						((uint32_t *)src->pixels)[i.x + (i.y * src->w)], alpha);
				}
			}
			i.x++;
		}
		i.y++;
	}
}
