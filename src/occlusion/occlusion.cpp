/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   occlusion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:40:03 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 16:32:39 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	default_entity_occlusion_settings(t_entity *e, t_world *world)
{
	e->occlusion.is_backface_cull = true;
	e->occlusion.is_occluded = false;
}

void	default_floor_occlusion_settings(t_meshtri *f, t_world *world)
{
	f->entity->occlusion.is_backface_cull = false;
	f->entity->occlusion.is_occluded = false;
}

void	default_wall_occlusion_settings(t_wall *w, t_world *world)
{
	w->entity->occlusion.is_backface_cull = false;
	w->entity->occlusion.is_occluded = false;
}

t_bitmask	init_bitmask(t_sdlcontext *sdl)
{
	t_bitmask	bitmask;

	bitmask.tile = prot_memalloc(sizeof(t_tile) * \
			((sdl->window_h * sdl->window_w) / 64));
	bitmask.bitmask_chunks.x = sdl->window_w / 16;
	bitmask.bitmask_chunks.y = sdl->window_h / 8;
	bitmask.tile_chunks.x = sdl->window_w / 8;
	bitmask.tile_chunks.y = sdl->window_h / 8;
	return (bitmask);
}

bool	is_entity_culled(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	uint32_t	clr;

	if (entity->obj && entity->occlusion.is_occluded == false && \
	render->occlusion.occlusion)
	{
		entity->occlusion.is_occluded = \
		is_entity_occlusion_culled(sdl, render, entity);
		if (sdl->render.occlusion.slow_render == true && \
		entity->occlusion.is_occluded == false)
		{
			ft_memcpy(sdl->window_surface->pixels, sdl->surface->pixels, \
			sizeof(uint32_t) * sdl->window_w * sdl->window_h);
			SDL_UpdateWindowSurface(sdl->window);
			usleep(100000);
		}
	}
	return (entity->occlusion.is_occluded);
}
