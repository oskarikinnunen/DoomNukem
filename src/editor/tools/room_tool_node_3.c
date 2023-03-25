/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_node_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 21:47:16 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 16:30:15 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"

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
		if (vector3_sqr_dist(rc, edge_3d) < 1000.0f
			&& vector3_sqr_dist(rc, edge_3d) < prev_dist)
		{
			looked = edge;
			prev_dist = vector3_sqr_dist(rc, edge_3d);
		}
		i++;
	}
	return (looked);
}

void	room_tool_node(t_vector3 cursor, t_roomtooldata *dat, t_editor *ed)
{
	static bool	recalc;

	dat->held_edge = closest_edge(dat->room, cursor);
	if (dat->held_edge == NULL)
		return ;
	draw_node_indicator(cursor, dat, ed);
	if (ed->hid.mouse.held == MOUSE_LEFT)
	{
		if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
			applyedgedrag_solo(dat->held_edge, v3tov2(cursor),
				dat->room, &ed->world);
		else
			applyedgedrag(dat->held_edge, v3tov2(cursor),
				dat->room, &ed->world);
		recalc = true;
	}
	if (ed->hid.mouse.held == 0 && recalc)
	{
		recalculate_rooms_with_edge(ed, *dat->held_edge);
		recalc = false;
	}
	if (check_alpha_key(ed->hid.alphakey_pressed, 'X')
		&& dat->room->edgecount > 2)
		remove_edge(&ed->world, dat->room, dat->held_edge);
}
