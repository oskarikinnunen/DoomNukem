/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frustrum_culling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:05:28 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 15:25:15 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool	is_entity_frustrum_culled(
	t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	if (entity->obj == NULL)
		return (true);
	calculate_triangles(*sdl, render, entity);
	set_square_from_triangles(&entity->occlusion, \
	render->screenspace_ptris, render->screenspace_ptri_count);
	if (render->screenspace_ptri_count > 0)
	{
		if (sdl->bitmask.max_dist < entity->occlusion.z_dist[0])
			sdl->bitmask.max_dist = entity->occlusion.z_dist[0];
		return (false);
	}
	return (true);
}

void	update_frustrum_culling(
	struct s_world *world, t_sdlcontext *sdl, t_render *render)
{
	int			i;
	int			found;
	t_entity	*ent;

	sdl->bitmask.last_max_dist = sdl->bitmask.max_dist;
	sdl->bitmask.max_dist = 0.0f;
	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount && \
	i < world->entitycache.alloc_count)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden && ent->obj)
				ent->occlusion.is_occluded = \
				is_entity_frustrum_culled(sdl, render, ent);
			found++;
		}
		i++;
	}
}
