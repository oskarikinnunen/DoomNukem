/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_node2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 21:45:46 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 21:46:34 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"

void	draw_node_indicator(t_vector3 cursor,
		t_roomtooldata *dat, t_editor *ed)
{
	t_vector3		nodepos;
	t_point			center;
	t_sdlcontext	*sdl;

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
}

t_vector2	*_room_find_edge(t_vector2 edge, t_area *room)
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

int	_room_find_edge_index(t_vector2 edge, t_area *room)
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

int	next_index(int cur, t_area *room)
{
	if (cur == room->edgecount - 1)
		return (0);
	return (cur + 1);
}
