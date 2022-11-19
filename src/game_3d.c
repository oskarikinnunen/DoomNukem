/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_3d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/19 16:58:11 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"
#include "objects.h"
#include "vectors.h"

static void save_clipped_points(float dist[3], t_triangle *tris[3], int points[4])
{
	if (dist[0] >= 0)
	{
		tris[1]->p[points[0]++] = tris[0]->p[0];
		tris[1]->t[points[1]++] = tris[0]->t[0];
	}
	else
	{
		tris[2]->p[points[2]++] = tris[0]->p[0];
		tris[2]->t[points[3]++] = tris[0]->t[0];
	}
	tris[1 + (dist[1] < 0)]->p[points[0 + ((dist[1] < 0) * 2)]++] = tris[0]->p[1];
	tris[1 + (dist[1] < 0)]->t[points[1 + ((dist[1] < 0) * 2)]++] = tris[0]->t[1];
	/*if (dist[1] >= 0)
	{
		tris[1]->p[points[0]++] = tris[0]->p[1];
		tris[1]->t[points[1]++] = tris[0]->t[1];
	}
	else
	{
		tris[2]->p[points[2]++] = tris[0]->p[1];
		tris[2]->t[points[3]++] = tris[0]->t[1];
	}*/
	if (dist[2] >= 0)
	{
		tris[1]->p[points[0]++] = tris[0]->p[2];
		tris[1]->t[points[1]++] = tris[0]->t[2];
	}
	else
	{
		tris[2]->p[points[2]++] = tris[0]->p[2];
		tris[2]->t[points[3]++] = tris[0]->t[2];
	}
}

static void sort_quat_tex_by_dist(float dist[3], t_quaternion q[3], t_texture t[3])
{
	int i, j;
	float			key;
	t_quaternion	temp_q;
	t_texture		temp_t;

	for (i = 1; i < 3; i++)
	{
		key = dist[i];
		temp_q = q[i];
		temp_t = t[i];
		j = i - 1;
		while (j >= 0 && dist[j] > key)
		{
			dist[j + 1] = dist[j];
			q[j + 1] = q[j];
			t[j + 1] = t[j];
			j = j - 1;
		}
		dist[j + 1] = key;
		q[j + 1] = temp_q;
		t[j + 1] = temp_t;
	}
}

static float flerp(float from, float to, float delta)
{
	return (from + ((to - from) * delta));
}

static t_texture lerp_texture(t_texture from_t, t_texture to_t, float delta)
{
	from_t.u = flerp(from_t.u, to_t.u, delta);
	from_t.v = flerp(from_t.v, to_t.v, delta);
	from_t.w = flerp(from_t.w, to_t.w, delta);

	return(from_t);
}

static int clip_triangle(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle *out_tri)
{
	float	t;

	out_tri->p[0] = quaternion_intersectplane(plane_p, plane_n, in_tri.p[0], in_tri.p[2], &t);
	out_tri->t[0] = lerp_texture(in_tri.t[0], in_tri.t[2], t);
	out_tri->p[1] = quaternion_intersectplane(plane_p, plane_n, in_tri.p[1], in_tri.p[2], &t);
	out_tri->t[1] = lerp_texture(in_tri.t[1], in_tri.t[2], t);
	out_tri->p[2] = in_tri.p[2];
	out_tri->t[2] = in_tri.t[2];

	return(1);
}

static int clip_quad_to_triangles(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle *out_tri)
{
	float	t;

	out_tri[0].p[0] = quaternion_intersectplane(plane_p, plane_n, in_tri.p[0], in_tri.p[1], &t);
	out_tri[0].t[0] = lerp_texture(in_tri.t[0], in_tri.t[1], t);
	out_tri[0].p[1] = quaternion_intersectplane(plane_p, plane_n, in_tri.p[0], in_tri.p[2], &t);
	out_tri[0].t[1] = lerp_texture(in_tri.t[0], in_tri.t[2], t);
	out_tri[0].p[2] = in_tri.p[1];
	out_tri[0].t[2] = in_tri.t[1];
	out_tri[1].p[0] = quaternion_intersectplane(plane_p, plane_n, in_tri.p[0], in_tri.p[2], &t);
	out_tri[1].t[0] = lerp_texture(in_tri.t[0], in_tri.t[2], t);
	out_tri[1].p[1] = in_tri.p[1];
	out_tri[1].t[1] = in_tri.t[1];
	out_tri[1].p[2] = in_tri.p[2];
	out_tri[1].t[2] = in_tri.t[2];

	return(2);
}

