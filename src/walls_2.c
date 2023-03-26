/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:19:24 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:40:52 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void	wall_calc_height(t_area *room, t_area *cur,
		t_wall *wall, t_wall *other_w)
{
	if (cur->height <= room->height)
	{
		wall->height = ft_abs((room->height + room->ceiling_height)
				- (cur->height + cur->ceiling_height));
		if (room->height + room->ceiling_height
			< cur->height + cur->ceiling_height)
		{
			wall->z_offset = room->ceiling_height;
			wall->height = (cur->height - room->height)
				+ (cur->ceiling_height - room->ceiling_height);
			if (other_w->height == wall->height
				&& other_w->z_offset == wall->z_offset)
				wall->entity->hidden = true;
		}
		else
			wall->z_offset = (room->ceiling_height) - wall->height;
		wall->ceiling_wall = true;
	}
	else
	{
		wall->height = cur->height - room->height;
		wall->entity->hidden = false;
	}
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
		if (cur != room
			&& other_w != NULL && rooms_share_zspace(room, cur))
		{
			wall_calc_height(room, cur, wall, other_w);
			return ;
		}
		l = l->next;
	}
}

static void	wall_apply_uvs(t_wall *wall)
{
	float	dist;
	int		i;

	dist = vector2_dist(*wall->edge_line.start, *wall->edge_line.end);
	wall->entity->obj->uvs[0] = vector2_zero();
	wall->entity->obj->uvs[1] = (t_vector2){dist / 100.0f, 0.0f};
	wall->entity->obj->uvs[2] = (t_vector2){0.0f, wall->height / 100.0f};
	wall->entity->obj->uvs[3]
		= (t_vector2){dist / 100.0f, wall->height / 100.0f};
	i = 0;
	while (i < 4)
	{
		wall->entity->obj->uvs[i].y
			+= wall->entity->obj->vertices[0].z / 100.0f;
		i++;
	}
}

static void	wall_apply_vertices(t_wall *wall, t_area *room)
{
	wall->entity->obj->vertices[0] = v2tov3(*wall->edge_line.start);
	wall->entity->obj->vertices[0].z += room->height + wall->z_offset;
	wall->entity->obj->vertices[1] = v2tov3(*wall->edge_line.end);
	wall->entity->obj->vertices[1].z
		+= room->height + wall->z_offset;
	wall->entity->obj->vertices[2] = v2tov3(*wall->edge_line.start);
	wall->entity->obj->vertices[2].z
		+= wall->height + room->height + wall->z_offset;
	wall->entity->obj->vertices[3] = v2tov3(*wall->edge_line.end);
	wall->entity->obj->vertices[3].z
		+= wall->height + room->height + wall->z_offset;
}

void	apply_wallmesh(t_wall *wall, t_area *room, t_world *world)
{
	int	i;

	if (wall->edge_line.start != NULL && wall->edge_line.end != NULL)
		clamp_wall_areaheight(wall, room, world);
	if (wall->edge_line.end != NULL && wall->edge_line.start != NULL)
	{
		wall_apply_vertices(wall, room);
		wall_apply_uvs(wall);
		default_wall_occlusion_settings(wall, NULL);
	}
}
