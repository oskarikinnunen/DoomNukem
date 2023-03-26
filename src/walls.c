/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 05:31:47 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:18:22 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "doomnukem.h"
#include "editor_tools.h"

void	free_walls(t_area *room, t_world *world)
{
	int	i;

	i = 0;
	while (i < room->wall_count)
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

t_wall	*find_wall(t_wall wall, t_area *room)
{
	int	i;

	i = 0;
	while (i < room->wall_count)
	{
		if (room->walls[i].edge_line.start == NULL || room->walls[i].edge_line.end == NULL
			|| wall.edge_line.start == NULL || wall.edge_line.end == NULL)
		{
			i++;
			continue;
		}
		if ((vector2_cmp(*room->walls[i].edge_line.start, *wall.edge_line.start)
			&& vector2_cmp(*room->walls[i].edge_line.end, *wall.edge_line.end))
			|| (vector2_cmp(*room->walls[i].edge_line.start, *wall.edge_line.end)
			&& vector2_cmp(*room->walls[i].edge_line.end, *wall.edge_line.start)))
			return (&room->walls[i]);
		i++;
	}
	return (NULL);	
}

void	clamp_wall_areaheight(t_wall *wall, t_area *room, t_world *world)
{
	t_area	*cur;
	t_list	*l;
	t_wall	*other_w;

	l = world->arealist;
	wall->entity->hidden = false;
	wall->ceiling_wall = false;
	wall->height = room->ceiling_height;
	wall->z_offset = 0;
	while (l != NULL)
	{
		cur = l->content;
		other_w = find_wall(*wall, cur);
		if (cur != room &&
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
					if (other_w->height == wall->height && other_w->z_offset == wall->z_offset)
						wall->entity->hidden = true;
				}
				else
					wall->z_offset = (room->ceiling_height) - wall->height;
				
				wall->ceiling_wall = true;
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

void	apply_wall_mesh(t_wall *wall, t_area *room, t_world *world)
{
	if (wall->edge_line.start != NULL && wall->edge_line.end != NULL)
		clamp_wall_areaheight(wall, room, world);
	if (wall->edge_line.end != NULL && wall->edge_line.start != NULL)
	{
		wall->entity->obj->vertices[0] = v2tov3(*wall->edge_line.start);
		wall->entity->obj->vertices[0].z += room->height + wall->z_offset;
		wall->entity->obj->vertices[1] = v2tov3(*wall->edge_line.end);
		wall->entity->obj->vertices[1].z += room->height+ wall->z_offset;
		
		wall->entity->obj->vertices[2] = v2tov3(*wall->edge_line.start);
		wall->entity->obj->vertices[2].z += wall->height + room->height + wall->z_offset;
		wall->entity->obj->vertices[3] = v2tov3(*wall->edge_line.end);
		wall->entity->obj->vertices[3].z += wall->height + room->height + wall->z_offset;

		float dist = vector2_dist(*wall->edge_line.start, *wall->edge_line.end);
		wall->entity->obj->uvs[0] = vector2_zero();
		wall->entity->obj->uvs[1] = (t_vector2){dist / 100.0f, 0.0f};
		wall->entity->obj->uvs[2] = (t_vector2){0.0f, wall->height / 100.0f};
		wall->entity->obj->uvs[3] = (t_vector2){dist / 100.0f, wall->height / 100.0f};
		int i = 0;
		while (i < 4)
		{
			wall->entity->obj->uvs[i].y += wall->entity->obj->vertices[0].z / 100.0f;
			i++;
		}
		wall->entity->obj->uvs[0] = vector2_add(wall->entity->obj->uvs[0], wall->uv_offset);
		wall->entity->obj->uvs[1] = vector2_add(wall->entity->obj->uvs[1], wall->uv_offset);
		wall->entity->obj->uvs[2] = vector2_add(wall->entity->obj->uvs[2], wall->uv_offset);
		wall->entity->obj->uvs[3] = vector2_add(wall->entity->obj->uvs[3], wall->uv_offset);
		default_wall_occlusion_settings(wall, NULL);
	}
}

void	_room_initwalls(t_world *world, t_area *room)
{
	int			i;
	t_entity	*ent;

	i = 0;
	room->wall_count = room->edge_count;
	if (!room->floor_enabled && !room->loop)
		room->wall_count--;
	while (i < room->wall_count)
	{
		room->walls[i].edge_line.start = &room->edges[i];
		room->walls[i].edge_line.start_index = i;
		if (i != room->edge_count - 1)
		{
			room->walls[i].edge_line.end = &room->edges[i + 1];
			room->walls[i].edge_line.end_index = i + 1;
		}
		else
		{
			room->walls[i].edge_line.end = &room->edges[0];
			room->walls[i].edge_line.end_index = 0;
		}
		if (room->walls[i].entity == NULL)
		{
			t_object *obj;
			obj = object_plane(world->sdl);
			if (ft_strlen(room->walls[i].s_walltex.str) != 0)
				obj->materials->img = get_image_by_name(*world->sdl, room->walls[i].s_walltex.str);
			room->walls[i].entity = spawn_entity(world, obj);
			room->walls[i].entity->rigid = true;
		}
		room->walls[i].entity->transform.position = vector3_zero();
		room->walls[i].entity->transform.scale = vector3_one();
		room->walls[i].height = room->ceiling_height;
		apply_wall_mesh(&room->walls[i], room, world);
		i++;
	}
}

void	_room_initwalls_shallow(t_world *world, t_area *room)
{
	int			i;
	t_entity	*ent;

	i = 0;
	room->wall_count = room->edge_count;
	if (!room->floor_enabled && !room->loop)
		room->wall_count--;
	while (i < room->wall_count)
	{
		room->walls[i].edge_line.start = &room->edges[i];
		room->walls[i].edge_line.start_index = i;
		if (i != room->edge_count - 1)
		{
			room->walls[i].edge_line.end = &room->edges[i + 1];
			room->walls[i].edge_line.end_index = i + 1;
		}
		else
		{
			room->walls[i].edge_line.end = &room->edges[0];
			room->walls[i].edge_line.end_index = 0;
		}
		if (room->walls[i].entity == NULL)
		{
			t_object *obj;
			obj = object_plane(world->sdl);
			if (strlen(room->walls[i].s_walltex.str) != 0)
				obj->materials->img = get_image_by_name(*world->sdl, room->walls[i].s_walltex.str);
			room->walls[i].entity = spawn_entity(world, obj); //Copy saved entitys important values
			room->walls[i].entity->rigid = true;
		}
		room->walls[i].entity->transform.position = vector3_zero();
		room->walls[i].entity->transform.scale = vector3_one();
		room->walls[i].height = room->ceiling_height;
		//update_wall_bounds(&room->walls[i]);
		i++;
	}
}

void	room_remove_entities(t_area *room, t_world *world)
{
	free_roomwalls(world, room);
	free_floor(world, room);
	free_ceilings(world, room);
}

void	room_init(t_area *room, t_world *world)
{
	room_remove_entities(room, world);
	if (room->walls_enabled)
		_room_initwalls(world, room);
	else
	{
		room->wall_count = 0;
		room->ceiling_height = 0;
	}
	if (room->floor_enabled)
	{
		room_make_floor(world, room);
		if (room->ceiling_enabled)
			room_make_ceilings(world, room);
	}
	doomlog_mul(LOG_NORMAL, (char *[3]){\
		"initialized area:", room->name, NULL});
}

void	room_init_shallow(t_area *room, t_world *world)
{
	room_remove_entities(room, world);
	if (room->walls_enabled)
		_room_initwalls_shallow(world, room);
	if (room->floor_enabled)
	{
		room_make_floor(world, room);
		if (room->ceiling_enabled)
			room_make_ceilings(world, room);
	}
}

void	apply_tri_mesh(t_meshtri tri, t_object *obj)
{
	obj->vertices[0] = tri.v[0];
	obj->vertices[1] = tri.v[1];
	obj->vertices[2] = tri.v[2];
	obj->uvs[0] = tri.uv[0];
	obj->uvs[1] = tri.uv[1];
	obj->uvs[2] = tri.uv[2];
}