int clip_triangle_against_plane(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle out_tri[2])
{
	float		dist[3];
	int			i;
	int			outside = 0;
	bool		inside;
	float		t;

	plane_n = vector3_normalise(plane_n);
	i = 0;
	while (i < 3)
	{
		dist[i] = vector3_fdist_to_plane(in_tri.p[i].v, plane_n, plane_p);
		if (dist[i] < 0.0f)
			outside++;
		i++;
	}
	if (outside == 3)
		return(0);
	if (outside == 0)
	{
		out_tri[0] = in_tri;
		return(1);
	}
	sort_quat_tex_by_dist(dist, in_tri.p, in_tri.t);
	if (outside == 1)
		return (clip_quad_to_triangles(plane_p, plane_n, in_tri, out_tri));
	if (outside == 2)
		return (clip_triangle(plane_p, plane_n, in_tri, out_tri));
	return(0);
}

static void draw_triangles(t_sdlcontext sdl, t_render *render)
{
	//return;
	t_triangle test;
/*
	for (int i = 0; i < sdl.window_h; i++)
	{
		for (int j = 0; j < sdl.window_w; j++)
		{
			draw(sdl, (t_point){j, i}, INT_MAX);
		}
	}
	return;
	test = render->draw_triangles[0];
	test.p[0].v.x = 0;
	test.p[0].v.y = 0;
	test.p[1].v.x = sdl.window_w - 1;
	test.p[1].v.y = 0;
	test.p[2].v.x = 0;
	test.p[2].v.y = sdl.window_h - 1;
	z_fill_tri(sdl, test, *render->img);
	test = render->draw_triangles[0];
	test.p[0].v.x = sdl.window_w - 1;
	test.p[0].v.y = 0;
	test.p[1].v.x = 0;
	test.p[1].v.y = sdl.window_h - 1;
	test.p[2].v.x = sdl.window_w - 1;
	test.p[2].v.y = sdl.window_h - 1;
	z_fill_tri(sdl, test, *render->img);
	return;*/
	int index = 0;
	while (index < render->draw_tri_count)
	{
		if (render->rendermode == fill)
			z_fill_tri(sdl, render->draw_triangles[index], *render->img);
		if (render->rendermode == wireframe)
		{
			drawline(sdl, (t_point){render->draw_triangles[index].p[0].v.x, render->draw_triangles[index].p[0].v.y}, (t_point){render->draw_triangles[index].p[1].v.x, render->draw_triangles[index].p[1].v.y}, INT_MAX);
			drawline(sdl, (t_point){render->draw_triangles[index].p[2].v.x, render->draw_triangles[index].p[2].v.y}, (t_point){render->draw_triangles[index].p[1].v.x, render->draw_triangles[index].p[1].v.y}, INT_MAX);
			drawline(sdl, (t_point){render->draw_triangles[index].p[0].v.x, render->draw_triangles[index].p[0].v.y}, (t_point){render->draw_triangles[index].p[2].v.x, render->draw_triangles[index].p[2].v.y}, INT_MAX);
		}
		index++;
	}
	render->stats.triangle_count += render->draw_tri_count;
}

//TODO: temp remove after occlusion is added
static void sort_triangles(t_triangle *triangles, int high)
{
	int i, j;
	t_triangle key;
	for (i = 1; i < high; i++)
	{
		key = triangles[i];
		j = i - 1;
		while (j >= 0 && triangles[j].t[0].w < key.t[0].w)
		{
			triangles[j + 1] = triangles[j];
			j = j - 1;
		}
		triangles[j + 1] = key;
	}
}

