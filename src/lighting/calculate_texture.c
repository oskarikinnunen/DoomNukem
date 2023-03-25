/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_texture.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 13:07:06 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/25 13:00:34 by raho             ###   ########.fr       */
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

static void	buffer_triangles(int start, int i, int index, t_entity *entity)
{
	t_screen_triangle	temp;
	int					vertex;
	size_t				size;
	t_lighting			lighting;

	lighting.map = &entity->map[index];
	lighting.img = entity->obj->materials[index].img;
	size = lighting.map->size.x * lighting.map->size.y * sizeof(bool);
	lighting.overdraw = malloc(size);
	if (lighting.overdraw == NULL)
		doomlog(LOG_FATAL, "Malloc failed in calculate_texture.c");
	ft_bzero(lighting.overdraw, size);
	for (int y = 0; y < lighting.map->size.y; y++)
	{
		for (int x = 0; x < lighting.map->size.x; x++)
		{
			sample_pixel((t_point){x, y}, &lighting);
		}
	}
	free(lighting.overdraw);
	return ; // early return;
	if (lighting.overdraw == NULL)
		doomlog(LOG_FATAL, "Malloc failed in calculate_texture.c");
	ft_bzero(lighting.overdraw, size);
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
		rasterize_texture(temp, &lighting);
		start++;
	}
	free(lighting.overdraw);
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
			buffer_triangles(start, i, index, entity);
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
	thread.structs = malloc(sizeof(t_entity) * THREAD);
	thread.count = 0;
	ft_bzero(thread.structs, sizeof(t_entity) * THREAD);
	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount
		&& i < world->entitycache.alloc_count)
	{
		ent = &world->entitycache.entities[i++];
		if (ent->status != es_free && ent->map != NULL)
		{
			set_lighting_texture_struct(&thread, ent);
			found++;
		}
	}
	multithread_start(&thread);
	free(thread.structs);
}
