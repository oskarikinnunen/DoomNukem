/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_triangulate_4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:00:41 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 22:34:13 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "collision.h"
#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "objects.h"

bool	points_collide(t_floorcalc *fc, t_vector2 tri[3])
{
	int					i;
	t_world_triangle	t;
	t_point				p;

	t.p[0].v = v2tov3(tri[0]);
	t.p[1].v = v2tov3(tri[1]);
	t.p[2].v = v2tov3(tri[2]);
	i = 0;
	while (i < fc->edge_count)
	{
		if (vector2_cmp(fc->edges[i], tri[0])
			|| vector2_cmp(fc->edges[i], tri[1])
			|| vector2_cmp(fc->edges[i], tri[2]))
		{
			i++;
			continue ;
		}
		p = vector2_to_point(fc->edges[i]);
		if (collision_point_triangle(p, t))
			return (true);
		i++;
	}
	return (false);
}

t_line	line_shorten(t_line line)
{
	t_vector2	temp;
	t_vector2	delta_start;
	t_vector2	delta_end;
	t_line		final;
	float		dist;

	dist = vector2_dist(line.start, line.end);
	temp = vector2_sub(line.start, line.end);
	temp = vector2_clamp_magnitude(temp, dist - 1.0f);
	temp = vector2_add(temp, line.end);
	final.start = temp;
	temp = vector2_sub(line.end, line.start);
	temp = vector2_clamp_magnitude(temp, dist - 1.0f);
	temp = vector2_add(temp, line.start);
	final.end = temp;
	return (final);
}

void	free_object(t_object **object)
{
	if (*object == NULL)
		return ;
	if ((*object)->faces != NULL)
		free((*object)->faces);
	if ((*object)->materials != NULL)
		free((*object)->materials);
	if ((*object)->uvs != NULL)
		free((*object)->uvs);
	if ((*object)->vertices != NULL)
		free((*object)->vertices);
	free(*object);
	*object = NULL;
}

void	free_floor(t_world *world, t_area *room)
{
	int	i;

	i = 0;
	while (i < room->floor_count)
	{
		if (room->floors[i].entity != NULL)
		{
			if (room->floors[i].entity->obj != NULL)
			{
				free_object(&room->floors[i].entity->obj);
				room->floors[i].entity->obj = NULL;
			}
			destroy_entity(world, room->floors[i].entity);
			room->floors[i].entity = NULL;
		}
		i++;
	}
}

void	free_ceilings(t_world *world, t_area *room)
{
	int	i;

	i = 0;
	while (i < room->ceiling_count)
	{
		if (room->ceilings[i].entity != NULL)
		{
			if (room->ceilings[i].entity->obj != NULL)
			{
				free_object(&room->ceilings[i].entity->obj);
				room->ceilings[i].entity->obj = NULL;
			}
			destroy_entity(world, room->ceilings[i].entity);
			room->ceilings[i].entity = NULL;
		}
		i++;
	}
}
