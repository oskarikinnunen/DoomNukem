/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_raycast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:47:07 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 13:41:08 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "objects.h"
#include "doomnukem.h"
#include "editor_tools.h"

typedef struct s_raynorminette
{
	t_vector3	e1;
	t_vector3	e2;
	t_vector3	n;
	t_vector3	result;
	t_vector3	ao;
	t_vector3	dao;
}	t_raytridata;
//	abc = tri to check against
//	w = distance of ray hit
// disable inverse normal hitting by removing fabs

bool	raycast_tri(t_ray r, t_vector3_tri tri, float *dist)
{
	t_raytridata	rd;
	float			det;
	float			invdet;
	bool			result_bool;

	rd.e1 = vector3_sub(tri.b, tri.a);
	rd.e2 = vector3_sub(tri.c, tri.a);
	rd.n = vector3_cross_product(rd.e1, rd.e2);
	det = -vector3_dot(r.dir, rd.n);
	invdet = 1.0f / det;
	rd.ao = vector3_sub(r.origin, tri.a);
	rd.dao = vector3_cross_product(rd.ao, r.dir);
	rd.result.x = vector3_dot(rd.e2, rd.dao) * invdet;
	rd.result.y = -vector3_dot(rd.e1, rd.dao) * invdet;
	rd.result.z = vector3_dot(rd.ao, rd.n) * invdet;
	result_bool = (*dist > rd.result.z
			&& fabs(det) >= 1e-6 && rd.result.z >= -0.01f && rd.result.x >= 0.0f
			&& rd.result.y >= 0.0f && (rd.result.x + rd.result.y) <= 1.0f);
	if (result_bool)
		*dist = rd.result.z;
	return (result_bool);
}

t_vector3	get_anim_vert(t_entity *entity, int index)
{
	return (entity->obj->o_anim.frames[entity->animation.frame]
		.deltavertices[index].delta);
}

t_vector3_tri	worldspace_tri(struct s_entity *entity, int index)
{
	t_vector3_tri	tri;
	t_object		*object;
	int				i1;
	int				i2;
	int				i3;

	object = entity->obj;
	if (object == NULL)
		return ((t_vector3_tri){0});
	i1 = object->faces[index].v_indices[0] - 1;
	i2 = object->faces[index].v_indices[1] - 1;
	i3 = object->faces[index].v_indices[2] - 1;
	tri.a = object->vertices[i1];
	tri.b = object->vertices[i2];
	tri.c = object->vertices[i3];
	if (entity->animation.active)
	{
		tri.a = vector3_add(get_anim_vert(entity, i1), tri.a);
		tri.b = vector3_add(get_anim_vert(entity, i2), tri.b);
		tri.c = vector3_add(get_anim_vert(entity, i3), tri.c);
	}
	tri.a = transformed_vector3(entity->transform, tri.a).v;
	tri.b = transformed_vector3(entity->transform, tri.b).v;
	tri.c = transformed_vector3(entity->transform, tri.c).v;
	return (tri);
}

static t_vector3_tri	worldtri_to_v3tri(t_world_triangle wt)
{
	t_vector3_tri	tri;

	tri.a = wt.p[0].v;
	tri.b = wt.p[1].v;
	tri.c = wt.p[2].v;
	return (tri);
}

bool	raycast_entity(t_ray r, t_raycast_info *info, t_entity *entity)
{
	int					i;
	bool				hit;
	t_vector3_tri		tri;
	t_world_triangle	tt;

	i = 0;
	hit = false;
	if (entity->ignore_raycasts || entity->obj == NULL \
	|| entity->world_triangles == NULL)
		return (hit);
	while (i < entity->obj->face_count)
	{
		tri = worldtri_to_v3tri(entity->world_triangles[i]);
		if (raycast_tri(r, tri, &info->distance))
		{
			info->hit_pos = vector3_add(r.origin,
					vector3_mul(r.dir, info->distance));
			info->hit_entity = entity;
			info->face_normal
				= normal_calc_quaternion(entity->world_triangles[i].p);
			hit = true;
		}
		i++;
	}
	return (hit);
}
