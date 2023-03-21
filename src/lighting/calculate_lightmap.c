/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_lightmap.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:14:49 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/21 18:31:41 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void buffer_triangles(int start, int i, t_lighting *lighting, t_entity *entity)
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
		return;
	i = 0;
	start = 0;
	while (i < obj->face_count)
	{
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || index != obj->faces[i + 1].materialindex)
		{
			lighting->map = &entity->map[index];
			lighting->img = entity->obj->materials[index].img;
			buffer_triangles(start, i, lighting, entity);
		}
		i++;
	}
}

void	*calculate_light_for_entities(t_test *ptr)
{
	int				i;
	int				found;
	t_entitycache	*cache;
	t_entity		*ent;
	t_light			*light;
	float			dist;
	t_lighting		lighting;

	if (ptr->world == NULL)
		return(NULL);
	lighting.light_ent = ptr->entity;
	light = ptr->entity->component.data;
	i = 0;
	found = 0;
	cache = &ptr->world->entitycache;
	while (found < cache->existing_entitycount
		&& i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status != es_free)
		{
			if	(ent->status == es_active && ent->obj && ent->map)
			{
				dist = vector3_dist(light->world_position, get_entity_world_position(ent)) - ent->obj->bounds.radius;
				if (dist < light->radius)
					calculate_light_for_entity(ent, &lighting);
			}
			found++;
		}
		i++;
	}
	return(NULL);
}

void	calculate_lighting(t_world *world)
{
	int             j;
    int				i;
	int				found;
	t_entity		*ent;
    uint32_t        light_amount;
    t_quaternion    q;

	light_amount = 0;
	i = 0;
	found = 0;
	t_thread test;
	test.func = (void *)calculate_light_for_entities;
	test.struct_size = sizeof(t_test);
	test.structs = malloc(sizeof(t_test) * THREAD);
	test.count = 0;
	ft_bzero(test.structs, sizeof(t_test) * THREAD);
	while (found < world->entitycache.existing_entitycount
		&& i < world->entitycache.alloc_count)
	{
		ent = &world->entitycache.entities[i];
		if (ent->status != es_free)
		{
			if	(ent->status == es_active && ent->component.type == COMP_LIGHT)
			{
				t_test *ptr;
				ptr = &(((t_test *)test.structs)[test.count++]);
				ptr->entity = ent;
				ptr->world = world;
				if (test.count == THREAD)
				{
					thread_set(&test);
					ft_bzero(test.structs, sizeof(t_test) * THREAD);
				}
			}
			found++;
		}
		i++;
	}
	thread_set(&test);
	free(test.structs);
}
