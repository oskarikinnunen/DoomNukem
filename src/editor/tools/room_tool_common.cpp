/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_common.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 08:53:20 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 20:27:18 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "tools/roomtool.h"

void	toggle_ceilings(t_world *world)
{
	t_list	*l;
	t_area	*r;
	int		i;

	l = world->arealist;
	while (l != NULL)
	{
		r = l->content;
		i = 0;
		while (i < r->wall_count)
		{
			if (r->walls[i].ceiling_wall)
				r->walls[i].entity->hidden = world->ceiling_toggle;
			i++;
		}
		l = l->next;
	}
}

void	highlight_entity(t_sdlcontext *sdl, t_entity *entity, uint32_t color)
{
	uint32_t	temp;

	sdl->render.wireframe = true;
	temp = sdl->render.gizmocolor;
	sdl->render.gizmocolor = color;
	render_entity(sdl, &sdl->render, entity);
	sdl->render.wireframe = false;
	sdl->render.gizmocolor = temp;
}

bool	is_joined(t_vector2 edge, t_area	*room, t_world *world)
{
	t_list	*l;
	t_area	*other;

	l = world->arealist;
	while (l != NULL)
	{
		other = l->content;
		if (other != room && edge_exists(edge, other)
			&& rooms_share_zspace(room, other))
			return (true);
		l = l->next;
	}
	return (false);
}

static void	highlight_area_walls(t_editor *ed, t_sdlcontext *sdl,
		t_area *area, uint32_t color)
{
	int			i;
	t_vector3	ws;
	t_vector3	ws2;

	i = 0;
	while (i < area->wall_count)
	{
		if (area->walls[i].edge_line.start != NULL
			&& area->walls[i].edge_line.end != NULL)
		{
			ws = v2tov3(*area->walls[i].edge_line.start);
			ws2 = v2tov3(*area->walls[i].edge_line.start);
			ws.z = area->height;
			ws2.z = area->height + area->walls[i].height;
			render_ray3d(sdl, ws, ws2, color);
		}
		i++;
	}
}

void	highlight_room(t_editor *ed, t_sdlcontext *sdl,
		t_area *room, uint32_t color)
{
	int			i;
	t_vector3	ws;
	t_vector3	ws2;
	t_vector2	start;

	i = 0;
	while (i < room->edge_count)
	{
		start = room->edges[i];
		ws = (t_vector3){start.x, start.y, room->height};
		render_gizmo3d(sdl, ws, 2, color);
		sdl->render.gizmocolor = color;
		ws2 = v2tov3(next_edge(room, i));
		ws2.z = room->height;
		if (!room->loop && i == room->edge_count - 1)
			;
		else
			render_ray3d(sdl, ws, ws2, color);
		i++;
	}
	highlight_area_walls(ed, sdl, room, color);
}
