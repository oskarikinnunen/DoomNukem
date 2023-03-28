/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 13:48:22 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/28 09:17:47 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	allocate_map(t_entity *entity, int index, t_vector2 max)
{
	uint32_t	clr;
	t_map		*map;

	map = &entity->map[index];
	map->size = (t_point){ceilf(max.x) + 1, ceilf(max.y) + 1};
	map->img_size = entity->obj->materials[index].img->size;
	map->texture = prot_memalloc(sizeof(uint32_t) * map->size.x * map->size.y);
	map->lightmap = prot_memalloc(sizeof(uint32_t) * map->size.x * map->size.y);
	clr = get_light_amount(LIGHT_AMBIENT, INT_MAX, 0);
	ft_memset(map->lightmap, clr, sizeof(uint32_t) * map->size.x * map->size.y);
}

void	clear_map_for_entity(t_entity *entity)
{
	int	i;

	if (entity->map == NULL)
		return ;
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
	entity->map = NULL;
}

static bool	for_each_triangle(t_entity *entity)
{
	int			i;
	int			index;
	t_vector2	max;

	max = vector2_zero();
	i = 0;
	while (i < entity->obj->face_count)
	{
		max = get_max_vector2_from_vector3_triangle(
				max, entity->world_triangles[i].t);
		index = entity->obj->faces[i].material_index;
		if (i + 1 == entity->obj->face_count || \
		index != entity->obj->faces[i + 1].material_index)
		{
			max.x = fmaxf(max.x, 1.0f);
			max.y = fmaxf(max.y, 1.0f);
			if (max.x > 10000.0f || max.y > 10000.0f)
				return (false);
			allocate_map(entity, index, max);
			max = vector2_zero();
		}
		i++;
	}
	return (true);
}

static void	allocate_map_for_entity(t_entity *entity, t_world *world)
{
	t_object	*obj;
	t_vector2	max;
	int			index;
	int			i;

	obj = entity->obj;
	if (entity->map)
		clear_map_for_entity(entity);
	if (obj == NULL || obj->uv_count == 0 || entity->world_triangles == NULL)
		return ;
	entity->map = prot_memalloc(sizeof(t_map) * entity->obj->material_count);
	ft_bzero(entity->map, sizeof(t_map) * entity->obj->material_count);
	if (for_each_triangle(entity) == false)
		clear_map_for_entity(entity);
}

void	allocate_map_for_entities(t_world *world)
{
	int				i;
	int				found;
	t_entitycache	*cache;
	t_entity		*ent;

	i = 0;
	found = 0;
	cache = &world->entitycache;
	while (found < cache->existing_entitycount
		&& i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !entity_has_transparent_mat(ent))
				allocate_map_for_entity(ent, world);
			found++;
		}
		i++;
	}
}
