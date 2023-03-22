/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 15:36:10 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/14 14:04:48 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

inline static void	update_world_triangle_box(t_quaternion *transformed, t_triangle *world_triangles)
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

inline static void	update_world_triangle_plane(t_quaternion *transformed, t_triangle *world_triangles)
{
	world_triangles[0].p[0] = transformed[0];
	world_triangles[0].p[1] = transformed[2];
	world_triangles[0].p[2] = transformed[3];
	world_triangles[1].p[0] = transformed[0];
	world_triangles[1].p[1] = transformed[1];
	world_triangles[1].p[2] = transformed[3];
}

//rename function and enum that has the same name
inline static void	update_world_triangle_ignore(t_quaternion *transformed, t_triangle *world_triangles)
{
	world_triangles[0] = (t_triangle){.p[0] = transformed[0],.p[1] = transformed[1],.p[2] = transformed[2]};
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

void render_worldspace(t_render *render, t_entity *entity)
{
	t_object *obj;
	int index;
	t_mat4x4 matworld;

	obj = entity->obj;
	if (obj == NULL) // TODO: Is this needed?
		return;
	matworld = make_transform_matrix(entity->transform);
	index = 0;
	while (index < obj->vertice_count)
	{
		render->q[index] = vector3_to_quaternion(obj->vertices[index]);
		if (entity->animation.active)
			render->q[index].v = vector3_add(entity->obj->o_anim.frames[entity->animation.frame].deltavertices[index].delta, render->q[index].v);
		// render->q[index] = quaternion_mul_matrix(matworld, render->q[index]);
		render->q[index] = transformed_vector3(entity->transform, render->q[index].v);
		index++;
	}
	update_bounds_world_triangles(entity, matworld);
}

t_triangle triangle_to_viewspace(t_triangle tritransformed, t_mat4x4 matview)
{
	tritransformed.p[0] = quaternion_mul_matrix(matview, tritransformed.p[0]);
	tritransformed.p[1] = quaternion_mul_matrix(matview, tritransformed.p[1]);
	tritransformed.p[2] = quaternion_mul_matrix(matview, tritransformed.p[2]);
	tritransformed.t[0] = tritransformed.t[0];
	tritransformed.t[1] = tritransformed.t[1];
	tritransformed.t[2] = tritransformed.t[2];
	tritransformed.clr = tritransformed.clr;
	return (tritransformed);
}

static t_vector3	normal_calc(t_triangle tritransformed)
{
	t_vector3 normal, line1, line2;

	line1 = vector3_sub(tritransformed.p[1].v, tritransformed.p[0].v);
	line2 = vector3_sub(tritransformed.p[2].v, tritransformed.p[0].v);
	normal = vector3_crossproduct(line1, line2);
	normal = vector3_normalise(normal);

	return (normal);
}

bool is_triangle_backface(t_triangle tritransformed, t_render *render)
{
	t_vector3 normal;
	t_vector3 vcameraray;

	normal = normal_calc(tritransformed);
	vcameraray = vector3_sub(tritransformed.p[0].v, render->camera.position);
	if (vector3_dot(normal, vcameraray) < 0.0f || 1)
		return (false);
	else
		return (true);
}

t_point_triangle triangle_to_screenspace_point_triangle(t_mat4x4 matproj, t_triangle clipped, t_sdlcontext sdl)
{
	t_triangle triprojected;
	t_point_triangle tri;
	int i;
	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};

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
	tri.clr = clipped.clr;
	return (tri);
}

static void clip_and_render_triangles(t_sdlcontext *sdl, t_render *render)
{
	clipped_point_triangle(render, *sdl);
	render_buffer(sdl, render);
	render->screenspace_ptri_count = 0;
	render->map.texture = NULL;
	render->img = NULL;
}

void render_quaternions(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	t_triangle		world_tri;
	t_object		*obj;
	t_quaternion	temp;
	int				i;
	int				index;

	if (entity->world_triangles == NULL || entity->obj == NULL)
	{
		//doomlog(LOG_WARNING, "entity->worldtriangles == null in render_quaternions\n!	printing entity->object_name.str and entity->obj->name");
		//doomlog(LOG_WARNING, entity->object_name.str);
		//doomlog(LOG_WARNING, entity->obj->name);
		return;
	}
	obj = entity->obj;
	render->world_triangles = entity->world_triangles;
	i = 0;
	render->start_index = 0;
	while (i < obj->face_count)
	{
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || index != obj->faces[i + 1].materialindex)
		{
			render->lightmode = lm_unlit;
			render->img = obj->materials[index].img;
			if (entity->map != NULL && sdl->lighting_toggled)
			{
				render->map = entity->map[index];
				//render->map.img_size = render->img->size;
				//render->map.size = point_sub(render->map.size, (t_point){1, 1});
				render->lightmode = lm_lit;
			}
			render->end_index = i;
			clip_and_render_triangles(sdl, render);
			render->start_index = i + 1;
		}
		i++;
	}
	render->world_triangles = NULL; // should be unnecessary
	render->rs.render_count++;
}
