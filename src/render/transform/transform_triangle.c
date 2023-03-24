/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_triangle.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:49:46 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/23 21:40:04 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

t_world_triangle triangle_to_viewspace(t_world_triangle tritransformed, t_mat4x4 matview)
{
	tritransformed.p[0] = quaternion_mul_matrix(matview, tritransformed.p[0]);
	tritransformed.p[1] = quaternion_mul_matrix(matview, tritransformed.p[1]);
	tritransformed.p[2] = quaternion_mul_matrix(matview, tritransformed.p[2]);
	tritransformed.t[0] = tritransformed.t[0];
	tritransformed.t[1] = tritransformed.t[1];
	tritransformed.t[2] = tritransformed.t[2];
	return (tritransformed);
}

t_screen_triangle world_triangle_to_screen_triangle(t_mat4x4 matproj, t_world_triangle clipped, t_sdlcontext sdl)
{
	t_world_triangle triprojected;
	t_screen_triangle tri;
	int i;
	const t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};

	i = 0;
	while (i < 3)
	{
		triprojected.p[i] = quaternion_mul_matrix(matproj, clipped.p[i]);
		triprojected.t[i] = clipped.t[i];
		tri.t[i].x = triprojected.t[i].x / triprojected.p[i].w;
		tri.t[i].y = triprojected.t[i].y / triprojected.p[i].w;
		tri.t[i].z = 1.0f / triprojected.p[i].w;
		triprojected.p[i].v = vector3_div(triprojected.p[i].v, triprojected.p[i].w);
		triprojected.p[i].v = vector3_negative(triprojected.p[i].v);
		triprojected.p[i].v = vector3_add(triprojected.p[i].v, voffsetview);
		tri.p[i].x = triprojected.p[i].v.x * (0.5f * (float)(sdl.window_w * sdl.resolution_scaling));
		tri.p[i].y = triprojected.p[i].v.y * (0.5f * (float)(sdl.window_h * sdl.resolution_scaling));
		i++;
	}
	return (tri);
}

void calculate_triangles_for_entity(t_entity *entity, t_world *world)
{
	t_world_triangle		tritransformed;
	t_render		*render;
	t_object		*obj;
	t_vector2		uv;
	int				i;
	int				index;

	obj = entity->obj;
	if (obj == NULL)
		return ;
	render = &world->sdl->render;
	if (calculate_quaternion_buffer_for_entity(render->q, entity) == false)
		return ;
	i = 0;
	while (i < obj->face_count)
	{
		index = obj->faces[i].materialindex;
		tritransformed = (t_world_triangle){render->q[obj->faces[i].v_indices[0] - 1], render->q[obj->faces[i].v_indices[1] - 1], render->q[obj->faces[i].v_indices[2] - 1]};
		if (obj->uv_count != 0)
		{
			for (int e = 0; e < 3; e++)
			{
				uv = obj->uvs[obj->faces[i].uv_indices[e] - 1];
				uv.x = truncf(uv.x * (float)(entity->obj->materials[index].img->size.x)) + 0.5f;
				uv.y = truncf(uv.y * (float)(entity->obj->materials[index].img->size.y)) + 0.5f;
				tritransformed.t[e] = (t_vector3){uv.x, uv.y, 1.0f};
			}
		}
		entity->world_triangles[i] = tritransformed;
		i++;
	}
}
