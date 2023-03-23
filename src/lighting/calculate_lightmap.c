/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_lightmap.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:14:49 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/23 19:42:31 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	buffer_triangles(
	int start, int i, t_lighting *lighting, t_entity *entity)
{
	t_point_triangle	temp;
	int					vertex;
	size_t				size;

	size = lighting->map->size.x * lighting->map->size.y * sizeof(bool);
	lighting->overdraw = malloc(size);
	if (lighting->overdraw == NULL)
		doomlog(LOG_FATAL, "Malloc failed in bake_lighting.c");
	ft_bzero(lighting->overdraw, size);
	while (start <= i)
	{
		vertex = 0;
		while (vertex < 3)
		{
			temp.t[vertex] = entity->world_triangles[start].p[vertex].v;
			temp.p[vertex].x = entity->world_triangles[start].t[vertex].x;
			temp.p[vertex].y = entity->world_triangles[start].t[vertex].y;
			vertex++;
		}
		rasterize_light(temp, lighting);
		start++;
	}
	free(lighting->overdraw);
}

static void	calculate_light_for_entity(t_entity *entity, t_lighting *lighting)
{
	t_object	*obj;
	int			index;
	int			start;
	int			i;

	obj = entity->obj;
	if (obj->uv_count == 0 || entity->world_triangles == NULL)
		return ;
	i = 0;
	start = 0;
	while (i < obj->face_count)
	{
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || \
		index != obj->faces[i + 1].materialindex)
		{
			lighting->map = &entity->map[index];
			lighting->img = entity->obj->materials[index].img;
			buffer_triangles(start, i, lighting, entity);
		}
		i++;
	}
}

static void	calculate_if_valid(t_entity *entity, t_lighting *lighting)
{
	float	dist;

	if (entity->status == es_active && entity->obj && entity->map)
	{
		dist = vector3_dist(lighting->light->world_position, \
		get_entity_world_position(entity)) - entity->obj->bounds.radius;
		if (dist < lighting->light->radius)
			calculate_light_for_entity(entity, lighting);
	}
}

void	*calculate_light_for_entities(t_test *ptr)
{
	int				i;
	int				found;
	t_entitycache	*cache;
	t_entity		*ent;
	t_lighting		lighting;

	if (ptr->world == NULL)
		return (NULL);
	lighting.entity = ptr->entity;
	lighting.light = ptr->entity->component.data;
	i = 0;
	found = 0;
	cache = &ptr->world->entitycache;
	while (found < cache->existing_entitycount
		&& i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status != es_free)
		{
			calculate_if_valid(ent, &lighting);
			found++;
		}
		i++;
	}
	return (NULL);
}

void	calculate_lighting(t_world *world)
{
	int				j;
	int				i;
	int				found;
	t_entity		*ent;
	t_thread		thread;

	i = 0;
	found = 0;
	thread.func = (void *)calculate_light_for_entities;
	thread.struct_size = sizeof(t_test);
	thread.structs = malloc(sizeof(t_test) * THREAD);
	thread.count = 0;
	ft_bzero(thread.structs, sizeof(t_test) * THREAD);
	while (found < world->entitycache.existing_entitycount
		&& i < world->entitycache.alloc_count)
	{
		ent = &world->entitycache.entities[i++];
		if (ent->status != es_free && ent->map != NULL)
		{
			set_light_thread_struct(&thread, ent, world);
			found++;
		}
	}
	multithread_start(&thread);
	free(thread.structs);
}
