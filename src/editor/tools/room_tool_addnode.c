/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_addnode.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 10:58:19 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 14:55:00 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"

/*static bool	cursor_in_room_zspace(t_vector3 cursor, t_room *room) {

}*/

/*
	This function is called from modifymode,
	and returns a node between two previous nodes if the player is looking at it
*/
typedef struct s_nodeline
{
	t_line	line;
	int		i;
}	t_nodeline;

t_line	linebetween(int i, t_area *room)
{
	int		next_i;
	t_line	line;

	if (i == room->edgecount - 1)
		next_i = 0;
	else
		next_i = i + 1;
	line.start = room->edges[i];
	line.end = room->edges[next_i];
	return (line);
}

static void draw_node_indicator(t_vector3 cursor, t_roomtooldata *dat, t_world *world)
{
	t_point	center;

	center = point_div(world->sdl->screensize, 2);
	center = point_add(center, (t_point){0, 15});
	render_circle(world->sdl, cursor, 10.0f, CLR_GREEN);
	print_text_boxed(world->sdl, "[E] Add node", center);
}

static void draw_node_line(t_vector3 cursor, t_line l, t_sdlcontext *sdl)
{
	t_vector3	start;
	t_vector3	end;

	start = v2tov3(l.start);
	end = v2tov3(l.end);
	start.z = cursor.z;
	end.z = cursor.z;
	render_ray3D(sdl, start, cursor, CLR_GREEN);
	render_ray3D(sdl, end, cursor, CLR_GREEN);
}

void	addnode(t_vector2 newnode, t_area *room, int node_i)
{
	int	cpy_before;
	int	cpy_after;
	t_vector2	*new_edges; //TODO: replace with t_vector2[32]
	new_edges = ft_memalloc(sizeof(t_vector2) * 32);

	cpy_before = ft_clamp(node_i + 1, 0, 32);
	cpy_after = ft_clamp(room->edgecount - node_i - 1, 0, 32);
	ft_memcpy(new_edges, room->edges, sizeof(t_vector2) * cpy_before);
	new_edges[node_i + 1] = newnode;
	ft_memcpy(new_edges + node_i + 2, room->edges + node_i + 1, sizeof(t_vector2) * cpy_after);
	ft_memcpy(room->edges, new_edges, sizeof(t_vector2 [32]));
	room->edgecount++;
	printf("copied %i edges before new edge \n", node_i - 1);
}


bool	potentialnode(t_vector3 cursor, t_roomtooldata *dat, t_editor *ed)
{
	int		i;
	t_line	l;
	//linecirclecollision()

	i = 0;
	while (i < dat->room->edgecount)
	{
		l = linebetween(i, dat->room);
		if (collision_line_circle(l, v3tov2(cursor), 10.0f))
		{
			draw_node_line(cursor, l, ed->world.sdl);
			draw_node_indicator(cursor, dat, &ed->world);
			if (check_alpha_key(ed->hid.alphakey_pressed, 'e'))
			{
				/*
					get cur index and next index,
					find all rooms that have matching edges (in either order, cur->next or next->cur)
						(and they share zspace with current room)
						addnode(foundroom);
				*/
				addnode(v3tov2(cursor), dat->room, i);
				room_init(dat->room, &ed->world);
				printf("Add node %i!!\n", i);
			}
				
			return (true);
		}
			
		i++;
	}
	return (false);
}