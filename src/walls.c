/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 05:31:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/29 16:54:58 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "doomnukem.h"

void	render_snapgrid(t_editor *ed, t_sdlcontext *sdl, t_vector2 wallpos, bool shift, bool hover)
{
	t_point		indexer;
	t_vector3	origin;
	t_vector3	ws;
	float		clen;

	indexer = point_zero();
	origin = (t_vector3){wallpos.x - 40.0f, wallpos.y - 40.0f, 0.0f};
	clen = 40 + (shift && !hover) * 220;
	ed->render.gizmocolor = CLR_GRAY;
	while (indexer.y < 90)
	{
		indexer.x = 0;
		while (indexer.x < 90)
		{
			ws = (t_vector3){origin.x + indexer.x, origin.y + indexer.y, 0.0f};
			if (indexer.x == 40 && indexer.y == 40)
			{
				if (hover)
					ed->render.gizmocolor = CLR_RED;
				else
					ed->render.gizmocolor = CLR_PRPL;
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x, ws.y, clen});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x + clen, ws.y, 0.0f});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x - clen, ws.y, 0.0f});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x, ws.y + clen, 0.0f});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x, ws.y - clen, 0.0f});
			}
			else
				ed->render.gizmocolor = CLR_GRAY;
			render_gizmo(*sdl, ed->render, ws, 2);
			indexer.x += 10;
		}
		indexer.y += 10;
	}
}

void	applywallmesh(t_wall *wall)
{
	wall->entity.obj->vertices[0] = (t_vector3){wall->line.start.x, wall->line.start.y, 0.0f};
	wall->entity.obj->vertices[1] = (t_vector3){wall->line.end.x, wall->line.end.y, 0.0f};
	
	wall->entity.obj->vertices[2] = (t_vector3){wall->line.start.x, wall->line.start.y, wall->height};
	wall->entity.obj->vertices[3] = (t_vector3){wall->line.end.x, wall->line.end.y, wall->height};

	float dist = vector2_dist(wall->line.start, wall->line.end) + 2.0f;
	wall->entity.obj->uvs[1] = (t_vector2){dist / 100.0f, 0.0f};
	wall->entity.obj->uvs[2] = (t_vector2){0.0f, wall->height / 100.0f};
	wall->entity.obj->uvs[3] = (t_vector2){dist / 100.0f, wall->height / 100.0f};
}

static float get_wall_radius(t_bounds bounds)
{
	float min_radius;
	float max_radius;

	min_radius = vector3_dist(bounds.origin, bounds.box.v[0]);
	max_radius = vector3_dist(bounds.origin, bounds.box.v[3]);
	if (max_radius > min_radius)
		return(max_radius);
	else
		return(min_radius);
}

static void set_wall_obj_boundingbox(t_wall *wall)
{
	t_object *obj;
	obj = wall->entity.obj;
	obj->bounds.box.v[0] = (t_vector3){wall->line.start.x, wall->line.start.y, 0.0f};
	obj->bounds.box.v[1] = (t_vector3){wall->line.end.x, wall->line.end.y, 0.0f};
	obj->bounds.box.v[2] = (t_vector3){wall->line.start.x, wall->line.start.y, wall->height};
	obj->bounds.box.v[3] = (t_vector3){wall->line.end.x, wall->line.end.y, wall->height};
	//bzero(&obj->bounds.box.v[4], sizeof(t_vector3) * 4);
	obj->bounds.origin = vector3_lerp(obj->bounds.box.v[0], obj->bounds.box.v[3], 0.5f);
	obj->bounds.radius = get_wall_radius(obj->bounds);
	obj->bounds.type = plane;
	wall->entity.occlusion.is_backface_cull = false;
	wall->entity.occlusion.is_occluded = false;
	wall->entity.occlusion.occlusion_cull = occlude_and_cull;
}

void	init_roomwalls(t_room *room, t_sdlcontext *sdl)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		room->walls[i].entity.transform.location = vector3_zero();
		if (!room->walls[i].disabled)
			room->walls[i].entity.transform.scale = vector3_one();
		else
			room->walls[i].entity.transform.scale = vector3_zero();
		room->walls[i].entity.obj = object_plane(sdl);
		applywallmesh(&room->walls[i]);
		set_wall_obj_boundingbox(&room->walls[i]);
		i++;
	}
}

void	applytrimesh(t_meshtri tri, t_object *obj)
{
	obj->vertices[0] = tri.v[0];
	obj->vertices[1] = tri.v[1];
	obj->vertices[2] = tri.v[2];
	obj->uvs[0] = tri.uv[0];
	obj->uvs[1] = tri.uv[1];
	obj->uvs[2] = tri.uv[2];
}

void	init_room_meshes(t_room *room, t_sdlcontext *sdl)
{
	int	i;

	i = 0;
	while (i < room->floorcount)
	{
		room->floors[i].entity.obj = object_tri(sdl);
		applytrimesh(room->floors[i], room->floors[i].entity.obj);
		room->floors[i].entity.obj->bounds.origin = vector3_lerp(vector3_lerp(room->floors[i].entity.obj->vertices[0], room->floors[i].entity.obj->vertices[1], 0.5f), room->floors[i].entity.obj->vertices[2], 0.5f);
		room->floors[i].entity.id = -2;
		/*room->walls[i].entity.transform.location = vector3_zero();
		room->walls[i].entity.transform.scale = vector3_one();
		room->walls[i].entity.obj = object_plane(sdl);
		applywallmesh(&room->walls[i]);*/
		i++;
	}
}
