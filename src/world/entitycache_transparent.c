/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitycache_transparent.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:18:30 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/23 15:47:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"


bool	entity_has_transparent_mat(t_entity *entity)
{
	int i;

	if (entity->obj == NULL)
		return ;
	i = 0;
	while (i < entity->obj->material_count)
	{
		if (entity->obj->materials[i].img->transparency == true)
			return (true);
		i++;
	}
	return (false);
}

void	update_transparent(t_sdlcontext *sdl, t_world *world, t_render *render)
{
	int			i;
	t_entity	*ent;
	
	i = world->entitycache.alloc_count - 1;
	while (i >= 0)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free && entity_has_transparent_mat(ent))
		{
			if(ent->component.func_update != NULL)
				ent->component.func_update(ent, world);
			if (is_entity_culled(sdl, render, ent) == false)
				render_entity(sdl, render, ent);
		}
		i--;
	}
}