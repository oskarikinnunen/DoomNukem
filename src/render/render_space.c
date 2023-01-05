/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 15:36:10 by vlaine            #+#    #+#             */
/*   Updated: 2023/01/05 16:16:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void render_worldspace(t_render *render, t_entity *entity)
{
	t_object		*obj;
	t_quaternion	temp;
	int				index;

	obj = entity->obj;
	if (obj == NULL)
		return;
	index = 0;
	while (index < obj->vertice_count)
	{
		temp.v = obj->vertices[index];
		if (entity->animation.active)
		{
			temp.v = vector3_add(entity->obj->o_anim.frames[entity->animation.frame].deltavertices[index].delta, temp.v);
		}
		render->q[index] = transformed_vector3(entity->transform, temp.v);
		render->q[index] = quaternion_mul_matrix(render->camera.matworld, render->q[index]);
		index++;
	}
}

t_triangle	triangle_to_viewspace(t_triangle tritransformed, t_mat4x4 matview)
{
	tritransformed.p[0] = quaternion_mul_matrix(matview, tritransformed.p[0]);
	tritransformed.p[1] = quaternion_mul_matrix(matview, tritransformed.p[1]);
	tritransformed.p[2] = quaternion_mul_matrix(matview, tritransformed.p[2]);
	tritransformed.t[0] = tritransformed.t[0];
	tritransformed.t[1] = tritransformed.t[1];
	tritransformed.t[2] = tritransformed.t[2];
	tritransformed.clr = tritransformed.clr;
	return(tritransformed);
}

static t_vector3 normal_calc(t_triangle tritransformed)
{
	t_vector3 normal, line1, line2;

	line1 = vector3_sub(tritransformed.p[1].v, tritransformed.p[0].v);
	line2 = vector3_sub(tritransformed.p[2].v, tritransformed.p[0].v);
	normal = vector3_crossproduct(line1, line2);
	normal = vector3_normalise(normal);

	return (normal);
}

static bool is_triangle_backface(t_triangle tritransformed, t_render *render)
{
	t_vector3	normal;	
	t_vector3	vcameraray;
	
	normal = normal_calc(tritransformed);
	vcameraray = vector3_sub(tritransformed.p[0].v, render->camera.position);
	if (vector3_dot(normal, vcameraray) < 0.0f || 1)
		return(false);
	else
		return(true);
}

t_point_triangle triangle_to_screenspace_point_triangle(t_mat4x4 matproj, t_triangle clipped, t_sdlcontext sdl)
{
	t_triangle			triprojected;
	t_point_triangle	tri;
	int					i;
	t_vector3			voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};

	i = 0;
	while (i < 3)
	{
		triprojected.p[i] = quaternion_mul_matrix(matproj, clipped.p[i]);
		triprojected.t[i] = clipped.t[i];
		tri.t[i].u = triprojected.t[i].u / triprojected.p[i].w;
		tri.t[i].v = triprojected.t[i].v / triprojected.p[i].w;
		tri.t[i].w = 1.0f / triprojected.p[i].w; //1.0f /
		triprojected.p[i].v = vector3_div(triprojected.p[i].v, triprojected.p[i].w);
		triprojected.p[i].v = vector3_negative(triprojected.p[i].v);
		triprojected.p[i].v = vector3_add(triprojected.p[i].v, voffsetview);
		tri.p[i].x = triprojected.p[i].v.x * (0.5f * (float)(sdl.window_w * sdl.resolution_scaling));
		tri.p[i].y = triprojected.p[i].v.y * (0.5f * (float)(sdl.window_h * sdl.resolution_scaling));
		i++;
	}
	tri.clr = clipped.clr;
	return(tri);
}

static void clip_and_render_triangles(t_sdlcontext *sdl, t_render *render)
{
	clipped_point_triangle(render, *sdl);
	render_buffer(sdl, render);
	render->worldspace_ptri_count = 0;
	render->screenspace_ptri_count = 0;
	render->map.data = NULL;
	render->img = NULL;
}

void render_quaternions(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	t_object		*obj;
	t_quaternion	temp;
	int				index;

	obj = entity->obj;
	render->worldspace_ptri_count = 0;
	render->screenspace_ptri_count = 0;

	index = 0;
	while (index < obj->face_count)
	{
		t_triangle	tritransformed;

		tritransformed = (t_triangle){render->q[obj->faces[index].v_indices[0] - 1], render->q[obj->faces[index].v_indices[1] - 1], render->q[obj->faces[index].v_indices[2] - 1]};
		if (obj->uv_count != 0 && !render->wireframe)
		{
			tritransformed.t[0] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[0] - 1]);
			tritransformed.t[1] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[1] - 1]);
			tritransformed.t[2] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[2] - 1]);
		}
		tritransformed.clr = obj->materials[obj->faces[index].materialindex].kd;
		if (!is_triangle_backface(tritransformed, render))
		{
			t_triangle clipped[2];
			tritransformed = triangle_to_viewspace(tritransformed, render->camera.matview);
			int nclippedtriangles = clip_triangle_against_plane((t_vector3){.z = 0.1f}, (t_vector3){.z = 1.0f}, tritransformed, clipped);
			for (int n = 0; n < nclippedtriangles; n++)
				render->worldspace_ptris[render->worldspace_ptri_count++] = triangle_to_screenspace_point_triangle(render->camera.matproj, clipped[n], *sdl);
		}
		if (index + 1 == obj->face_count || obj->faces[index].materialindex != obj->faces[index + 1].materialindex)
		{
			render->lightmode = lm_unlit;
			render->img = obj->materials[obj->faces[index].materialindex].img;
			if (entity->map != NULL && sdl->lighting_toggled)
			{
				render->map = entity->map[obj->faces[index].materialindex];
				render->map.img_size = point_sub(render->map.img_size, (t_point){1, 1});
				render->lightmode = lm_lit;
			}
			clip_and_render_triangles(sdl, render);
		}
		index++;
	}
	render->rs.render_count++;
}
