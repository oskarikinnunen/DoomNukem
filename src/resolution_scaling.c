/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolution_scaling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 06:55:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 11:23:14 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

static inline uint32_t	darken(uint32_t clr, float lerp)
{
	uint32_t	final;

	final = (clr & 0xFF) * lerp;
	final += (uint32_t)((clr >> 8 & 0xFF) * lerp) << 8;
	final += (uint32_t)((clr >> 16 & 0xFF) * lerp) << 16;
	return (final);
}

static inline uint32_t	sample_fog(t_sdlcontext *sdl,
	t_point sample, uint32_t stride)
{
	float	zval;
	float	lerp;

	zval = sdl->zbuffer[sample.x + stride];
	if (zval < 0.0048f)
	{
		lerp = 1.0f - ((0.0048f - zval) * 350.0f);
		lerp = ft_clampf(lerp, 0.02f, 1.0f);
		return (darken(((uint32_t *)sdl->surface->pixels)
				[sample.x + stride], lerp));
	}
	return (((uint32_t *)sdl->surface->pixels)[sample.x + stride]);
}

static void	surface_fog(t_sdlcontext *sdl)
{
	t_point			sample;
	uint32_t		sample_stride;

	if (!sdl->fog)
		return ;
	sample.y = 0;
	while (sample.y < sdl->window_h * sdl->resolution_scaling)
	{
		sample.x = 0;
		sample_stride = sample.y * sdl->window_w;
		while (sample.x < sdl->window_w * sdl->resolution_scaling)
		{
			((uint32_t *)sdl->surface->pixels)[sample.x + sample_stride]
				= sample_fog(sdl, sample, sample_stride);
			sample.x++;
		}
		sample.y++;
	}
}

static inline void	swap_surfaces(t_sdlcontext *sdl)
{
	static uint32_t	*swap;

	swap = sdl->surface->pixels;
	sdl->surface->pixels = sdl->scaling_buffer;
	sdl->scaling_buffer = swap;
}

void	rescale_surface(t_sdlcontext *sdl)
{
	t_point			p;
	t_point			sample;
	uint32_t		pstride;

	surface_fog(sdl);
	if (sdl->resolution_scaling >= 1.0f || sdl->resolution_scaling < 0.25f)
		return ;
	p.y = 0;
	while (p.y < sdl->window_h)
	{
		p.x = 0;
		pstride = p.y * sdl->window_w;
		sample.y = (int)(p.y * sdl->resolution_scaling) * sdl->window_w;
		while (p.x < sdl->window_w)
		{
			sample.x = p.x * sdl->resolution_scaling;
			sdl->scaling_buffer[p.x + pstride]
				= ((uint32_t *)sdl->surface->pixels)[sample.x + (sample.y)];
			p.x++;
		}
		p.y++;
	}
	swap_surfaces(sdl);
}
