/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dynamic_light.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:01:01 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 16:32:46 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

inline static void	sample_pixel(
	t_point xy, t_lighting *lighting, uint32_t light)
{
	uint32_t	clr;
	t_img		*img;

	img = lighting->img;
	clr = img->data[(xy.y % img->size.y) * img->size.x + (xy.x % img->size.x)];
	clr = update_pixel_brightness(light, clr);
	lighting->map->texture[xy.y * lighting->map->size.x + xy.x] = clr;
}

static void	combine_texture_and_light(
	int index, t_entity *entity, uint32_t light_amount)
{
	t_screen_triangle	temp;
	t_lighting			lighting;
	t_point				xy;

	if (!entity->map)
		return ;
	lighting.map = &entity->map[index];
	lighting.img = entity->obj->materials[index].img;
	xy.y = 0;
	while (xy.y < lighting.map->size.y)
	{
		xy.x = 0;
		while (xy.x < lighting.map->size.x)
		{
			sample_pixel(xy, &lighting, light_amount);
			xy.x++;
		}
		xy.y++;
	}
}

static uint32_t	calculate_entitys_light_amount(
t_entity *entity, t_entity *light_ent, t_world *world, uint32_t light_amount)
{
	float			dist;
	t_light			*light;
	t_ray			r;
	t_raycast_info	ri;
	bool			temp;

	light = light_ent->component.data;
	if (!light)
		return (light_amount);
	temp = entity->ignore_raycasts;
	entity->ignore_raycasts = true;
	dist = vector3_dist(light->world_position, \
	entity->occlusion.bounds.origin);
	r.dir = vector3_sub(light->world_position, entity->occlusion.bounds.origin);
	r.origin = entity->occlusion.bounds.origin;
	if (dist > light->radius || (raycast(r, &ri, world) && ri.distance < dist))
	{
		entity->ignore_raycasts = temp;
		return (light_amount);
	}
	entity->ignore_raycasts = temp;
	dist = 1.0f - (dist / light->radius);
	light_amount = get_light_amount(dist, light->clr, light_amount);
	return (light_amount);
}

static uint32_t	init_map(t_map *map)
{
	uint32_t	clr;

	clr = get_light_amount(LIGHT_AMBIENT, INT_MAX, 0);
	ft_bzero(map->texture, sizeof(uint32_t) * map->size.x * map->size.y);
	ft_memset(map->lightmap, clr, sizeof(uint32_t) * map->size.x * map->size.y);
	return (clr);
}

void	dynamic_lit(t_entity *entity, struct s_world *world, int index)
{
	int				i;
	int				found;
	t_entitycache	*cache;
	uint32_t		light_amount;

	if (!world)
		return ;
	light_amount = init_map(&entity->map[index]);
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
				light_amount = calculate_entitys_light_amount(entity, \
				&cache->entities[i], world, light_amount);
			found++;
		}
		i++;
	}
	combine_texture_and_light(index, entity, light_amount);
}
