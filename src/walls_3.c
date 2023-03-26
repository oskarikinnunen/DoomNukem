/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:20:18 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:01:25 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void	wall_assign_edges(t_area *room, int i)
{
	room->walls[i].edgeline.start = &room->edges[i];
	room->walls[i].edgeline.start_index = i;
	if (i != room->edgecount - 1)
	{
		room->walls[i].edgeline.end = &room->edges[i + 1];
		room->walls[i].edgeline.end_index = i + 1;
	}
	else
	{
		room->walls[i].edgeline.end = &room->edges[0];
		room->walls[i].edgeline.end_index = 0;
	}
}

void	_room_initwalls_shallow(t_world *world, t_area *room)
{
	int			i;
	t_entity	*ent;
	t_object	*obj;

	i = 0;
	room->wallcount = room->edgecount;
	if (!room->floor_enabled && !room->loop)
		room->wallcount--;
	while (i < room->wallcount)
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
	room->wallcount = room->edgecount;
	if (!room->floor_enabled && !room->loop)
		room->wallcount--;
	while (i < room->wallcount)
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
		applywallmesh(&room->walls[i], room, world);
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
		room->wallcount = 0;
		room->ceiling_height = 0;
	}
	if (room->floor_enabled)
	{
		room_make_floor(world, room);
		if (room->ceiling_enabled)
			room_makeceilings(world, room);
	}
	doomlog_mul(LOG_NORMAL, (char *[3]){\
		"initialized area:", room->name, NULL});
}