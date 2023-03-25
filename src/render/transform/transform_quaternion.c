/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_quaternion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:54:38 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/25 16:30:46 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static t_box	set_bounding_box(t_quaternion q, t_box box)
{
	box.max.x = ft_max(box.max.x, q.v.x);
	box.max.y = ft_max(box.max.y, q.v.y);
	box.max.z = ft_max(box.max.z, q.v.z);
	box.min.x = ft_min(box.min.x, q.v.x);
	box.min.y = ft_min(box.min.y, q.v.y);
	box.min.z = ft_min(box.min.z, q.v.z);
	return (box);
}

static inline t_quaternion	transform_entity_vertex_to_quaternion(
									t_entity *entity, int index)
{
	t_quaternion	q;

	q = vector3_to_quaternion(entity->obj->vertices[index]);
	if (entity->animation.active)
		q.v = vector3_add(entity->obj->o_anim.frames[\
					entity->animation.frame].deltavertices[index].delta, q.v);
	q = transformed_vector3(entity->transform, q.v);
	return (q);
}

bool	calculate_quat_buffer_for_entity(t_quaternion *q, t_entity *entity)
{
	t_mat4x4	matworld;
	int			index;

	q[0] = transform_entity_vertex_to_quaternion(entity, 0);
	entity->occlusion.bounds.box.max = q[0].v;
	entity->occlusion.bounds.box.min = q[0].v;
	entity->old_transform = entity->transform;
	index = 1;
	while (index < entity->obj->vertice_count)
	{
		q[index] = transform_entity_vertex_to_quaternion(entity, index);
		entity->occlusion.bounds.box = \
				set_bounding_box(q[index], entity->occlusion.bounds.box);
		index++;
	}
	update_bounds_world_triangles(entity);
	return (true);
}
