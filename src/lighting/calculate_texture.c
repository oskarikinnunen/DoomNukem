/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_texture.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 13:07:06 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 22:28:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	sample_pixel(t_point xy, t_lighting *lighting)
{
	uint32_t	light;
	uint32_t	clr;
	t_img		*img;

	img = lighting->img;
	light = lighting->map->lightmap[xy.y * lighting->map->size.x + xy.x];
	clr = img->data[(xy.y % img->size.y) * img->size.x + (xy.x % img->size.x)];
	clr = update_pixel_brightness(light, clr);
	lighting->map->texture[xy.y * lighting->map->size.x + xy.x] = clr;
}

void	parse_entity_texture(int index, t_entity *entity)
{
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
			sample_pixel(xy, &lighting);
			xy.x++;
		}
		xy.y++;
	}
	if (entity->map[index].lightmap)
	{
		free(entity->map[index].lightmap);
		entity->map[index].lightmap = NULL;
	}
}

void	*calculate_texture_for_entity(t_entity *entity)
{
	int			index;
	int			start;
	int			i;

	if (entity->obj->uv_count == 0 || entity->world_triangles == NULL)
		return (NULL);
	i = 0;
	start = 0;
	while (i < entity->obj->face_count)
	{
		index = entity->obj->faces[i].material_index;
		if (i + 1 == entity->obj->face_count
			|| index != entity->obj->faces[i + 1].material_index)
			parse_entity_texture(index, entity);
		i++;
	}
	return (NULL);
}

void	calculate_texture_for_entities(t_world *world)
{
	int				i;
	int				found;
	t_entity		*ent;
	t_thread		thread;

	thread.func = (void *)calculate_texture_for_entity;
	thread.struct_size = sizeof(t_entity);
	thread.structs = prot_memalloc(sizeof(t_entity) * THREAD);
	thread.count = 0;
	ft_bzero(thread.structs, sizeof(t_entity) * THREAD);
	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount
		&& i < world->entitycache.alloc_count)
	{
		ent = &world->entitycache.entities[i++];
		if (ent->status != es_free && ent->map != NULL && \
		!ent->dynamic_lit)
		{
			set_lighting_texture_struct(&thread, ent);
			found++;
		}
	}
	multi_thread_start(&thread);
	free(thread.structs);
}
