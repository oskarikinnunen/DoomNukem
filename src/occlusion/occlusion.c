/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   occlusion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:40:03 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 12:00:59 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void default_entity_occlusion_settings(t_entity *e, t_world *world)
{
	e->occlusion.is_backface_cull = true;
	e->occlusion.is_occluded = false;
}

void default_floor_occlusion_settings(t_meshtri *f, t_world *world)
{
	f->entity->occlusion.is_backface_cull = false;
	f->entity->occlusion.is_occluded = false;
}

void default_wall_occlusion_settings(t_wall *w, t_world *world)
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
	return(bitmask);
}

bool is_entity_culled(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	uint32_t	clr;

	if (entity->occlusion.is_occluded == false)
	{
		entity->occlusion.is_occluded = is_entity_occlusion_culled(sdl, render, entity);
		if (render->occlusion.occluder_box == true)
		{
			if (entity->occlusion.is_occluded == true)
				clr = CLR_RED;
			else
			{
				draw_wireframe(*sdl, entity->occlusion.clip, CLR_TURQ);
				clr = CLR_GREEN;
			}
			draw_wireframe(*sdl, entity->occlusion.box, clr);
		}
		if (sdl->render.occlusion.slow_render == true)
		{
			memcpy(sdl->window_surface->pixels, sdl->surface->pixels, sizeof(uint32_t) * sdl->window_w * sdl->window_h);
			SDL_UpdateWindowSurface(sdl->window);
			usleep(100000);
		}
	}
	return(entity->occlusion.is_occluded);
}
