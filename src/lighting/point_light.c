/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:01:16 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 19:52:35 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	render_shadowmap(t_world *world, t_lighting *l)
{
	int			i;
	int			found;
	t_entity	*ent;
	bool		is_valid;

	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden
				&& !entity_has_transparent_mat(ent)
				&& !ent->dynamic_lit)
			{
				is_valid = (l->light->ignoreself && ent->id != l->entity->id);
				if (l->light->ignoreself == false || is_valid)
					render_zbuffer(l, ent);
			}
			found++;
		}
		i++;
	}
}

static void	update_cubemap_cameras(t_entity *entity)
{
	t_light		*light;
	t_mat4x4	matproj;
	int			i;

	light = entity->component.data;
	if (light == NULL)
		return ;
	light->world_position = \
	transformed_vector3(entity->transform, light->origin).v;
	matproj = matrix_makeprojection(90.0f, light->cubemap.resolution.y \
	/ light->cubemap.resolution.x, 2.0f, 1000.0f);
	i = 0;
	while (i < 6)
	{
		light->cubemap.shadowmaps[i] = (float *)prot_memalloc(sizeof(float) * \
		(light->cubemap.resolution.x * light->cubemap.resolution.y));
		light->cubemap.cameras[i].position = light->world_position;
		light->cubemap.cameras[i].matproj = matproj;
		calculate_matview(&light->cubemap.cameras[i]);
		i++;
	}
}

static void	calculate_pointlight(t_world *world, t_entity *entity)
{
	int				index;
	t_lighting		l;

	update_cubemap_cameras(entity);
	l.light = entity->component.data;
	l.resolution = l.light->cubemap.resolution;
	l.entity = entity;
	index = 0;
	while (index < 6)
	{
		l.camera = l.light->cubemap.cameras[index];
		l.zbuffer = l.light->cubemap.shadowmaps[index];
		ft_bzero(l.zbuffer, sizeof(float) * l.light->cubemap.resolution.x \
		* l.light->cubemap.resolution.y);
		render_shadowmap(world, &l);
		index++;
	}
}

void	recalculate_pointlight(t_world *world)
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
		ent = &cache->entities[i++];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && \
			ent->component.type == COMP_LIGHT)
			{
				calculate_pointlight(world, ent);
			}
			found++;
		}
	}
}