static void clipped(t_render *render, t_sdlcontext sdl)
{
	int i = 0;
	int start = 0;
	int end = 0;

	t_triangle	triangles[200];
	t_triangle	clipped[2];
	while (i < render->calc_tri_count)
	{
		triangles[end++] = render->calc_triangles[i];
		int nnewtriangles = 1;
		for (int p = 0; p < 4; p++)
		{
			int ntristoadd = 0;
			while (nnewtriangles > 0)
			{
				t_triangle test;
				test = triangles[start++];
				nnewtriangles--;
				switch (p)
				{
				case 0: ntristoadd = clip_triangle_against_plane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){0.0f, 1.0f, 0.0f}, test, clipped); break;
				case 1: ntristoadd = clip_triangle_against_plane((t_vector3){0.0f, (float)sdl.window_h - 1.0f, 0.0f}, (t_vector3){0.0f, -1.0f, 0.0f}, test, clipped); break;
				case 2: ntristoadd = clip_triangle_against_plane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){1.0f, 0.0f, 0.0f}, test, clipped); break;
				case 3: ntristoadd = clip_triangle_against_plane((t_vector3){(float)sdl.window_w - 1.0f, 0.0f, 0.0f}, (t_vector3){-1.0f, 0.0f, 0.0f}, test, clipped); break;
				}
				for (int w = 0; w < ntristoadd; w++)
				{
					triangles[end++] = clipped[w];
				}
			}
			nnewtriangles = end - start;
		}
		while (start < end)
		{
			render->draw_triangles[render->draw_tri_count++] = triangles[start++];
		}
		start = 0;
		end = 0;
		i++;
	}
	draw_triangles(sdl, render);
}

