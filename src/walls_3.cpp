/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:20:18 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 18:52:07 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void	wall_assign_edges(t_area *room, int i)
{
	room->walls[i].edge_line.start = &room->edges[i];
	room->walls[i].edge_line.start_index = i;
	if (i != room->edge_count - 1)
	{
		room->walls[i].edge_line.end = &room->edges[i + 1];
		room->walls[i].edge_line.end_index = i + 1;
	}
	else
	{
		room->walls[i].edge_line.end = &room->edges[0];
		room->walls[i].edge_line.end_index = 0;
	}
}

void	_room_initwalls_shallow(t_world *world, t_area *room)
{
	int			i;
	t_entity	*ent;
	t_object	*obj;

	i = 0;
	room->wall_count = room->edge_count;
	if (!room->floor_enabled && !room->loop)
		room->wall_count--;
	while (i < room->wall_count)
	{
		wall_assign_edges(room, i);
		if (room->walls[i].entity == NULL)
		{
			obj = object_plane(world->sdl);
			if (strlen(room->walls[i].s_walltex.str) != 0)
				obj->materials->img = get_image_by_name(
						*world->sdl, room->walls[i].s_walltex.str);
			room->walls[i].entity = spawn_entity(world, obj);
			room->walls[i].entity->rigid = true;
		}
		room->walls[i].height = room->ceiling_height;
		i++;
	}
}

void	_room_initwalls(t_world *world, t_area *room)
{
	int			i;
	t_entity	*ent;
	t_object	*obj;

	i = 0;
	room->wall_count = room->edge_count;
	if (!room->floor_enabled && !room->loop)
		room->wall_count--;
	while (i < room->wall_count)
	{
		wall_assign_edges(room, i);
		if (room->walls[i].entity == NULL)
		{
			obj = object_plane(world->sdl);
			if (ft_strlen(room->walls[i].s_walltex.str) != 0)
				obj->materials->img = get_image_by_name(
						*world->sdl, room->walls[i].s_walltex.str);
			room->walls[i].entity = spawn_entity(world, obj);
			room->walls[i].entity->rigid = true;
		}
		room->walls[i].height = room->ceiling_height;
		apply_wallmesh(&room->walls[i], room, world);
		i++;
	}
}

void	room_remove_entities(t_area *room, t_world *world)
{
	free_roomwalls(world, room);
	free_floor(world, room);
	free_ceilings(world, room);
}

void	room_init(t_area *room, t_world *world)
{
	room_remove_entities(room, world);
	if (room->walls_enabled)
		_room_initwalls(world, room);
	else
	{
		room->wall_count = 0;
		room->ceiling_height = 0;
	}
	if (room->floor_enabled && room->edge_count > 2)
	{
		room_make_floor(world, room);
		if (room->ceiling_enabled)
			room_make_ceilings(world, room);
	}
	doomlog_mul(LOG_NORMAL, (char *[3]){\
		"initialized area:", room->name, NULL});
}
