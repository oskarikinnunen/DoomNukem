/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_bounds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:41:03 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/25 16:34:30 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

// [0] = bottom left
// [3] = bottom right 3
// [5] = top left 1
// [6] = top right 2
static void	set_vertices_from_bounds(t_quaternion vertices[8], t_box box)
{
	vertices[0] = (t_quaternion){box.min.x, box.min.y, box.min.z, 1.0f};
	vertices[1] = (t_quaternion){box.min.x, box.max.y, box.min.z, 1.0f};
	vertices[2] = (t_quaternion){box.max.x, box.max.y, box.min.z, 1.0f};
	vertices[3] = (t_quaternion){box.max.x, box.min.y, box.min.z, 1.0f};
	vertices[4] = (t_quaternion){box.min.x, box.min.y, box.max.z, 1.0f};
	vertices[5] = (t_quaternion){box.min.x, box.max.y, box.max.z, 1.0f};
	vertices[6] = (t_quaternion){box.max.x, box.max.y, box.max.z, 1.0f};
	vertices[7] = (t_quaternion){box.max.x, box.min.y, box.max.z, 1.0f};
}

static inline void	update_world_triangle_box(
		t_quaternion vertices[8], t_world_triangle *world_triangles)
{
	ft_memcpy(world_triangles[0].p, (t_quaternion[3]){\
			vertices[0], vertices[1], vertices[2]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[1].p, (t_quaternion[3]){\
			vertices[0], vertices[2], vertices[3]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[2].p, (t_quaternion[3]){\
			vertices[4], vertices[6], vertices[5]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[3].p, (t_quaternion[3]){\
			vertices[4], vertices[7], vertices[6]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[4].p, (t_quaternion[3]){\
			vertices[4], vertices[5], vertices[1]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[5].p, (t_quaternion[3]){\
			vertices[4], vertices[1], vertices[0]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[6].p, (t_quaternion[3]){\
			vertices[3], vertices[2], vertices[6]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[7].p, (t_quaternion[3]){\
			vertices[3], vertices[6], vertices[7]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[8].p, (t_quaternion[3]){\
			vertices[1], vertices[5], vertices[6]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[9].p, (t_quaternion[3]){\
			vertices[1], vertices[6], vertices[2]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[10].p, (t_quaternion[3]){\
			vertices[4], vertices[0], vertices[3]}, sizeof(t_quaternion) * 3);
	ft_memcpy(world_triangles[11].p, (t_quaternion[3]){\
			vertices[4], vertices[3], vertices[7]}, sizeof(t_quaternion) * 3);
}

static void	update_entity_radius_and_origin(t_entity *entity, t_quaternion q[8])
{
	float	dist;
	t_box	box;
	int		i;

	box = entity->occlusion.bounds.box;
	entity->occlusion.bounds.origin = vector3_div(\
									vector3_add(box.max, box.min), 2.0f);
	entity->occlusion.bounds.radius = vector3_dist(\
									entity->occlusion.bounds.origin, q[0].v);
	i = 1;
	while (i < 8)
	{
		dist = vector3_dist(entity->occlusion.bounds.origin, q[i].v);
		if (dist > entity->occlusion.bounds.radius)
			entity->occlusion.bounds.radius = dist;
		i++;
	}
}

void	update_bounds_world_triangles(t_entity *entity)
{
	t_quaternion	vertices[8];

	set_vertices_from_bounds(vertices, entity->occlusion.bounds.box);
	if (entity->occlusion.world_tri_count >= 8)
		update_world_triangle_box(vertices, entity->occlusion.world_tri);
	else if (entity->occlusion.world_tri_count == 2)
		ft_memcpy(entity->occlusion.world_tri, entity->world_triangles, \
				sizeof(t_world_triangle) * entity->obj->face_count);
	else if (entity->occlusion.world_tri_count == 1)
		entity->occlusion.world_tri[0] = entity->world_triangles[0];
	update_entity_radius_and_origin(entity, vertices);
}
