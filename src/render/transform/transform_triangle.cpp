/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_triangle.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:49:46 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/25 13:00:34 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

t_world_triangle	triangle_to_viewspace(
	t_world_triangle tritransformed, t_mat4x4 matview)
{
	tritransformed.p[0] = quaternion_mul_matrix(matview, tritransformed.p[0]);
	tritransformed.p[1] = quaternion_mul_matrix(matview, tritransformed.p[1]);
	tritransformed.p[2] = quaternion_mul_matrix(matview, tritransformed.p[2]);
	tritransformed.t[0] = tritransformed.t[0];
	tritransformed.t[1] = tritransformed.t[1];
	tritransformed.t[2] = tritransformed.t[2];
	return (tritransformed);
}

t_screen_triangle	world_triangle_to_screen_triangle(
	t_mat4x4 matproj, t_world_triangle clipped, t_sdlcontext sdl)
{
	t_world_triangle	tri_proj;
	t_screen_triangle	tri;
	int					i;
	const t_vector3		voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	t_vector2			window_scale;

	window_scale.x = 0.5f * (float)sdl.window_w * sdl.resolution_scaling;
	window_scale.y = 0.5f * (float)sdl.window_h * sdl.resolution_scaling;
	i = 0;
	while (i < 3)
	{
		tri_proj.p[i] = quaternion_mul_matrix(matproj, clipped.p[i]);
		tri_proj.t[i] = clipped.t[i];
		tri.t[i].x = tri_proj.t[i].x / tri_proj.p[i].w;
		tri.t[i].y = tri_proj.t[i].y / tri_proj.p[i].w;
		tri.t[i].z = 1.0f / tri_proj.p[i].w;
		tri_proj.p[i].v = vector3_div(tri_proj.p[i].v, tri_proj.p[i].w);
		tri_proj.p[i].v = vector3_negative(tri_proj.p[i].v);
		tri_proj.p[i].v = vector3_add(tri_proj.p[i].v, voffsetview);
		tri.p[i].x = tri_proj.p[i].v.x * window_scale.x;
		tri.p[i].y = tri_proj.p[i].v.y * window_scale.y;
		i++;
	}
	return (tri);
}

static inline void	set_world_triangles_quats(
	t_quaternion q[3], t_quaternion *buffer, uint32_t v_indices[3])
{
	int	i;

	i = 0;
	while (i < 3)
	{
		q[i] = buffer[v_indices[i] - 1];
		i++;
	}
}

static inline void	set_world_triangles_uvs(
	t_vector3 t[3], t_object *obj, int index, int i)
{
	t_vector2	uv;
	int			e;

	e = 0;
	while (e < 3)
	{
		uv = obj->uvs[obj->faces[i].uv_indices[e] - 1];
		uv.x = truncf(uv.x * (float)(obj->materials[index].img->size.x)) + 0.5f;
		uv.y = truncf(uv.y * (float)(obj->materials[index].img->size.y)) + 0.5f;
		t[e] = (t_vector3){uv.x, uv.y, 1.0f};
		e++;
	}
}

void	calculate_triangles_for_entity(
	t_entity *entity, t_world *world)
{
	t_render			*render;
	t_object			*obj;
	t_vector2			uv;
	int					i;

	obj = entity->obj;
	render = &world->sdl->render;
	if (obj == NULL || obj->uv_count == 0 || \
	!calculate_quat_buffer_for_entity(render->q, entity))
		return ;
	i = 0;
	while (i < obj->face_count)
	{
		set_world_triangles_quats(
			entity->world_triangles[i].p, render->q, obj->faces[i].v_indices);
		set_world_triangles_uvs(
			entity->world_triangles[i].t, obj, obj->faces[i].material_index, i);
		i++;
	}
}
