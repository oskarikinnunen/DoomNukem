/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_entity.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/23 21:49:31 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
 
#include "bresenham.h"
#include "objects.h"
#include "vectors.h"

void render_entity(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	render->screenspace_ptri_count = 0;
	/*t_world	w;
	w.sdl = sdl;
	render_entity_worldtriangles(entity, &w);*/
	if ((point_cmp(entity->occlusion.clip.max, point_zero()) && point_cmp(entity->occlusion.clip.min, point_zero()))
		|| !render->occlusion.occlusion)
	{
		render->screen_edge.max.x = (float)(sdl->window_w * sdl->resolution_scaling) - 1.0f;
		render->screen_edge.max.y = (float)(sdl->window_h * sdl->resolution_scaling) - 1.0f;
		render->screen_edge.min = vector2_zero();
	}
	else
	{
		render->screen_edge.min = point_to_vector2(entity->occlusion.clip.min);
		render->screen_edge.max = point_to_vector2(entity->occlusion.clip.max);
	}
	render->screen_edge.max.x = (float)(sdl->window_w * sdl->resolution_scaling) - 1.0f;
	render->screen_edge.max.y = (float)(sdl->window_h * sdl->resolution_scaling) - 1.0f;
	render->screen_edge.min = vector2_zero();
	render_world_triangles(sdl, render, entity);
}
