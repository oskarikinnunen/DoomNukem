/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_common.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 08:53:20 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 14:38:24 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

t_vector2   vector2_flipxy(t_vector2 vec)
{
	return ((t_vector2) {-vec.y, vec.x});
}

t_vector2 prev_edge(t_area *room, int i)
{
	int	ni;

	ni = i - 1;
	if (i == 0)
		ni = room->edgecount - 1;
	return (room->edges[ni]);
}

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
		while (i < r->wallcount)
		{
			if (r->walls[i].ceilingwall)
			{
				r->walls[i].entity->hidden = world->ceiling_toggle;
			}
				
			i++;
		}
		l = l->next;
	}
}

bool	rooms_share_zspace(t_area *room1, t_area *room2)
{
	bool	share = false;
	if (room1->height <= (room2->height + room2->ceiling_height)
		&& room1->height >= room2->height)
		share = true;
	if (room2->height <= (room1->height + room1->ceiling_height)
		&& room2->height >= room1->height)
		share = true;
	return (share);
}

t_vector2 next_edge(t_area *room, int i)
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
		if (w.connection && (vector2_cmp(v2, *w.edgeline.start) || vector2_cmp(v2, *w.edgeline.end)))
			return (true);
		i++;
	}
	return (false);
}

void highlight_roomborders(t_editor *ed, t_sdlcontext *sdl, t_area *room)
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
			edges[edgecount] = v2tov3(norm);
			edges[edgecount++].z += room->height;
		}
			
		i++;
	}
	i = 0;
}

bool	edge_exists(t_vector2 edge, t_area	*room)
{
	int	i;

	i = 0;
	while (i < room->edgecount)
	{
		if (vector2_cmp(edge,
			room->edges[i]))
			return (true);
		i++;
	}
	return (false);
}

void highlight_entity(t_sdlcontext *sdl, t_entity *entity, uint32_t color)
{
	uint32_t	temp;

	sdl->render.wireframe = true;
	temp = sdl->render.gizmocolor;
	sdl->render.gizmocolor = color;
	render_entity(sdl, &sdl->render, entity);
	sdl->render.wireframe = false;
	sdl->render.gizmocolor = temp;
}

void rendergrid(t_world *world, t_vector3 position, int size, uint32_t color)
{
	t_point		p;
	t_vector3	from;
	t_vector3	to;
	t_vector3	snap = vector3_snap(position, 10);

	snap = vector3_sub(snap, (t_vector3){(size * 10) / 2, (size * 10) /2, 0.0f});
	p.y = 0;
	from = snap;
	world->sdl->render.gizmocolor = color;
	while (p.y <= size)
	{
		from = vector3_add(snap, (t_vector3){.y = p.y * 10.0f});
		to = vector3_add(from, (t_vector3){.x = size * 10});
		render_ray(world->sdl, from, to);
		p.y++;
	}
	p.x = 0;
	while (p.x <= size)
	{
		from = vector3_add(snap, (t_vector3){.x = p.x * 10.0f});
		to = vector3_add(from, (t_vector3){.y = size * 10});
		render_ray(world->sdl, from, to);
		p.x++;
	}
}

bool	is_joined(t_vector2 edge, t_area	*room, t_world *world)
{
	t_list	*l;
	t_area	*other;
	l = world->arealist;
	while (l != NULL)
	{
		other = l->content;
		if (other != room && edge_exists(edge, other) && rooms_share_zspace(room, other))
			return (true);
		l = l->next;
	}
	return (false);
}

static void	highlight_area_walls(t_editor *ed, t_sdlcontext *sdl, t_area *area, uint32_t color)
{
	int	i;
	t_vector3	ws;
	t_vector3	ws2;

	i = 0;
	while (i < area->wallcount)
	{
		if (area->walls[i].edgeline.start != NULL && area->walls[i].edgeline.end != NULL)
		{
			ws = v2tov3(*area->walls[i].edgeline.start);
			ws2 = v2tov3(*area->walls[i].edgeline.start);
			ws.z = area->height;
			ws2.z = area->height + area->walls[i].height;
			render_ray3D(sdl, ws, ws2, color);
			//ws.z = 
		}
		i++;
	}
}

void highlight_room(t_editor *ed, t_sdlcontext *sdl, t_area *room, uint32_t color)
{
	int	i;

	i = 0;
	while (i < room->edgecount)
	{
		t_vector3	ws;
		t_vector3	ws2;
		t_vector2	start;

		start = room->edges[i];
		ws = (t_vector3){start.x, start.y, room->height};
		render_gizmo3d(sdl, ws, 2, color);
		sdl->render.gizmocolor = color;
		ws2 = v2tov3(next_edge(room, i));
		ws2.z = room->height;
		if (!room->loop && i == room->edgecount - 1)
			;
		else
			render_ray3D(sdl, ws, ws2, color);
		i++;
	}
	highlight_area_walls(ed, sdl, room, color);
}
