/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bake_lighting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:00:29 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/16 16:31:05 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void sample_pixel(int x, int y, int index, t_entity *entity)
{
	uint32_t	light;
	uint32_t	clr;
	t_img		*img;

	img = entity->obj->materials[index].img;
	light = entity->map->lightmap[y * entity->map->size.x + x];
	clr = img->data[(y % (img->size.y)) * img->size.x + (x % (img->size.x))];
	clr = update_pixel_brightness(light, clr);
	entity->map[index].texture[y * entity->map[index].size.x + x] = clr;
}

void create_map_for_entity(t_entity *entity, struct s_world *world)
{
	int			y;
	int			x;
	int			index;

	if (!entity->map)
		return;
	index = 0;
	while (index < entity->obj->material_count)
	{
		y = 0;
		while (y < entity->map->size.y)
		{
			x = 0;
			while (x < entity->map->size.x)
			{
				sample_pixel(x, y, index, entity);
				x++;
			}
			y++;
		}
		index++;
	}
}

t_vector2	get_max_uv(t_vector2 max, t_vector3 t[3])
{
	t_vector3	uv;
	int			i;

	i = 0;
	while (i < 3)
	{
		uv = t[i];
		if (uv.x > max.x)
			max.x = uv.x;
		if (uv.y > max.y)
			max.y = uv.y;
		i++;
	}
	return(max);
}

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

void calculate_light_for_entity(t_entity *entity, t_lighting *lighting)
{
	t_object	*obj;
	t_vector2	max;
	int			index;
	int			start;
	int			i;

	obj = entity->obj;
	if (obj->uv_count == 0 || entity->world_triangles == NULL)
		return;
	i = 0;
	start = 0;
	max = vector2_zero();
	while (i < obj->face_count)
	{
		max = get_max_uv(max, entity->world_triangles[i].t);
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || index != obj->faces[i + 1].materialindex)
		{
			max.x = fmaxf(max.x, 1.0f);
			max.y = fmaxf(max.y, 1.0f);
			lighting->map = &entity->map[index];
			lighting->img = entity->obj->materials[index].img;
			buffer_triangles(start, i, lighting, entity);
			max = vector2_zero();
		}
		i++;
	}
}

typedef struct s_test
{
	t_world *world;
	t_entity *entity;
} t_test;

void	calculate_light_for_entities(void *ptr)
{
	t_test *test = ptr;
	t_world *world = test->world;
	t_entity *light_ent = test->entity;
	int				i;
	int				found;
	t_entitycache	*cache;
	t_entity		*ent;
	t_light			*light;
	float			dist;
	t_lighting		lighting;

	if (world == NULL)
		return;
	lighting.light_ent = light_ent;
	light = light_ent->component.data;
	i = 0;
	found = 0;
	cache = &world->entitycache;
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
}

static void allocate_map(t_map *map, t_point size, t_vector2 max)
{
	uint32_t clr;

	map->size = (t_point){ceilf(max.x) + 1, ceilf(max.y) + 1};
	map->img_size = size;
	map->texture = malloc(sizeof(uint32_t) * map->size.x * map->size.y);
	map->lightmap = malloc(sizeof(uint32_t) * map->size.x * map->size.y);
	if (map->texture == NULL || map->lightmap == NULL)
		doomlog(LOG_FATAL, "Malloc fail in bake_lighting.c");
	clr = get_light_amount(LIGHT_AMBIENT, INT_MAX, 0);
	ft_memset(map->lightmap, clr, sizeof(uint32_t) * map->size.x * map->size.y);
}

static void clear_map_for_entity(t_entity *entity)
{
	int i;

	if (entity->map == NULL)
		return;
	i = 0;
	while (i < entity->obj->material_count)
	{
		if (entity->map[i].texture)
			free(entity->map[i].texture);
		if (entity->map[i].lightmap)
			free(entity->map[i].lightmap);
		i++;
	}
	free(entity->map);
}

void	create_lightmap_for_entity(t_entity *entity, t_world *world)
{
	t_object	*obj;
	t_vector2	max;
	int			index;
	int			i;

	obj = entity->obj;
	if (obj->uv_count == 0 || entity->world_triangles == NULL)
	{
		entity->map = NULL;
		return;
	}
	if (entity->map)
		clear_map_for_entity(entity);
	entity->map = malloc(sizeof(t_map) * entity->obj->material_count);
	bzero(entity->map, sizeof(t_map) * entity->obj->material_count);
	if (entity->map == NULL)
		doomlog(LOG_FATAL, "Malloc fail in bake_lighting.c");
	i = 0;
	max = vector2_zero();
	while (i < obj->face_count)
	{
		max = get_max_uv(max, entity->world_triangles[i].t);
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || index != obj->faces[i + 1].materialindex)
		{
			max.x = fmaxf(max.x, 1.0f);
			max.y = fmaxf(max.y, 1.0f);
			allocate_map(&entity->map[index], obj->materials[index].img->size, max);
			max = vector2_zero();
		}
		i++;
	}
}

void	calculate_lighting(t_world *world)
{
	int             j;
    int				i;
	int				found;
	t_entity		*ent;
    uint32_t        light_amount;
    t_quaternion    q;
	int e = 0;

	for_all_active_entities(world, create_lightmap_for_entity);
	light_amount = 0;
	i = 0;
	found = 0;
	t_thread test;
	test.func = thread_func_lighting;
	test.init = thread_init_lighting;
	test.struct_size = sizeof(t_test);
	test.structs = malloc(sizeof(t_test) * THREAD);
	while (found < world->entitycache.existing_entitycount
		&& i < world->entitycache.alloc_count)
	{
		ent = &world->entitycache.entities[i];
		if (ent->status != es_free)
		{
			if	(ent->status == es_active && ent->component.type == COMP_LIGHT)
			{
				t_test *ptr;
				ptr = &(((t_test *)test.structs)[e]);
				ptr->entity = ent;
				ptr->world = world;
				//calculate_light_for_entities(world, ent);
				e++;
			}
			if (e == 6)
			{
				thread_set(&test);
				e = 0;
			}
			found++;
		}
		i++;
	}
	if (e != 0)
	{
		thread_set(&test);
	}
}

void	recalculate_lighting(t_world *world)
{
	int				i;
	int				found;
	t_entitycache	*cache;
	t_entity		*ent;

	recalculate_pointlight(world);
	calculate_lighting(world);
	return;
	i = 0;
	found = 0;
	cache = &world->entitycache;
	while (found < cache->existing_entitycount
		&& i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status != es_free)
		{
			if	(ent->status == es_active && ent->obj != NULL)
			{
				create_map_for_entity(ent, world);
			}
			found++;
		}
		i++;
	}
}