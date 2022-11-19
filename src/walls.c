/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 05:31:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/19 13:44:52 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "doomnukem.h"

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

static void set_wall_boundingbox(t_wall *wall)
{
	wall->entity.boundingbox.boundingbox[0] = (t_vector3){wall->line.start.x, wall->line.start.y, 0.0f};
	wall->entity.boundingbox.boundingbox[1] = (t_vector3){wall->line.end.x, wall->line.end.y, 0.0f};
	wall->entity.boundingbox.boundingbox[2] = (t_vector3){wall->line.start.x, wall->line.start.y, wall->height};
	wall->entity.boundingbox.boundingbox[3] = (t_vector3){wall->line.end.x, wall->line.end.y, wall->height};
	wall->entity.boundingbox.height = wall->height;
	wall->entity.boundingbox.is_wall = true;
}

void	walls_init(struct s_world *world)
{
	t_wall	*wall;
	t_list	*l;

	l = world->wall_list;
	while (l != NULL) 
	{
		wall = (t_wall *)l->content;
		wall->entity.obj = object_plane();
		set_wall_boundingbox(wall);
		wall->entity.transform.location = vector3_zero();
		wall->entity.transform.scale = vector3_one();
		applywallmesh(wall);
		l = l->next;
	}
}