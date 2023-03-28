/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitycache_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:11 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 09:18:32 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"

void	destroy_entity(t_world *world, t_entity *ent)
{
	t_entitycache	*cache;

	cache = &world->entitycache;
	if (ent->id >= cache->alloc_count)
		doomlog_mul(LOG_EC_MALLOC, (char *[4]){
			"Tried to remove entity ", s_itoa(ent->id),
			", which exceeds the allocated cache size.", NULL
		});
	cache->entities[ent->id].status = es_free;
	clear_map_for_entity(ent);
	if (ent->destroy_obj)
		free_object(&ent->obj);
	cache->entities[ent->id].obj = NULL;
	if (ent->world_triangles != NULL)
		free(ent->world_triangles);
	ent->world_triangles = NULL;
	if (ent->component.data != NULL)
		free(ent->component.data);
	ent->component.data = NULL;
	if (cache->existing_entitycount == 0)
		doomlog(LOG_EC_MALLOC, "Tried to remove entity -1\n");
	cache->existing_entitycount--;
}

t_entity	*find_entity_with_comp(t_world	*world, t_component_type comp)
{
	int				i;
	t_entitycache	*cache;

	cache = &world->entitycache;
	i = 0;
	while (i < cache->alloc_count)
	{
		if (cache->entities[i].status == es_active
			&& cache->entities[i].component.type == comp)
			return (&cache->entities[i]);
		i++;
	}
	return (NULL);
}

void	update_lifetime(t_entity *entity, t_world *world)
{
	if (entity->life_timeable)
	{
		entity->life_time -= world->clock.delta;
		if (entity->life_time <= 0)
			destroy_entity(world, entity);
	}
}
