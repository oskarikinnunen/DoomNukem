/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_bounds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:41:03 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/23 21:41:53 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"


inline static void	update_world_triangle_box(t_quaternion *transformed, t_world_triangle *world_triangles)
{
	int i;
	
	i = 0;
	while (i < 4)
	{
		world_triangles[i].p[0] = transformed[i];
		world_triangles[i].p[1] = transformed[i + 4];
		world_triangles[i].p[2] = transformed[((i + 1) % 4) + 4];
		world_triangles[i + 4].p[0] = transformed[i];
		world_triangles[i + 4].p[1] = transformed[(i + 1) % 4];
		world_triangles[i + 4].p[2] = transformed[((i + 1) % 4) + 4];
		i++;
	}
	world_triangles[8].p[0] = transformed[0];
	world_triangles[8].p[1] = transformed[1];
	world_triangles[8].p[2] = transformed[2];
	world_triangles[9].p[0] = transformed[0];
	world_triangles[9].p[1] = transformed[2];
	world_triangles[9].p[2] = transformed[3];
	world_triangles[10].p[0] = transformed[4];
	world_triangles[10].p[1] = transformed[5];
	world_triangles[10].p[2] = transformed[6];
	world_triangles[11].p[0] = transformed[4];
	world_triangles[11].p[1] = transformed[6];
	world_triangles[11].p[2] = transformed[7];
}

inline static void	update_world_triangle_plane(t_quaternion *transformed, t_world_triangle *world_triangles)
{
	world_triangles[0].p[0] = transformed[0];
	world_triangles[0].p[1] = transformed[2];
	world_triangles[0].p[2] = transformed[3];
	world_triangles[1].p[0] = transformed[0];
	world_triangles[1].p[1] = transformed[1];
	world_triangles[1].p[2] = transformed[3];
}

//rename function and enum that has the same name
inline static void	update_world_triangle_ignore(t_quaternion *transformed, t_world_triangle *world_triangles)
{
	world_triangles[0] = (t_world_triangle){.p[0] = transformed[0],.p[1] = transformed[1],.p[2] = transformed[2]};
}

inline static void update_bounds_world_triangles(t_entity *entity, t_mat4x4 matworld)
{
	t_quaternion	transformed[8];
	t_object		*obj;
	int				index;

	obj = entity->obj;
	index = 0;
	while (index < obj->bounds.count)
	{
		transformed[index] = vector3_to_quaternion(obj->bounds.box.v[index]);
		transformed[index] = quaternion_mul_matrix(matworld, transformed[index]);
		index++;
	}
	if (obj->bounds.type == bt_box)
		update_world_triangle_box(transformed, entity->occlusion.world_tri);
	else if (obj->bounds.type == bt_plane)
		update_world_triangle_plane(transformed, entity->occlusion.world_tri);
	else
		update_world_triangle_ignore(transformed, entity->occlusion.world_tri);
}
