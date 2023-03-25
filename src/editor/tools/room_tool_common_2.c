/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_common_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 19:45:54 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 19:46:07 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "tools/roomtool.h"

bool	rooms_share_zspace(t_area *room1, t_area *room2)
{
	bool	share;

	share = false;
	if (room1->height <= (room2->height + room2->ceiling_height)
		&& room1->height >= room2->height)
		share = true;
	if (room2->height <= (room1->height + room1->ceiling_height)
		&& room2->height >= room1->height)
		share = true;
	return (share);
}

void	room_tool_remove_room(t_editor *ed, t_roomtooldata *dat)
{
	t_area	temp;

	ft_bzero(&temp, sizeof(t_area));
	ft_memcpy(temp.edges, dat->room->edges,
		sizeof(t_vector2) * dat->room->edgecount);
	temp.edgecount = dat->room->edgecount;
	world_remove_room(&ed->world, dat->room);
	room_recalculate_joined_rooms(&ed->world, &temp);
	dat->room = NULL;
	dat->rtm = rtm_none;
}

t_vector2	next_edge(t_area *room, int i)
{
	int	ni;

	ni = i + 1;
	if (room->edgecount != 0 && i == room->edgecount - 1)
		ni = 0;
	return (room->edges[ni]);
}

bool	isconnect(t_vector2 v2, t_area *room)
{
	int		i;
	t_wall	w;

	i = 0;
	while (i < room->wallcount)
	{
		w = room->walls[i];
		if (w.connection && (vector2_cmp(v2, *w.edgeline.start)
				|| vector2_cmp(v2, *w.edgeline.end)))
			return (true);
		i++;
	}
	return (false);
}

bool	edge_exists(t_vector2 edge, t_area	*room)
{
	int	i;

	i = 0;
	while (i < room->edgecount)
	{
		if (vector2_cmp(edge, room->edges[i]))
			return (true);
		i++;
	}
	return (false);
}
