/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 11:37:18 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/17 18:56:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"

/*
	This is called from room_tool_modifymode and is used to 
	modify existing nodes in an area, or remove them
*/

t_vector2	*closest_edge(t_area *room, t_vector3 rc)
{
	int			i;
	t_vector2	*edge;
	t_vector3	edge_3d;
	float		prev_dist;
	t_vector2	*looked;

	i = 0;
	looked = NULL;
	prev_dist = 1001.0f;
	while (i < room->edgecount)
	{
		edge = &room->edges[i];
		edge_3d = (t_vector3){edge->x, edge->y, room->height};
		if (vector3_sqr_dist(rc, edge_3d) < 500.0f && vector3_sqr_dist(rc, edge_3d) < prev_dist)
		{
			looked = edge;
			prev_dist = vector3_sqr_dist(rc, edge_3d);
		}
		i++;
	}
	return (looked);
}

static void draw_node_indicator(t_vector3 cursor, t_roomtooldata *dat, t_editor *ed)
{
	t_vector3	nodepos;
	t_point		center;
	t_sdlcontext *sdl;

	sdl = ed->world.sdl;
	nodepos = v2tov3(*dat->held_edge);
	nodepos.z = cursor.z;
	render_circle(sdl, nodepos, 10.0f, CLR_BLUE);
	center = point_div(sdl->screensize, 2);
	center = point_add(center, (t_point){0, 15});
	if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
		print_text_boxed(sdl, "[Shift + Left mouse] Move & Detach", center);
	else
		print_text_boxed(sdl, "[Left mouse] Move", center);
	print_text_boxed(sdl, "[X] Remove", point_add(center, (t_point){0, 15}));
	//print_text_boxed(world->sdl, "[X] Remove", point_add(center, (t_point){0, 10}));
}

static t_vector2	*_room_find_edge(t_vector2 edge, t_area *room)
{
	int	i;

	i = 0;
	while (i < room->edgecount)
	{
		if (vector2_cmp(edge, room->edges[i]))
			return (&room->edges[i]);
		i++;
	}
	return (NULL);
}

static int	_room_find_edge_index(t_vector2 edge, t_area *room)
{
	int	i;

	i = 0;
	while (i < room->edgecount)
	{
		if (vector2_cmp(edge, room->edges[i]))
			return (i);
		i++;
	}
	return (-1);
}

static int next_index(int cur, t_area *room)
{
	if (cur == room->edgecount - 1)
		return (0);
	return (cur + 1);
}

void	remove_edge(t_world *world, t_area *room, t_vector2 *edge)
{
	t_list	*l;
	t_area	*cur;
	t_vector2	orig;

	l = world->arealist;
	orig = *edge;
	while (l != NULL)
	{
		cur = l->content;
		int match = _room_find_edge_index(orig, cur);
		int	i = match;
		if (i != -1)
		{
			while (i < cur->edgecount)
			{
				cur->edges[i] = cur->edges[next_index(i, cur)];
				i++;
			}
			cur->edgecount--; //TODO: protect
			room_remove_entities(cur, world);
			/*i = match;
			while (i < cur->edgecount)
			{
				ft_memcpy(cur->walls[next_index(i,cur)].texname, cur->walls[i].texname, sizeof(char [256]));
				cur->walls[i].edgeline.start = &cur->edges[i];
				cur->walls[i].edgeline.end = &cur->edges[next_index(i, cur)];
				i++;
			}
			free_object(cur->walls[cur->wallcount - 1].entity->obj);
			destroy_entity(world, cur->walls[cur->wallcount - 1].entity);
			cur->wallcount--;*/
			room_init(cur, world);
			//make_areas(world, cur);
			recalculate_joined_rooms(world, cur);
		}
		l = l->next;
	}
}


static void	recalculate_rooms_with_edge(t_editor *ed, t_vector2 edge)
{
	t_list	*l;
	t_area	*r;
	int		i;

	l = ed->world.arealist;
	while (l != NULL)
	{
		r = l->content;
		if (_room_find_edge(edge, r) != NULL) //TODO: and z share
		{
			room_init(r, &ed->world);
			//make_areas(&ed->world, r);
		}
		l = l->next;
	}
}

void	applyedgedrag_solo(t_vector2 *edge, t_vector2 snap, t_area *room, t_world *world)
{
	int				i;
	t_vector2		temp;
	t_vector2		*test;
	t_area			*cur;
	t_list			*l;
	bool			legal;
	t_vector2		orig;

	orig = *edge;
	if (vector2_cmp(*edge, snap))
		return ;
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
			if (!edge_is_legal(test, cur) || !isroomlegal(world, cur))
				legal = false;
			*test = temp;
		}
		l = l->next;
	}
	if (legal)
	{
		t_area	temp;
		ft_bzero(&temp, sizeof(t_area));
		ft_memcpy(temp.edges, room->edges, sizeof(t_vector2) * 32);
		temp.edgecount = room->edgecount;
		if (edge_exists(orig, room))
		{
			i = _room_find_edge_index(orig, room);
			room->edges[i] = snap;
		}
	}
}

void	applyedgedrag(t_vector2 *edge, t_vector2 cursor, t_area *room, t_world *world)
{
	int				i;
	t_vector2		temp;
	t_vector2		*test;
	t_area			*cur;
	t_list			*l;
	bool			legal;
	t_vector2		orig;

	orig = *edge;
	if (vector2_cmp(*edge, cursor))
		return ;
	legal = true;
	l = world->arealist;
	while (l != NULL)
	{
		cur = l->content;
		if (edge_exists(*edge, cur) && rooms_share_zspace(room, cur))
		{
			test = _room_find_edge(*edge, cur);
			temp = *test;
			*test = cursor;
			if (!edge_is_legal(test, cur) || !isroomlegal(world, cur))
				legal = false;
			*test = temp;
		}
		l = l->next;
	}

	if (legal)
	{
		l = world->arealist;
		while (l != NULL)
		{
			cur = l->content;
			if (edge_exists(orig, cur) && rooms_share_zspace(room, cur))
			{
				printf("exists in room %s \n", cur->name);
				i = _room_find_edge_index(orig, cur);
				cur->edges[i] = cursor;
				//room_init(cur, world);
			}
			l = l->next;
		}
	}
}


void	room_tool_node(t_vector3 cursor, t_roomtooldata *dat, t_editor *ed)
{
	static bool	recalc;
	//data->held_edge
	dat->held_edge = closest_edge(dat->room, cursor);
	if (dat->held_edge != NULL)
	{
		draw_node_indicator(cursor, dat, ed);
		if (ed->hid.mouse.held == MOUSE_LEFT)
		{
			if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
				applyedgedrag_solo(dat->held_edge, v3tov2(cursor), dat->room, &ed->world);
			else
				applyedgedrag(dat->held_edge, v3tov2(cursor), dat->room, &ed->world);
			recalc = true;
		}
		if (ed->hid.mouse.held == 0 && recalc)
		{
			recalculate_rooms_with_edge(ed, *dat->held_edge);
			recalc = false;
		}
		if (check_alpha_key(ed->hid.alphakey_pressed, 'X'))
		{
			remove_edge(&ed->world, dat->room, dat->held_edge);
		}
	}
}