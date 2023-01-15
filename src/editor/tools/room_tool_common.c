/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_common.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 08:53:20 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/09 19:04:06 by okinnune         ###   ########.fr       */
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

void	toggle_ceilings(t_world *world)
{
	t_list	*l;
	t_room	*r;
	int		i;

	l = world->roomlist;
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

bool	rooms_share_zspace(t_room *room1, t_room *room2)
{
	bool	share = false;
	if (room1->height < (room2->height + room2->ceiling_height)
		&& room1->height >= room2->height)
		share = true;
	if (room2->height < (room1->height + room1->ceiling_height)
		&& room2->height >= room1->height)
		share = true;
	return (share);
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
			edges[edgecount] = v2tov3(norm);
			edges[edgecount++].z += room->height;
		}
			
		i++;
	}
	i = 0;
	/*sdl->render.gizmocolor = CLR_RED;
	while (i < edgecount)
	{
		if (i == edgecount - 1)
		{
			//render_ray(*sdl, sdl->render, vector2_to_vector3(room->edges[i]), vector2_to_vector3(room->edges[0]));

			render_ray(*sdl, sdl->render, edges[i], edges[0]);
		}
		else
		{
			//render_ray(*sdl, sdl->render, vector2_to_vector3(room->edges[i]), vector2_to_vector3(room->edges[i + 1]));
			render_ray(*sdl, sdl->render, edges[i], edges[i + 1]);
		}
			
		i++;
	}*/
}

bool	edge_exists(t_vector2 edge, t_room	*room)
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

void highlight_room_edgelines(t_editor *ed, t_sdlcontext *sdl, t_room *room)
{
	int	i;

	i = 0;
	//while (i < )
}

static t_vector3 vector3_snap(t_vector3 vec, int mod)
{
	float	z;

	z = vec.z;
	vec = v2tov3(vector2_snap(v3tov2(vec), mod));
	vec.z = z;
	return (vec);
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

bool	is_joined(t_vector2 edge, t_room	*room, t_world *world)
{
	t_list	*l;
	t_room	*other;
	l = world->roomlist;
	while (l != NULL)
	{
		other = l->content;
		if (other != room && edge_exists(edge, other) && rooms_share_zspace(room, other))
			return (true);
		l = l->next;
	}
	return (false);
}

void highlight_room(t_editor *ed, t_sdlcontext *sdl, t_room *room, uint32_t color)
{
	int	i;

	i = 0;
	t_vector3	ws;
	while (i < room->edgecount)
	{
		ws = (t_vector3){room->edges[i].x, room->edges[i].y, room->height};
		if (color == AMBER_3)
			rendergrid(&ed->world, ws, 10, CLR_GRAY);
		else if (sdl->render_grid)
			rendergrid(&ed->world, ws, 10, CLR_BLACKGRAY);
		i++;
	}
	i = 0;
	sdl->render.wireframe = true;
	sdl->render.gizmocolor = color;
	while (i < room->wallcount)
	{
		t_wall	w;

		w = room->walls[i];
		//render_entity(sdl, &sdl->render, room->walls[i].entity);
		if (w.edgeline.end != NULL && w.edgeline.start != NULL && !w.entity->hidden)
		{
			float zl = room->height + w.z_offset;
			float zh = room->height + w.z_offset + w.height;
			
			if (is_joined(*w.edgeline.start, room, &ed->world)
				&& is_joined(*w.edgeline.end, room, &ed->world))
			{
				render_gizmo3d(sdl, vector3_add(v2tov3(*w.edgeline.start),(t_vector3){.z = zl}), 2, CLR_GREEN);
				render_gizmo3d(sdl, vector3_add(v2tov3(*w.edgeline.end),(t_vector3){.z = zl}), 2, CLR_GREEN);
				render_gizmo3d(sdl, vector3_add(v2tov3(*w.edgeline.start),(t_vector3){.z = zh}), 2, CLR_GREEN);
				render_gizmo3d(sdl, vector3_add(v2tov3(*w.edgeline.end),(t_vector3){.z = zh}), 2, CLR_GREEN);
			}
			render_ray(sdl,
				vector3_add(v2tov3(*w.edgeline.start),(t_vector3){.z = zh}),
				vector3_add(v2tov3(*w.edgeline.end),(t_vector3){.z = zh}));
			render_ray(sdl,
				vector3_add(v2tov3(*w.edgeline.start),(t_vector3){.z = zl}),
				vector3_add(v2tov3(*w.edgeline.end),(t_vector3){.z = zl}));
			render_ray(sdl,
				vector3_add(v2tov3(*w.edgeline.start),(t_vector3){.z = zl}),
				vector3_add(v2tov3(*w.edgeline.start),(t_vector3){.z = zh}));
			render_ray(sdl,
				vector3_add(v2tov3(*w.edgeline.end),(t_vector3){.z = zl}),
				vector3_add(v2tov3(*w.edgeline.end),(t_vector3){.z = zh}));
		}
		i++;
	}
	i = 0;
	while (i < room->edgecount)
	{
		t_vector3	ws;
		t_vector3	ws2;
		t_vector2	start;

		start = room->edges[i];
		if (is_joined(start, room, &ed->world))
			sdl->render.gizmocolor = CLR_GREEN;
		ws = (t_vector3){start.x, start.y, room->height};
		render_gizmo(*sdl, sdl->render, ws, 2);
		sdl->render.gizmocolor = color;
		ws2 = v2tov3(next_edge(room, i));
		ws2.z = room->height;
		render_ray(sdl, ws, ws2);
		i++;
	}
	sdl->render.wireframe = false;
	highlight_roomborders(ed, sdl, room);
}

/*void highlight_roomwalls(t_editor *ed, t_sdlcontext sdl, t_room room, uint32_t color)
{
	int	i;

	i = 0;
	sdl->render.wireframe = true;
	sdl->render.gizmocolor = color;
	while (i < room.wallcount)
	{
		//printf("HIGHLIGHTING ROOM \n");
		render_entity(&sdl, &sdl->render, room.walls[i].entity);
		i++;
	}
	sdl->render.wireframe = false;
}*/


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
		if (selectedfloor(ed, sdl, r) != NULL)
			return (r);
		l = l->next;
	}
	return (NULL);
}