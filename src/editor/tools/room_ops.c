/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 17:45:17 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 19:07:18 by raho             ###   ########.fr       */
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
	t_area	*worldroom;

	ft_strncpy_term(room->name, "area", 30);
	worldroom = prot_memalloc(sizeof(t_area));
	ft_strncpy_term(worldroom->s_floor_tex.str, room->s_floor_tex.str, 60);
	ft_strncpy_term(worldroom->s_ceil_tex.str, room->s_ceil_tex.str, 60);
	ft_memcpy(worldroom->edges, room->edges, sizeof(worldroom->edges));
	worldroom->edge_count = room->edge_count;
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
	free(room);
	return (worldroom);
}

void	world_remove_room(t_world *world, t_area *room)
{
	room_remove_entities(room, world);
	list_remove(&world->arealist, room, sizeof(t_area));
}

void	room_set_preset_floor(t_area *room)
{
	room->floor_enabled = true;
	room->ceiling_enabled = false;
	room->walls_enabled = false;
	room->loop = true;
}

void	room_set_preset_exterior(t_area *room)
{
	room->floor_enabled = false;
	room->ceiling_enabled = false;
	room->walls_enabled = true;
}

void	room_set_preset_room(t_area *room)
{
	room->floor_enabled = true;
	room->ceiling_enabled = true;
	room->walls_enabled = true;
	room->loop = true;
}
