/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 05:31:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/08 05:37:09 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "doomnukem.h"

void	applywallmesh(t_wall *wall)
{
	wall->object.vertices[0] = (t_vector3){wall->line.start.x, wall->line.start.y, 0.0f};
	wall->object.vertices[1] = (t_vector3){wall->line.end.x, wall->line.end.y, 0.0f};
	
	wall->object.vertices[2] = (t_vector3){wall->line.start.x, wall->line.start.y, wall->height};
	wall->object.vertices[3] = (t_vector3){wall->line.end.x, wall->line.end.y, wall->height};

	float dist = vector2_dist(wall->line.start, wall->line.end) + 2.0f;
	wall->object.uvs[1] = (t_vector2){dist / 100.0f, 0.0f};
	wall->object.uvs[2] = (t_vector2){0.0f, wall->height / 100.0f};
	wall->object.uvs[3] = (t_vector2){dist / 100.0f, wall->height / 100.0f};
}

void	walls_init(struct s_world *world)
{
	t_wall	*wall;
	t_list	*l;

	l = world->wall_list;
	while (l != NULL) 
	{
		wall = (t_wall *)l->content;
		wall->object = *object_plane();
		applywallmesh(wall);
		l = l->next;
	}
}