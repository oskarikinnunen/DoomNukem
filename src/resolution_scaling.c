/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolution_scaling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 06:55:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/07 07:28:08 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

void	rescale_surface(t_sdlcontext *sdl)
{
	static	uint32_t	*temp;
	static	uint32_t	*swap;
	t_point			p;
	t_point			sample;

	if (sdl->resolution_scaling == 1.0f)
		return ;
	if (temp == NULL)
		temp = ft_memalloc(sdl->window_w * sdl->window_h * sizeof(uint32_t));
	/*else
		bzero(temp, sdl->window_w * sdl->window_h * sizeof(uint32_t));*/
	p.y = 0;
	uint32_t	pstride;
	uint32_t	sample_stride;
	while (p.y < sdl->window_h)
	{
		p.x = 0;
		pstride = p.y * sdl->window_w;
		sample.y = p.y * sdl->resolution_scaling;
		while (p.x < sdl->window_w)
		{
			sample.x = p.x * sdl->resolution_scaling;
			temp[p.x + pstride] = ((uint32_t *)sdl->surface->pixels)[sample.x + (sample.y * sdl->window_w)];
			p.x++;
		}
		p.y++;
	}
	swap = sdl->surface->pixels;
	sdl->surface->pixels = temp;
	temp = swap;
	//memcpy(sdl->surface->pixels, temp, sdl->window_w * sdl->window_h * sizeof(uint32_t));
}