/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolution_scaling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 06:55:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/13 13:50:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

/*
uint8_t	average(t_point sample, t_lightmap *lmap, t_map *map)
{
	t_point		subsample;
	t_point		avgsample;
	t_point		start;
	uint32_t	avg;
	int			hit;

	subsample.x = 0;
	subsample.y = 0;
	start = point_sub(sample, point_one());
	avg = 0;
	hit = 0;
	while (subsample.y < 3)
	{
		subsample.x = 0;
		while (subsample.x < 3)
		{
			avgsample = point_add(start, subsample);
			if (avgsample.x >= 0 && avgsample.y >= 0 && avgsample.x < map->size.x - 1 && avgsample.y < map->size.y - 1)
			{
			avg += lmap->data[avgsample.y * map->size.x + avgsample.x];
			hit++;
			}
			subsample.x++;
		}
		subsample.y++;
	}
	return ((uint8_t)(avg / hit));
}
*/

uint32_t	combine_colors(uint32_t clr1, uint32_t clr2)
{
	uint32_t	result;

	result = ((clr1 & 0xFF) + (clr2 & 0xFF)) / 2;
	result += ((
			((clr1 & (0xFF << 8)) >> 8)
			+
			((clr2 & (0xFF << 8)) >> 8))
				/ 2) << 8;
	result += ((
			((clr1 & (0xFF << 16)) >> 16)
			+
			((clr2 & (0xFF << 16)) >> 16))
				/ 2) << 16;
	return (result);
}

//#include <pthread.h>

void	rescale_surface(t_sdlcontext *sdl)
{
	static	uint32_t	*swap;
	t_point			p;
	t_point			sample;

	if (sdl->resolution_scaling >= 1.0f || sdl->resolution_scaling < 0.25f)
		return ;
	p.y = 0;
	uint32_t	pstride;
	uint32_t	sample_stride;
	while (p.y < sdl->window_h)
	{
		p.x = 0;
		pstride = p.y * sdl->window_w;
		sample.y = p.y * sdl->resolution_scaling;
		sample.y *= sdl->window_w;
		while (p.x < sdl->window_w)
		{
			sample.x = p.x * sdl->resolution_scaling;
			if (0/* && p.y < sdl->window_h - 2*/)
			{
				uint32_t	clr;
				uint32_t	clr2;
				clr = ((uint32_t *)sdl->surface->pixels)[sample.x + (sample.y)];
				int	sy = (p.y + 1) * sdl->resolution_scaling;
				sy *= sdl->window_w;
				clr2 = ((uint32_t *)sdl->surface->pixels)[sample.x + (sy)];
				sdl->scaling_buffer[p.x + pstride] = combine_colors(clr, clr2);
			} else {
				sdl->scaling_buffer[p.x + pstride] = ((uint32_t *)sdl->surface->pixels)[sample.x + (sample.y)];
			}
			p.x++;
		}
		p.y++;
	}
	swap = sdl->surface->pixels;
	sdl->surface->pixels = sdl->scaling_buffer;
	sdl->scaling_buffer = swap;
}