/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 05:31:47 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:01:29 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "doomnukem.h"
#include "editor_tools.h"

void	free_walls(t_area *room, t_world *world)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		if (room->walls[i].entity->obj != NULL)
		{
			free_object(room->walls[i].entity->obj);
			destroy_entity(world, room->walls[i].entity);
			room->walls[i].entity = NULL;
		}
		i++;
	}
}

t_wall	*find_wall(t_wall wall, t_area *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		if (room->walls[i].edgeline.start == NULL
			|| room->walls[i].edgeline.end == NULL
			|| wall.edgeline.start == NULL || wall.edgeline.end == NULL)
		{
			i++;
			continue ;
		}
		if ((vector2_cmp(*room->walls[i].edgeline.start, *wall.edgeline.start) \
		&& vector2_cmp(*room->walls[i].edgeline.end, *wall.edgeline.end))
			|| (vector2_cmp(*room->walls[i].edgeline.start, *wall.edgeline.end) \
		&& vector2_cmp(*room->walls[i].edgeline.end, *wall.edgeline.start)))
			return (&room->walls[i]);
		i++;
	}
	return (NULL);
}

void	room_init_shallow(t_area *room, t_world *world)
{
	room_remove_entities(room, world);
	if (room->walls_enabled)
		_room_initwalls_shallow(world, room);
	if (room->floor_enabled)
	{
		room_make_floor(world, room);
		if (room->ceiling_enabled)
			room_makeceilings(world, room);
	}
}

void	applytrimesh(t_meshtri tri, t_object *obj)
{
	obj->vertices[0] = tri.v[0];
	obj->vertices[1] = tri.v[1];
	obj->vertices[2] = tri.v[2];
	obj->uvs[0] = tri.uv[0];
	obj->uvs[1] = tri.uv[1];
	obj->uvs[2] = tri.uv[2];
}
