/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 05:31:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/28 20:18:59 by okinnune         ###   ########.fr       */
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

//if edges_exists_in_other_room && 

void	clamp_wall_areaheight(t_wall *wall, t_room *room, t_world *world)
{
	t_room	*cur;
	t_list	*l;

	l = world->roomlist;
	while (l != NULL)
	{
		cur = l->content;
		if (cur != room && edge_exists(*wall->edgeline.start, cur) && edge_exists(*wall->edgeline.end, cur))
		{
			if (cur->height <= room->height)
			{
				wall->height = room->ceiling_height;
				wall->entity->hidden = true;
				//wall->entity->hidden = !wall->disabled;b
			}
			else
			{
				wall->height = cur->height - room->height;
				wall->entity->hidden = false;
			}
			return ;
		}
		l = l->next;
	}
}

void	applywallmesh(t_wall *wall, t_room *room, t_world *world)
{
	if (wall->edgeline.start != NULL && wall->edgeline.end != NULL)
		clamp_wall_areaheight(wall, room, world);
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

		float dist = vector2_dist(*wall->edgeline.start, *wall->edgeline.end);
		wall->entity->obj->uvs[0] = vector2_zero();
		wall->entity->obj->uvs[1] = (t_vector2){dist / 100.0f, 0.0f};
		wall->entity->obj->uvs[2] = (t_vector2){0.0f, wall->height / 100.0f};
		wall->entity->obj->uvs[3] = (t_vector2){dist / 100.0f, wall->height / 100.0f};
		printf("dist %f, height %i added %i\n", dist, wall->height, wall->height + room->height);
		printf("calculated uvs: \n%f %f\n%f %f\n%f %f\n%f %f\n\n",
			wall->entity->obj->uvs[0].x, wall->entity->obj->uvs[0].y,
			wall->entity->obj->uvs[1].x, wall->entity->obj->uvs[1].y,
			wall->entity->obj->uvs[2].x, wall->entity->obj->uvs[2].y,
			wall->entity->obj->uvs[3].x, wall->entity->obj->uvs[3].y);
		create_lightmap_for_entity(wall->entity, world);
		create_map_for_entity(wall->entity, world);
		
		/*wall->entity->obj->uvs[1] = flipped_uv(wall->entity->obj->uvs[1]);
		wall->entity->obj->uvs[2] = flipped_uv(wall->entity->obj->uvs[2]);
		wall->entity->obj->uvs[3] = flipped_uv(wall->entity->obj->uvs[3]);*/
	}
}

void	init_roomwalls(t_world *world, t_room *room)
{
	int			i;
	t_entity	*ent;

	i = 0;
	room->wallcount = room->edgecount;
	while (i < room->edgecount)
	{
		if (room->walls[i].entity == NULL)
			room->walls[i].entity = spawn_entity(world); //Copy saved entitys important values
		/*if (room->walls[i].disabled)
			room->walls[i].entity->hidden = true;*/
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
		printf("wall indexing = %i -> %i , last edge %i \n", room->walls[i].edgeline.start_index, room->walls[i].edgeline.end_index, room->edgecount - 1);
		room->walls[i].entity->transform.position = vector3_zero();
		room->walls[i].entity->transform.scale = vector3_one();
		if (room->walls[i].entity->obj == NULL)
		{
			printf("allocated object for wall , img name %s \n", room->walls[i].texname);
			room->walls[i].entity->obj = object_plane(world->sdl);
			room->walls[i].entity->obj->materials->img = get_image_by_name(*world->sdl, room->walls[i].texname);
			printf("img size: %i x %i \n", room->walls[i].entity->obj->materials->img->size.x, room->walls[i].entity->obj->materials->img->size.y);
		}
		room->walls[i].height = room->ceiling_height;
		//printf("wall height %i points: 1: %f %f 2: %f %f  \n", room->walls[i].height, room->walls[i].edgeline.start->x, room->walls[i].edgeline.start->y, room->walls[i].edgeline.end->x, room->walls[i].edgeline.start->y);
		applywallmesh(&room->walls[i], room, world);
		update_wall_bounds(&room->walls[i]);
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
		/*room->walls[i].entity->transform.position = vector3_zero();
		room->walls[i].entity->transform.scale = vector3_one();
		room->walls[i].entity->obj = object_plane(sdl);
		applywallmesh(&room->walls[i]);*/
		i++;
	}
}
