/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 05:31:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/27 18:41:11 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "doomnukem.h"
#include "editor_tools.h"

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

void	applywallmesh(t_wall *wall, t_room *room, t_world *world)
{
	if (wall->edgeline.end != NULL && wall->edgeline.start != NULL)
	{
		wall->entity->obj->vertices[0] = vector2_to_vector3(*wall->edgeline.start);
		wall->entity->obj->vertices[0].z += room->height;
		wall->entity->obj->vertices[1] = vector2_to_vector3(*wall->edgeline.end);
		wall->entity->obj->vertices[1].z += room->height;
		
		wall->entity->obj->vertices[2] = vector2_to_vector3(*wall->edgeline.start);
		wall->entity->obj->vertices[2].z += wall->height + room->height;
		wall->entity->obj->vertices[3] = vector2_to_vector3(*wall->edgeline.end);
		wall->entity->obj->vertices[3].z += wall->height + room->height;

		float dist = vector2_dist(*wall->edgeline.start, *wall->edgeline.end) + 2.0f;
		wall->entity->obj->uvs[1] = (t_vector2){dist / 100.0f, 0.0f};
		wall->entity->obj->uvs[2] = (t_vector2){0.0f, wall->height / 100.0f};
		wall->entity->obj->uvs[3] = (t_vector2){dist / 100.0f, wall->height / 100.0f};
		
		create_lightmap_for_entity(wall->entity, world);
		create_map_for_entity(wall->entity, world);

		update_wall_bounds(wall);
		default_wall_occlusion_settings(wall, NULL);

		
		/*wall->entity->obj->uvs[1] = flipped_uv(wall->entity->obj->uvs[1]);
		wall->entity->obj->uvs[2] = flipped_uv(wall->entity->obj->uvs[2]);
		wall->entity->obj->uvs[3] = flipped_uv(wall->entity->obj->uvs[3]);*/
	}/* else
	{
		wall->entity->obj->vertices[0] = (t_vector3){wall->line.start.x, wall->line.start.y, 0.0f};
		wall->entity->obj->vertices[1] = (t_vector3){wall->line.end.x, wall->line.end.y, 0.0f};
		wall->entity->obj->vertices[2] = (t_vector3){wall->line.start.x, wall->line.start.y, wall->height};
		wall->entity->obj->vertices[3] = (t_vector3){wall->line.end.x, wall->line.end.y, wall->height};

		float dist = vector2_dist(wall->line.start, wall->line.end) + 2.0f;
		wall->entity->obj->uvs[1] = (t_vector2){dist / 100.0f, 0.0f};
		wall->entity->obj->uvs[2] = (t_vector2){0.0f, wall->height / 100.0f};
		wall->entity->obj->uvs[3] = (t_vector2){dist / 100.0f, wall->height / 100.0f};
		wall->entity->obj->uvs[1] = flipped_uv(wall->entity->obj->uvs[1]);
		wall->entity->obj->uvs[2] = flipped_uv(wall->entity->obj->uvs[2]);
		wall->entity->obj->uvs[3] = flipped_uv(wall->entity->obj->uvs[3]);
	}*/
	
}

void	init_roomwalls(t_world *world, t_room *room)
{
	int			i;
	t_entity	*ent;

	i = 0;
	while (i < room->wallcount)
	{
		if (room->walls[i].entity == NULL)
			room->walls[i].entity = spawn_entity(world); //Copy saved entitys important values
		if (room->walls[i].disabled)
			room->walls[i].entity->hidden = true;
		//room->walls
		room->walls[i].edgeline.start = &room->edges[i];
		room->walls[i].edgeline.start_index = i;
		if (i != room->wallcount - 1)
		{
			room->walls[i].edgeline.end = &room->edges[i + 1];
			room->walls[i].edgeline.end_index = i + 1;
		}
		else
		{
			room->walls[i].edgeline.end = &room->edges[0];
			room->walls[i].edgeline.end_index = 0;
		}
			
		room->walls[i].entity->transform.position = vector3_zero();
		room->walls[i].entity->transform.scale = vector3_one();
		if (room->walls[i].entity->obj == NULL)
			room->walls[i].entity->obj = object_plane(world->sdl);
		applywallmesh(&room->walls[i], room, world);
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

void	init_room_meshes(t_room *room, t_sdlcontext *sdl, t_world *world)
{
	int	i;

	i = 0;
	while (i < room->floorcount)
	{
		//room->floors[i].entity = raise_entity(world);
		room->floors[i].entity->obj = object_tri(sdl);
		applytrimesh(room->floors[i], room->floors[i].entity->obj);
		default_floor_occlusion_settings(&room->floors[i], NULL);
		update_floor_bounds(&room->floors[i]);
		/*room->walls[i].entity->transform.position = vector3_zero();
		room->walls[i].entity->transform.scale = vector3_one();
		room->walls[i].entity->obj = object_plane(sdl);
		applywallmesh(&room->walls[i]);*/
		i++;
	}
}
