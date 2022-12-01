/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:37:04 by raho              #+#    #+#             */
/*   Updated: 2022/12/01 19:57:18 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "colors.h"

void	join_text_boxed_to_surface(t_sdlcontext *sdl, SDL_Surface *new, t_point pos, t_point padding)
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

void	join_text_to_surface(SDL_Surface *base, SDL_Surface *new, t_point pos, uint8_t alpha)
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

void	join_surface_to_wsurface(SDL_Surface *src, SDL_Surface *dest)
{
	t_point	i;

	i.y = 0;
	while (i.y < dest->h)
	{
		i.x = 0;
		while (i.x < dest->w)
		{
			if (((uint32_t *)src->pixels)[i.x + (i.y * src->w)] >> 24 == 0) // checking alpha
			{
				((uint32_t *)dest->pixels)[i.x + (i.y * dest->w)] = \
						blend_colors_alpha(((uint32_t *)dest->pixels)[i.x + (i.y * dest->w)], \
						((uint32_t *)src->pixels)[i.x + (i.y * src->w)], src->format, 255);
			}
			else
			{
				((uint32_t *)dest->pixels)[i.x + (i.y * dest->w)] = \
						blend_colors_alpha(((uint32_t *)dest->pixels)[i.x + (i.y * dest->w)], \
						((uint32_t *)src->pixels)[i.x + (i.y * src->w)], src->format, \
						((uint32_t *)src->pixels)[i.x + (i.y * src->w)] >> 24);
			}
			i.x++;
		}
		i.y++;
	}
}
