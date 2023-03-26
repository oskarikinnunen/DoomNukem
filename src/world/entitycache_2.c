/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitycache_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:11 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 17:47:38 by raho             ###   ########.fr       */
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
	cache->entities[ent->id].obj = NULL;
	if (ent->world_triangles != NULL)
		free(ent->world_triangles);
	if (cache->existing_entitycount == 0)
		doomlog(LOG_EC_MALLOC, "Tried to remove entity -1\n");
	cache->existing_entitycount--;
}

t_entity	*find_entity_with_comp(t_world	*world, t_componenttype comp)
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
