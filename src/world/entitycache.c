/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitycache.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:13:16 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 16:32:52 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"

t_entity	*spawn_entity(t_world *world, t_object *obj)
{
	int				i;
	t_entitycache	*cache;

	cache = &world->entitycache;
	i = 0;
	while (i < cache->alloc_count)
	{
		if (cache->entities[i].status == es_free)
		{
			ft_bzero(&cache->entities[i], sizeof(t_entity));
			cache->entities[i].status = es_active;
			cache->entities[i].transform.scale = vector3_one();
			cache->entities[i].id = i;
			cache->entities[i].hidden = false;
			ft_bzero(&cache->entities[i].component, sizeof(t_component));
			entity_assign_object(world, &cache->entities[i], obj);
			cache->existing_entitycount++;
			if (cache->existing_entitycount >= cache->alloc_count)
				doomlog(LOG_EC_MALLOC, "2 many entities,increase cache size!");
			return (&cache->entities[i]);
		}
		i++;
	}
	doomlog(LOG_EC_MALLOC, NULL);
	return (NULL);
}

void	entity_assign_object(t_world *world, t_entity *entity, t_object *obj)
{
	entity->obj = obj;
	if (entity->world_triangles != NULL)
	{
		free(entity->world_triangles);
		entity->world_triangles = NULL;
	}
	if (obj == NULL)
		return ;
	ft_strncpy_term(entity->object_name.str, obj->name, 60);
	entity->world_triangles = \
			(t_world_triangle *)prot_memalloc(sizeof(t_world_triangle) \
			* obj->face_count);
	entity->occlusion.world_tri_count = ft_min(12, obj->face_count);
}

void	for_all_entities(t_world	*world,
			void (*func)(t_entity *ent, t_world *world))
{
	int				i;
	int				found;
	t_entitycache	*cache;

	i = 0;
	found = 0;
	cache = &world->entitycache;
	while (found < cache->existing_entitycount)
	{
		if (cache->entities[i].status != es_free)
		{
			func(&cache->entities[i], world);
			found++;
		}
		i++;
	}
}

void	void_for_all_active_entities(t_world	*world,
		void *ptr, void (*func)(t_entity *ent, void *ptr))
{
	int				i;
	int				found;
	t_entitycache	*cache;

	i = 0;
	found = 0;
	cache = &world->entitycache;
	while (found < cache->existing_entitycount)
	{
		if (cache->entities[i].status != es_free)
		{
			if (cache->entities[i].status == es_active)
			{
				func(&cache->entities[i], ptr);
			}
			found++;
		}
		i++;
	}
}

void	for_all_active_entities(t_world	*world,
		void (*func)(t_entity *ent, t_world *world))
{
	int				i;
	int				found;
	t_entitycache	*cache;

	i = 0;
	found = 0;
	cache = &world->entitycache;
	while (found < cache->existing_entitycount)
	{
		if (cache->entities[i].status != es_free)
		{
			if (cache->entities[i].status == es_active)
			{
				func(&cache->entities[i], world);
			}
			found++;
		}
		i++;
	}
}
