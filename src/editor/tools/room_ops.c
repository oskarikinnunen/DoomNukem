/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 17:45:17 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 17:47:15 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

//TODO: room_free
t_area	*world_add_room(t_world *world, t_area *room)
{
	int		i;
	char	roomname[64];
	t_area	*worldroom;

	ft_bzero(roomname, 64);
	snprintf(roomname, 64, "area(%i)", ft_listlen(world->arealist));
	ft_strncpy_term(room->name, roomname, 30);
	worldroom = ft_memalloc(sizeof(t_area));
	ft_strncpy_term(worldroom->s_floortex.str, room->s_floortex.str, 60);
	ft_strncpy_term(worldroom->s_ceiltex.str, room->s_ceiltex.str, 60);
	ft_memcpy(worldroom->edges, room->edges, sizeof(worldroom->edges));
	worldroom->edgecount = room->edgecount;
	worldroom->floor_enabled = room->floor_enabled;
	worldroom->walls_enabled = room->walls_enabled;
	worldroom->ceiling_enabled = room->ceiling_enabled;
	worldroom->loop = room->loop;
	if (room->ceiling_height == 0)
		worldroom->ceiling_height = 100;
	ft_strncpy_term(worldroom->name, room->name, 30);
	worldroom->height = room->height;
	list_push(&world->arealist, worldroom, sizeof(t_area));
	free(worldroom);
	worldroom = list_findlast(world->arealist);
	return (worldroom);
}

void	world_remove_room(t_world *world, t_area *room)
{
	room_remove_entities(room, world);
	list_remove(&world->arealist, room, sizeof(t_area));
}

void	room_setpreset_floor(t_area *room)
{
	room->floor_enabled = true;
	room->ceiling_enabled = false;
	room->walls_enabled = false;
	room->loop = true;
}

void	room_setpreset_exterior(t_area *room)
{
	room->floor_enabled = false;
	room->ceiling_enabled = false;
	room->walls_enabled = true;
}

void	room_setpreset_room(t_area *room)
{
	room->floor_enabled = true;
	room->ceiling_enabled = true;
	room->walls_enabled = true;
	room->loop = true;
}
