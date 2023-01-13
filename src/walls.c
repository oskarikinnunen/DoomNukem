/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 05:31:47 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/11 10:42:39 by vlaine           ###   ########.fr       */
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
	sdl->render.gizmocolor = CLR_GRAY;
	while (indexer.y < 90)
	{
		indexer.x = 0;
		while (indexer.x < 90)
		{
			ws = (t_vector3){origin.x + indexer.x, origin.y + indexer.y, 0.0f};
			if (indexer.x == 40 && indexer.y == 40)
			{
				if (hover)
					sdl->render.gizmocolor = CLR_RED;
				else
					sdl->render.gizmocolor = CLR_PRPL;
				render_ray(sdl, ws, (t_vector3){ws.x, ws.y, clen});
				render_ray(sdl, ws, (t_vector3){ws.x + clen, ws.y, 0.0f});
				render_ray(sdl, ws, (t_vector3){ws.x - clen, ws.y, 0.0f});
				render_ray(sdl, ws, (t_vector3){ws.x, ws.y + clen, 0.0f});
				render_ray(sdl, ws, (t_vector3){ws.x, ws.y - clen, 0.0f});
			}
			else
				sdl->render.gizmocolor = CLR_GRAY;
			render_gizmo(*sdl, sdl->render, ws, 2);
			indexer.x += 10;
		}
		indexer.y += 10;
	}
}

void	free_walls(t_room *room, t_world *world)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		if (room->walls[i].entity->obj != NULL)
		{
			free_object(room->walls[i].entity->obj);
			destroy_entity(world, room->walls[i].entity);
			room->walls[i].entity = NULL;
		}
		i++;
	}
}

//if edges_exists_in_other_room && 

t_wall	*find_wall(t_wall wall, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		if ((vector2_cmp(*room->walls[i].edgeline.start, *wall.edgeline.start)
			&& vector2_cmp(*room->walls[i].edgeline.end, *wall.edgeline.end))
			|| (vector2_cmp(*room->walls[i].edgeline.start, *wall.edgeline.end)
			&& vector2_cmp(*room->walls[i].edgeline.end, *wall.edgeline.start)))
			return (&room->walls[i]);
		i++;
	}
	return (NULL);	
}

bool	basicly_identical(t_wall *wall1, t_wall *wall2, t_room *room1, t_room *room2)
{
	return (wall1->height == wall2->height
			&& wall1->z_offset == wall2->z_offset
			&& room1->height == room2->height
			&& room1->ceiling_height == room2->ceiling_height);
}

void	clamp_wall_areaheight(t_wall *wall, t_room *room, t_world *world)
{
	t_room	*cur;
	t_list	*l;
	t_wall	*other_w;

	l = world->roomlist;
	wall->entity->hidden = false;
	wall->ceilingwall = false;
	wall->height = room->ceiling_height;
	wall->z_offset = 0;
	while (l != NULL)
	{
		cur = l->content;
		other_w = find_wall(*wall, cur);
		if (cur != room && /*edge_exists(*wall->edgeline.start, cur) && edge_exists(*wall->edgeline.end, cur)*/
			other_w != NULL && rooms_share_zspace(room, cur))
		{
			if (cur->height <= room->height)
			{
				wall->height = ft_abs((room->height + room->ceiling_height) - (cur->height + cur->ceiling_height));
				if (room->height + room->ceiling_height
					< cur->height + cur->ceiling_height)
				{
					wall->z_offset = room->ceiling_height;
					wall->height = (cur->height - room->height) + (cur->ceiling_height - room->ceiling_height);
				}
				else
					wall->z_offset = (room->ceiling_height) - wall->height;
				
				wall->ceilingwall = true;
				wall->entity->hidden = world->ceiling_toggle;
				//}
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
		wall->entity->obj->vertices[0] = v2tov3(*wall->edgeline.start);
		wall->entity->obj->vertices[0].z += room->height + wall->z_offset;
		wall->entity->obj->vertices[1] = v2tov3(*wall->edgeline.end);
		wall->entity->obj->vertices[1].z += room->height+ wall->z_offset;
		
		wall->entity->obj->vertices[2] = v2tov3(*wall->edgeline.start);
		wall->entity->obj->vertices[2].z += wall->height + room->height + wall->z_offset;
		wall->entity->obj->vertices[3] = v2tov3(*wall->edgeline.end);
		wall->entity->obj->vertices[3].z += wall->height + room->height + wall->z_offset;

		float dist = vector2_dist(*wall->edgeline.start, *wall->edgeline.end);
		wall->entity->obj->uvs[0] = vector2_zero();
		wall->entity->obj->uvs[1] = (t_vector2){dist / 100.0f, 0.0f};
		wall->entity->obj->uvs[2] = (t_vector2){0.0f, wall->height / 100.0f};
		wall->entity->obj->uvs[3] = (t_vector2){dist / 100.0f, wall->height / 100.0f};
		wall->entity->obj->uvs[0] = vector2_add(wall->entity->obj->uvs[0], wall->uv_offset);
		wall->entity->obj->uvs[1] = vector2_add(wall->entity->obj->uvs[1], wall->uv_offset);
		wall->entity->obj->uvs[2] = vector2_add(wall->entity->obj->uvs[2], wall->uv_offset);
		wall->entity->obj->uvs[3] = vector2_add(wall->entity->obj->uvs[3], wall->uv_offset);
		//create_lightmap_for_entity(wall->entity, world);
		//create_map_for_entity(wall->entity, world);
		/*wall->entity->obj->uvs[1] = flipped_uv(wall->entity->obj->uvs[1]);
		wall->entity->obj->uvs[2] = flipped_uv(wall->entity->obj->uvs[2]);
		wall->entity->obj->uvs[3] = flipped_uv(wall->entity->obj->uvs[3]);*/

		update_wall_bounds(wall);
		default_wall_occlusion_settings(wall, NULL);
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
		{
			room->walls[i].entity = spawn_entity(world); //Copy saved entitys important values
			room->walls[i].entity->rigid = true;
		}
			
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
		//printf("wall indexing = %i -> %i , last edge %i \n", room->walls[i].edgeline.start_index, room->walls[i].edgeline.end_index, room->edgecount - 1);
		room->walls[i].entity->transform.position = vector3_zero();
		room->walls[i].entity->transform.scale = vector3_one();
		if (room->walls[i].entity->obj == NULL)
		{
			//printf("allocated object for wall , img name %s \n", room->walls[i].texname);
			room->walls[i].entity->obj = object_plane(world->sdl);
			if (strlen(room->walls[i].texname) != 0)
			{
				room->walls[i].entity->obj->materials->img = get_image_by_name(*world->sdl, room->walls[i].texname);
				//printf("overriding image \n");
			}
				
		}
		room->walls[i].height = room->ceiling_height;
		applywallmesh(&room->walls[i], room, world);
		//update_wall_bounds(&room->walls[i]);
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
