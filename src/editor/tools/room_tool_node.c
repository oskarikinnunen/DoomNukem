/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 11:37:18 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:36:42 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"

/*
	This is called from room_tool_modifymode and is used to 
	modify existing nodes in an area, or remove them
*/
//TODO: protect edge_count--?
void	remove_edge(t_world *world, t_area *room, t_vector2 *edge)
{
	t_list		*l;
	t_area		*cur;
	t_vector2	orig;
	int			i;

	l = world->arealist;
	orig = *edge;
	while (l != NULL)
	{
		cur = l->content;
		i = _room_find_edge_index(orig, cur);
		if (i != -1)
		{
			while (i < cur->edge_count)
			{
				cur->edges[i] = cur->edges[next_index(i, cur)];
				i++;
			}
			cur->edge_count--;
			room_remove_entities(cur, world);
			room_init(cur, world);
			room_recalculate_joined_rooms(world, cur);
		}
		l = l->next;
	}
}

void	recalculate_rooms_with_edge(t_editor *ed, t_vector2 edge)
{
	t_list	*l;
	t_area	*r;
	int		i;

	l = ed->world.arealist;
	while (l != NULL)
	{
		r = l->content;
		if (_room_find_edge(edge, r) != NULL)
		{
			room_init(r, &ed->world);
		}
		l = l->next;
	}
}

static bool	edge_is_legal_in_all_areas_that_have_it(t_vector2 *edge,
		t_vector2 snap, t_world *world)
{
	t_vector2	temp;
	t_vector2	*test;
	t_area		*cur;
	t_list		*l;
	bool		legal;

	legal = true;
	l = world->arealist;
	while (l != NULL)
	{
		cur = l->content;
		if (_room_find_edge(*edge, cur) != NULL)
		{
			test = _room_find_edge(*edge, cur);
			temp = *test;
			*test = snap;
			if (!room_edge_is_legal(test, cur) || !room_is_legal(world, cur))
				legal = false;
			*test = temp;
		}
		l = l->next;
	}
	return (legal);
}

void	apply_edge_drag_solo(t_vector2 *edge, t_vector2 snap,
		t_area *room, t_world *world)
{
	int			i;
	t_vector2	orig;
	t_area		temp;

	orig = *edge;
	if (vector2_cmp(*edge, snap))
		return ;
	if (edge_is_legal_in_all_areas_that_have_it(edge, snap, world))
	{
		ft_bzero(&temp, sizeof(t_area));
		ft_memcpy(temp.edges, room->edges, sizeof(t_vector2) * 32);
		temp.edge_count = room->edge_count;
		if (edge_exists(orig, room))
		{
			i = _room_find_edge_index(orig, room);
			room->edges[i] = snap;
		}
	}
}

void	apply_edge_drag(t_vector2 *edge, t_vector2 cursor,
		t_area *room, t_world *world)
{
	int				i;
	t_area			*cur;
	t_list			*l;
	t_vector2		orig;

	orig = *edge;
	if (vector2_cmp(*edge, cursor))
		return ;
	if (edge_is_legal_in_all_areas_that_have_it(edge, cursor, world))
	{
		l = world->arealist;
		while (l != NULL)
		{
			cur = l->content;
			if (edge_exists(orig, cur) && rooms_share_zspace(room, cur))
			{
				i = _room_find_edge_index(orig, cur);
				cur->edges[i] = cursor;
			}
			l = l->next;
		}
	}
}
