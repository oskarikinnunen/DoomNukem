/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:37:04 by raho              #+#    #+#             */
/*   Updated: 2022/12/20 11:55:00 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "colors.h"

void	join_text_boxed_to_surface(t_sdlcontext *sdl, SDL_Surface *src, t_point pos, t_point padding)
{
	t_point		i;

	i.y = 0 - padding.y;
	while (i.y < src->h + padding.y)
	{
		i.x = 0 - padding.x;
		while (i.x < src->w + padding.x)
		{
			if (i.x + pos.x > 0 && i.x + pos.x < sdl->ui_surface->w && \
				i.y + pos.y > 0 && i.y + pos.y < sdl->ui_surface->h)
			{
				if (i.x >= 0 && i.x < src->w && i.y >= 0 && i.y < src->h)
				{	
					if (((uint32_t *)src->pixels)[i.x + (i.y * src->w)] >> 24 > 0) // checking alpha
					{
						((uint32_t *)sdl->ui_surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->ui_surface->w)] = \
								blend_colors_alpha(((uint32_t *)sdl->ui_surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->ui_surface->w)], \
								((uint32_t *)src->pixels)[i.x + (i.y * src->w)], sdl->font.color.a);
					}
					else
					{
						((uint32_t *)sdl->ui_surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->ui_surface->w)] = \
								blend_colors_alpha(((uint32_t *)sdl->ui_surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->ui_surface->w)], \
								sdl->font.box_color, sdl->font.color.a);
					}
				}
				else
				{
					((uint32_t *)sdl->ui_surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->ui_surface->w)] = \
							blend_colors_alpha(((uint32_t *)sdl->ui_surface->pixels)[(i.x + pos.x) + ((i.y + pos.y) * sdl->ui_surface->w)], \
							sdl->font.box_color, sdl->font.color.a);
				}
			}
			i.x++;
		}
		i.y++;
	}
}

void	join_text_to_surface(SDL_Surface *dest, SDL_Surface *src, t_point pos, uint8_t alpha)
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
				if ((((uint32_t *)src->pixels)[i.x + (i.y * src->w)] >> 24) > 0) // checking alpha
				{
					((uint32_t *)dest->pixels)[(i.x + pos.x) + ((i.y + pos.y) * dest->w)] = \
							blend_colors_alpha(((uint32_t *)dest->pixels)[(i.x + pos.x) + ((i.y + pos.y) * dest->w)], \
							((uint32_t *)src->pixels)[i.x + (i.y * src->w)], alpha);
				}
			}
			i.x++;
		}
		i.y++;
	}
}

void	join_surfaces(SDL_Surface *dest, SDL_Surface *src)
{
	t_point	i;
	int		stride;

	i.y = 0;
	while (i.y < dest->h)
	{
		i.x = 0;
		stride = i.y * dest->w;
		while (i.x < dest->w)
		{
			if (i.x < src->w && i.y < src->h && ((uint32_t *)src->pixels)[i.x + stride] != 0)
				((uint32_t *)dest->pixels)[i.x + stride] = ((uint32_t *)src->pixels)[i.x + stride];
			i.x++;
		}
		i.y++;
	}
}
