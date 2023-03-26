/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dynamic_light.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:01:01 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 21:17:34 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	parse_entity_lightmap(
	t_entity *entity, float light, uint32_t light_clr, int index)
{
	int			x;
	int			y;
	uint32_t	light_amount;

	y = 0;
	while (y < entity->map[index].size.y)
	{
		x = 0;
		while (x < entity->map[index].size.x)
		{
			light_amount = \
			entity->map[index].lightmap[y * entity->map[index].size.x + x];
			light_amount = get_light_amount(light, light_clr, light_amount);
			entity->map[index].lightmap[y * entity->map[index].size.x + x] = \
			light_amount;
			x++;
		}
		y++;
	}
}

static void	calculate_entitys_lightmap_for_light(
	t_entity *entity, t_entity *light_ent, int index)
{
	float	dist;
	t_light	*light;

	light = light_ent->component.data;
	if (!light)
		return ;
	dist = vector3_dist(light->world_position, \
	entity->occlusion.bounds.origin);
	if (dist > light->radius)
		return ;
	dist = 1.0f - (dist / light->radius);
	parse_entity_lightmap(entity, dist, light->clr, index);
}

static void	init_map(t_map *map)
{
	uint32_t	clr;

	clr = get_light_amount(LIGHT_AMBIENT, INT_MAX, 0);
	ft_bzero(map->texture, sizeof(uint32_t) * map->size.x * map->size.y);
	ft_memset(map->lightmap, clr, sizeof(uint32_t) * map->size.x * map->size.y);
}

void	dynamic_lit(t_entity *entity, struct s_world *world, int index)
{
	int				i;
	int				found;
	t_entitycache	*cache;

	if (!world)
		return ;
	init_map(&entity->map[index]);
	i = 0;
	found = 0;
	cache = &world->entitycache;
	while (found < cache->existing_entitycount && \
	i < cache->existing_entitycount)
	{
		if (cache->entities[i].status != es_free)
		{
			if (cache->entities[i].status == es_active && \
			cache->entities[i].component.type == COMP_LIGHT)
				calculate_entitys_lightmap_for_light(entity, \
				&cache->entities[i], index);
			found++;
		}
		i++;
	}
	parse_entity_texture(index, entity);
}
