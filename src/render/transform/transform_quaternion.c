/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_quaternion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:54:38 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/24 16:11:28 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool calculate_quat_buffer_for_entity(t_quaternion *q, t_entity *entity)
{
	t_object	*obj;
	int			index;
	t_mat4x4	matworld;

	obj = entity->obj;
	if (obj == NULL || q == NULL)
		return(false);
//	matworld = make_transform_matrix(entity->transform);
	entity->matworld = matworld;
	index = 0;
	while (index < obj->vertice_count)
	{
		q[index] = vector3_to_quaternion(obj->vertices[index]);
		if (entity->animation.active)
			q[index].v = vector3_add(entity->obj->o_anim.frames[entity->animation.frame].deltavertices[index].delta, q[index].v);
		// render->q[index] = quaternion_mul_matrix(matworld, render->q[index]);
		q[index] = transformed_vector3(entity->transform, q[index].v);
		index++;
	}
	//update_bounds_world_triangles(entity, matworld);
	return(true);
}
