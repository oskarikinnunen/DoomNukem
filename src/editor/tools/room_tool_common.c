/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_common.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 08:53:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/21 15:22:46 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

t_vector2   vector2_flipxy(t_vector2 vec)
{
	return ((t_vector2) {-vec.y, vec.x});
}

t_vector2 prev_edge(t_room *room, int i)
{
	int	ni;

	ni = i - 1;
	if (i == 0)
		ni = room->edgecount - 1;
	return (room->edges[ni]);
}

t_vector2 next_edge(t_room *room, int i)
{
	int	ni;

	ni = i + 1;
	if (i == room->edgecount - 1)
		ni = 0;
	return (room->edges[ni]);
}

bool	isconnect(t_vector2 v2, t_room *room)
{
	int		i;
	t_wall	w;

	i = 0;
	while (i < room->wallcount)
	{
		w = room->walls[i];
		if (w.connection && (vector2_cmp(v2, *w.edgeline.start) || vector2_cmp(v2, *w.edgeline.end)))
			return (true);
		i++;
	}
	return (false);
}

void highlight_roomborders(t_editor *ed, t_sdlcontext *sdl, t_room *room)
{
	int			i;
	int			edgecount;
	t_vector2	prev;
	t_vector2	cur;
	t_vector2	next;
	t_vector2	norm;
	t_vector3	edges[32];

	i = 0;
	edgecount = 0;
	while (i < room->edgecount)
	{
		prev = prev_edge(room, i);
		cur	= room->edges[i];
		next = next_edge(room, i);
		norm = vector2_lerp(vector2_normalise(vector2_flipxy(vector2_sub(next, cur))),
			vector2_normalise(vector2_flipxy(vector2_sub(cur, prev))), 0.5f);
		norm = vector2_normalise(norm);
		norm = vector2_mul(norm, 20.0f);
		norm = vector2_add(cur, norm);
		if (!isconnect(cur, room))
		{
			edges[edgecount] = vector2_to_vector3(norm);
			edges[edgecount++].z += room->height;
		}
			
		i++;
	}
	i = 0;
	ed->render.gizmocolor = CLR_RED;
	while (i < edgecount)
	{
		if (i == edgecount - 1)
		{
			//render_ray(*sdl, ed->render, vector2_to_vector3(room->edges[i]), vector2_to_vector3(room->edges[0]));

			render_ray(*sdl, ed->render, edges[i], edges[0]);
		}
		else
		{
			//render_ray(*sdl, ed->render, vector2_to_vector3(room->edges[i]), vector2_to_vector3(room->edges[i + 1]));
			render_ray(*sdl, ed->render, edges[i], edges[i + 1]);
		}
			
		i++;
	}
}

void highlight_entity(t_editor *ed, t_sdlcontext sdl, t_entity *entity, uint32_t color)
{
	ed->render.wireframe = true;
	ed->render.gizmocolor = color;
	render_entity(sdl, &ed->render, entity);
	ed->render.wireframe = false;
}

void highlight_room_edgelines(t_editor *ed, t_sdlcontext *sdl, t_room *room)
{
	int	i;

	i = 0;
	//while (i < )
}

void highlight_room(t_editor *ed, t_sdlcontext sdl, t_room room, uint32_t color)
{
	int	i;

	i = 0;
	ed->render.wireframe = true;
	ed->render.gizmocolor = color;
	while (i < room.wallcount)
	{
		//render_entity(sdl, &ed->render, room.walls[i].entity);
		if (room.walls[i].edgeline.end != NULL && room.walls[i].edgeline.start != NULL)
		{
			//render ray from edgeline start to end
			ed->render.gizmocolor = CLR_BLUE;
			render_ray(sdl, ed->render,
				vector3_add(vector2_to_vector3(*room.walls[i].edgeline.start),(t_vector3){.z = room.height}),
				vector3_add(vector2_to_vector3(*room.walls[i].edgeline.end),(t_vector3){.z = room.height}));
			ed->render.gizmocolor = color;
		}
		else
		{
			ed->render.gizmocolor = CLR_GREEN;
			render_ray(sdl, ed->render,
				vector2_to_vector3(*room.walls[i].edgeline.start),
				vector2_to_vector3(*room.walls[i].edgeline.end));
			ed->render.gizmocolor = color;
		}
		i++;
	}
	i = 0;
	while (i < room.floorcount)
	{
		//render_entity(sdl, &ed->render, room.floors[i].entity);
		i++;
	}
	ed->render.wireframe = false;
	highlight_roomborders(ed, &sdl, &room);
}

void highlight_roomwalls(t_editor *ed, t_sdlcontext sdl, t_room room, uint32_t color)
{
	int	i;

	i = 0;
	ed->render.wireframe = true;
	ed->render.gizmocolor = color;
	while (i < room.wallcount)
	{
		//printf("HIGHLIGHTING ROOM \n");
		render_entity(sdl, &ed->render, room.walls[i].entity);
		i++;
	}
	ed->render.wireframe = false;
}


t_meshtri	*selectedfloor(t_editor *ed, t_sdlcontext sdl, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->floorcount)
	{
		if (entity_lookedat(ed, sdl, room->floors[i].entity))
			return (&room->floors[i]);
		i++;
	}
	return (NULL);
}

t_wall	*selectedwall(t_editor *ed, t_sdlcontext sdl, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		if (entity_lookedat(ed, sdl, room->walls[i].entity))
			return (&room->walls[i]);
		i++;
	}
	return (NULL);
}

t_room *selectedroom(t_editor *ed, t_sdlcontext sdl)
{
	t_list	*l;
	t_room	*r;

	l = ed->world.roomlist;
	while (l != NULL)
	{
		r = (t_room *)l->content;
		if (selectedwall(ed, sdl, r) != NULL)
			return (r);
		/*if (selectedfloor(ed, sdl, r) != NULL)
			return (r);*/
		l = l->next;
	}
	return (NULL);
}