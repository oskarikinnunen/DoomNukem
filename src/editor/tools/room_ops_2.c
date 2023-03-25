/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_ops_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 17:54:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 21:33:58 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"
#include "collision.h"

static t_line	edgeline_to_line(t_edgeline edgeline)
{
	return ((t_line){.start = *edgeline.start, .end = *edgeline.end});
}

static bool	illegalwall_move(t_wall *wall, t_area *room)
{
	int		i;
	t_wall	*ow;

	i = 0;
	if (vector2_cmp(*wall->edgeline.start, *wall->edgeline.end))
	{
		return (true);
	}
	while (i < room->wallcount)
	{
		ow = &room->walls[i];
		return (wall != &room->walls[i]
			&& collision_line_line_intersect(
				line_shorten(edgeline_to_line(wall->edgeline)),
				line_shorten(edgeline_to_line(ow->edgeline))
			));
		i++;
	}
	return (false);
}

bool	room_is_legal(t_world *world, t_area *room)
{
	int		i;
	int		ii;
	t_list	*l;
	t_area	*other;

	i = 0;
	if (room->walls_enabled == false)
		return (true);
	while (i < room->wallcount)
	{
		if (illegalwall_move(&room->walls[i], room))
			return (false);
		i++;
	}
	return (true);
}

bool	room_edge_is_legal(t_vector2 *edge, t_area *room)
{
	int			i;
	t_vector2	*other;

	i = 0;
	while (i < room->edgecount)
	{
		other = &room->edges[i];
		if (other != edge && vector2_dist(*edge, *other) < 10.0f)
			return (false);
		i++;
	}
	return (true);
}

void	room_recalculate_joined_rooms(t_world *world, t_area *room)
{
	t_list	*l;
	t_area	*other;
	int		i;

	l = world->arealist;
	while (l != NULL)
	{
		other = l->content;
		if (other != room)
		{
			i = 0;
			while (i < room->edgecount)
			{
				if (edge_exists(room->edges[i], other))
					room_init(other, world);
				i++;
			}
		}
		l = l->next;
	}
}
