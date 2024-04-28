/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_addnode.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 10:58:19 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 14:27:56 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"

typedef struct s_nodeline
{
	t_line	line;
	int		i;
}	t_nodeline;

t_line	linebetween(int i, t_area *room)
{
	int		next_i;
	t_line	line;

	if (i == room->edge_count - 1)
		next_i = 0;
	else
		next_i = i + 1;
	line.start = room->edges[i];
	line.end = room->edges[next_i];
	return (line);
}

static void	draw_node_line(t_vector3 cursor, t_line l, t_sdlcontext *sdl)
{
	t_vector3	start;
	t_vector3	end;

	start = v2tov3(l.start);
	end = v2tov3(l.end);
	start.z = cursor.z;
	end.z = cursor.z;
	render_ray3d(sdl, start, cursor, CLR_GREEN);
	render_ray3d(sdl, end, cursor, CLR_GREEN);
}

//TODO: make sure this is actually safe, spooky memcpy stuff
void	addnode(t_vector2 newnode, t_area *room, int node_i)
{
	int			cpy_before;
	int			cpy_after;
	t_vector2	new_edges[32];

	ft_bzero(new_edges, sizeof(new_edges));
	cpy_before = ft_clamp(node_i + 1, 0, 32);
	cpy_after = ft_clamp(room->edge_count - node_i - 1, 0, 32);
	ft_memcpy(new_edges, room->edges, sizeof(t_vector2) * cpy_before);
	new_edges[node_i + 1] = newnode;
	ft_memcpy(new_edges + node_i + 2,
		room->edges + node_i + 1, sizeof(t_vector2) * cpy_after);
	ft_memcpy(room->edges, new_edges, sizeof(t_vector2 [32]));
	room->edge_count++;
}

bool	potential_node(t_vector3 cursor, t_room_tool_data *dat, t_editor *ed)
{
	int		i;
	t_line	l;

	i = 0;
	while (i < dat->room->edge_count)
	{
		l = linebetween(i, dat->room);
		if (collision_line_circle(l, v3tov2(cursor), 10.0f))
		{
			draw_node_line(cursor, l, ed->world.sdl);
			draw_node_indicator(cursor, dat, ed);
			if (check_alpha_key(ed->hid.alphakey_pressed, 'e')
				&& dat->room->edge_count < 31)
			{
				addnode(v3tov2(cursor), dat->room, i);
				room_init(dat->room, &ed->world);
			}
			return (true);
		}
		i++;
	}
	return (false);
}
