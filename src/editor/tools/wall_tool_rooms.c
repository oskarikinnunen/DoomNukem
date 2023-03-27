/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_tool_rooms.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 03:20:37 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:23:37 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "collision.h"
#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "objects.h"

//TODO: lol rename, I'm tired
static void	room_floor_apply_obj_vertices_etc(t_world *world,
		int i, t_area *room, t_floorcalc fc)
{
	t_meshtri	*mtri;

	mtri = &room->floors[i];
	mtri->v[0] = v2tov3(fc.edges[fc.faces[i].v_indices[0]]);
	mtri->v[1] = v2tov3(fc.edges[fc.faces[i].v_indices[1]]);
	mtri->v[2] = v2tov3(fc.edges[fc.faces[i].v_indices[2]]);
	mtri->v[0].z += room->height;
	mtri->v[1].z += room->height;
	mtri->v[2].z += room->height;
	mtri->uv[0] = fc.edges[fc.faces[i].v_indices[0]];
	mtri->uv[1] = fc.edges[fc.faces[i].v_indices[1]];
	mtri->uv[2] = fc.edges[fc.faces[i].v_indices[2]];
	mtri->uv[0] = vector2_div(mtri->uv[0], 100.0f);
	mtri->uv[1] = vector2_div(mtri->uv[1], 100.0f);
	mtri->uv[2] = vector2_div(mtri->uv[2], 100.0f);
}

static void	_room_apply_floor_objs(t_world *world, t_area *room, t_floorcalc fc)
{
	int			i;
	t_meshtri	*mtri;
	t_object	*obj;

	i = 0;
	while (i < fc.face_count)
	{
		mtri = &room->floors[i];
		obj = object_tri(world->sdl);
		obj->materials->img = get_image_by_name(*world->sdl,
				room->s_floor_tex.str);
		room_floor_apply_obj_vertices_etc(world, i, room, fc);
		apply_trimesh(*mtri, obj);
		mtri->entity = spawn_entity(world, obj);
		mtri->entity->rigid = true;
		i++;
	}
}

void	room_make_floor(t_world *world, t_area *room)
{
	t_floorcalc	fc;
	t_wall		*w;
	t_meshtri	*mtri;
	t_object	*obj;
	int			i;

	ft_bzero(&fc, sizeof(fc));
	i = 0;
	while (i < room->edge_count)
	{
		fc.edges[fc.edge_count++] = room->edges[i];
		i++;
	}
	triangulate(&fc);
	if (fc.face_count == 0)
		return ;
	ft_bzero(room->floors, sizeof(room->floors));
	room->floor_count = fc.face_count;
	_room_apply_floor_objs(world, room, fc);
}

static void	room_ceiling_apply_vertices_and_uv(t_area *room,
		t_meshtri *mtri, int i)
{
	mtri->v[0] = room->floors[i].v[0];
	mtri->v[1] = room->floors[i].v[1];
	mtri->v[2] = room->floors[i].v[2];
	mtri->v[0].z += room->ceiling_height;
	mtri->v[1].z += room->ceiling_height;
	mtri->v[2].z += room->ceiling_height;
	mtri->uv[0] = room->floors[i].uv[0];
	mtri->uv[1] = room->floors[i].uv[1];
	mtri->uv[2] = room->floors[i].uv[2];
}

void	room_make_ceilings(t_world *world, t_area *room)
{
	int			i;
	t_meshtri	*mtri;
	t_object	*obj;

	i = 0;
	while (i < room->floor_count)
	{
		mtri = &room->ceilings[i];
		obj = object_tri(world->sdl);
		obj->materials->img = get_image_by_name(*world->sdl,
				room->s_ceil_tex.str);
		room_ceiling_apply_vertices_and_uv(room, mtri, i);
		apply_trimesh(*mtri, obj);
		mtri->entity = spawn_entity(world, obj);
		mtri->entity->rigid = true;
		i++;
	}
	room->ceiling_count = i;
}