static t_triangle transform_calc(t_mat4x4 matworld, t_triangle triangles)
{
	t_triangle tritransformed;

	tritransformed.p[0] = quaternion_mul_matrix(matworld, triangles.p[0]);
	tritransformed.p[1] = quaternion_mul_matrix(matworld, triangles.p[1]);
	tritransformed.p[2] = quaternion_mul_matrix(matworld, triangles.p[2]);
	tritransformed.t[0] = triangles.t[0];
	tritransformed.t[1] = triangles.t[1];
	tritransformed.t[2] = triangles.t[2];
	tritransformed.clr = triangles.clr;

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

static int clippedtriangles(t_triangle tritransformed, t_mat4x4 matview, t_triangle *clipped)
{
	t_triangle triviewed;

	triviewed.p[0] = quaternion_mul_matrix(matview, tritransformed.p[0]);
	triviewed.p[1] = quaternion_mul_matrix(matview, tritransformed.p[1]);
	triviewed.p[2] = quaternion_mul_matrix(matview, tritransformed.p[2]);
	triviewed.t[0] = tritransformed.t[0];
	triviewed.t[1] = tritransformed.t[1];
	triviewed.t[2] = tritransformed.t[2];
	return (clip_triangle_against_plane(
	(t_vector3){0.0f, 0.0f, 0.1f},
	(t_vector3){0.0f, 0.0f, 0.2f},
	triviewed, clipped));
}

static t_triangle triangle_to_screenspace(t_mat4x4 matproj, t_triangle clipped, t_sdlcontext sdl)
{
	t_triangle triprojected;

	triprojected.p[0] = quaternion_mul_matrix(matproj, clipped.p[0]);
	triprojected.p[1] = quaternion_mul_matrix(matproj, clipped.p[1]);
	triprojected.p[2] = quaternion_mul_matrix(matproj, clipped.p[2]);
	triprojected.t[0] = clipped.t[0];
	triprojected.t[1] = clipped.t[1];
	triprojected.t[2] = clipped.t[2];

	triprojected.t[0].u = triprojected.t[0].u / triprojected.p[0].w;
	triprojected.t[1].u = triprojected.t[1].u / triprojected.p[1].w;
	triprojected.t[2].u = triprojected.t[2].u / triprojected.p[2].w;

	triprojected.t[0].v = triprojected.t[0].v / triprojected.p[0].w;
	triprojected.t[1].v = triprojected.t[1].v / triprojected.p[1].w;
	triprojected.t[2].v = triprojected.t[2].v / triprojected.p[2].w;

	triprojected.t[0].w = 1.0f / triprojected.p[0].w;
	triprojected.t[1].w = 1.0f / triprojected.p[1].w;
	triprojected.t[2].w = 1.0f / triprojected.p[2].w;
	
	triprojected.p[0].v = vector3_div(triprojected.p[0].v, triprojected.p[0].w);
	triprojected.p[1].v = vector3_div(triprojected.p[1].v, triprojected.p[1].w);
	triprojected.p[2].v = vector3_div(triprojected.p[2].v, triprojected.p[2].w);

	triprojected.p[0].v = vector3_negative(triprojected.p[0].v);
	triprojected.p[1].v = vector3_negative(triprojected.p[1].v);
	triprojected.p[2].v = vector3_negative(triprojected.p[2].v);

	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	triprojected.p[0].v = vector3_add(triprojected.p[0].v, voffsetview);
	triprojected.p[1].v = vector3_add(triprojected.p[1].v, voffsetview);
	triprojected.p[2].v = vector3_add(triprojected.p[2].v, voffsetview);

	triprojected.p[0].v.x *= 0.5f * (float)sdl.window_w;
	triprojected.p[0].v.y *= 0.5f * (float)sdl.window_h;
	triprojected.p[1].v.x *= 0.5f * (float)sdl.window_w;
	triprojected.p[1].v.y *= 0.5f * (float)sdl.window_h;
	triprojected.p[2].v.x *= 0.5f * (float)sdl.window_w;
	triprojected.p[2].v.y *= 0.5f * (float)sdl.window_h;

	return(triprojected);
}

static t_texture	vector2_to_texture(t_vector2 v)
{
	t_texture t;
	t.u = v.x;
	t.v = v.y;
	t.w = 1.0f;
	return(t);
}

void render_entity(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	int				index;
	t_object		*obj;
	t_vector3		temp;


	obj = entity->obj;
	render->calc_tri_count = 0;
	render->draw_tri_count = 0;
	if (obj == NULL)
		return;
	index = 0;
	while (index < obj->vertice_count)
	{
		//TODO: Matrix rotations
		temp = vector3_mul_vector3(entity->transform.scale, obj->vertices[index]);
		temp = vector3_add(entity->transform.location, temp);
		render->q[index] = vector3_to_quaternion(temp);
		render->q[index] = quaternion_mul_matrix(render->matworld, render->q[index]);
		index++;
	}
	index = 0;
	while (index < obj->face_count)
	{
		t_triangle	tritransformed;
		t_vector3	normal;	
		t_vector3	vcameraray;	

		tritransformed = (t_triangle){render->q[obj->faces[index].v_indices[0] - 1], render->q[obj->faces[index].v_indices[1] - 1], render->q[obj->faces[index].v_indices[2] - 1]};
		if (obj->uv_count != 0 && render->rendermode != wireframe)
		{
			tritransformed.t[0] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[0] - 1]);
			tritransformed.t[1] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[1] - 1]);
			tritransformed.t[2] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[2] - 1]);
		}
		normal = normal_calc(tritransformed);
		vcameraray = vector3_sub(tritransformed.p[0].v, render->position);
		if (vector3_dot(normal, vcameraray) < 0.0f || 1) //TODO: Currently ignoring normals with || 1
		{
			t_triangle clipped[2];
			int nclippedtriangles = clippedtriangles(tritransformed, render->matview, clipped);
			for (int n = 0; n < nclippedtriangles; n++)
			{
				render->calc_triangles[render->calc_tri_count++] = triangle_to_screenspace(render->matproj, clipped[n], sdl);
			}
		}
		index++;
	}
	if (obj->material_count != 0)
		render->img = obj->materials[0].img;
	if (!render->img)
		render->img = render->debug_img;
	clipped(render, sdl);
	render->stats.render_count++;
}

void render_ray(t_sdlcontext sdl, t_render render, t_vector3 from, t_vector3 to)
{
	render.q[0] = vector3_to_quaternion(from);
	render.q[0] = quaternion_to_screenspace(render, render.q[0], sdl);
	render.q[1] = vector3_to_quaternion(to);
	render.q[1] = quaternion_to_screenspace(render, render.q[1], sdl);
	drawline(sdl,
		(t_point) {render.q[0].v.x, render.q[0].v.y},
		(t_point) {render.q[1].v.x, render.q[1].v.y}, render.gizmocolor);
		
	render.calc_tri_count = 0;
	render.draw_tri_count = 0;
}

void render_gizmo(t_sdlcontext sdl, t_render render, t_vector3 pos, int size)
{
	t_object		obj;
	t_vector3		vertex;

	obj.vertice_count = 1;
	vertex = vector3_zero();
	obj.vertices = &vertex;
	//vertex = vector3_mul_vector3(entity->transform.scale, vertex);
	vertex = pos;
	render.q[0] = vector3_to_quaternion(vertex);
	render.q[0] = quaternion_to_screenspace(render, render.q[0], sdl);
	if (render.q[0].w > 0.0f
		&& render.q[0].v.x >= 0.0f && render.q[0].v.x < sdl.window_w
		&& render.q[0].v.y >= 0.0f && render.q[0].v.y < sdl.window_h)
		drawcircle(sdl, (t_point){render.q[0].v.x, render.q[0].v.y}, size, render.gizmocolor);
	render.calc_tri_count = 0;
	render.draw_tri_count = 0;
}

static t_vector3 lookdirection2(t_vector2 angle)
{
	t_quaternion	temp;
	t_mat4x4		matcamerarot;

	matcamerarot = matrix_makerotationz(angle.x);
	temp = quaternion_mul_matrix(matcamerarot, (t_quaternion){1.0f, 0.0f, 0.0f, 1.0f});

	return (temp.v);
}

//TODO: this don't work
void	draw_screen_to_worldspace_ray(t_sdlcontext sdl, t_render render, t_point origin, t_vector2 og_angle)
{
	t_vector3	curstep;
	t_vector3	dir;
	t_vector2	angle;
	t_entity	ent;
	int	debug_iter;
	

	curstep = render.position;
	angle.y = (((sdl.window_h / 2) - (float)origin.y) / (sdl.window_h / 2.0f)); //[1][1] should be FOV in radians;
	angle.y = angle.y * ft_degtorad(45.0f);

	angle.x = (((sdl.window_w / 2) - (float)origin.x) / (sdl.window_w / 2.0f));
	angle.x = angle.x * (ft_degtorad(45.0f) * ((float)sdl.window_w / (float)sdl.window_h));
	//angle.x = 0.0f;
	//angle.x = 0;
	dir = vector3_zero();
	dir.y = -lookdirection((t_vector2){og_angle.x, og_angle.y + angle.y}).y;
	dir.x = -lookdirection2((t_vector2){og_angle.x + angle.x, og_angle.y + angle.y}).x;
	dir.y *= render.position.z * 1.4f;
	dir.x *= render.position.z * ((float)sdl.window_w / (float)sdl.window_h);
	dir.z = render.position.z;
	/*dir.x *= 2.0f;
	dir.y *= 2.0f;
	dir.z = 2.0f;*/ //WORKS
	debug_iter = 0;
	while (curstep.z > 0.0f && debug_iter < 400)
	{
		curstep = vector3_movetowards(curstep, dir, 0.01f);
		ent.transform.location = curstep;
		ent.transform.scale = vector3_one();
		//render_gizmo(sdl, render, &ent);
		debug_iter++;
	}
}