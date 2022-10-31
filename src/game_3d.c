/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_3d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/31 16:27:06 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"
#include "objects.h"

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
	if (dist[1] >= 0)
	{
		tris[1]->p[points[0]++] = tris[0]->p[1];
		tris[1]->t[points[1]++] = tris[0]->t[1];
	}
	else
	{
		tris[2]->p[points[2]++] = tris[0]->p[1];
		tris[2]->t[points[3]++] = tris[0]->t[1];
	}
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

static int Triangle_ClipAgainstPlane(t_vector3 plane_p, t_vector3 plane_n, t_triangle *in_tri, t_triangle out_tri[2])
{
	t_triangle	tri_inside;
	t_triangle	tri_outside;
	float		dist[3];
	int			points[4];
	float		t;

	bzero(points, sizeof(int[4]));
	plane_n = vector3_normalise(plane_n);
	dist[0] = vector3_fdist_to_plane(in_tri->p[0].v, plane_n, plane_p);
	dist[1] = vector3_fdist_to_plane(in_tri->p[1].v, plane_n, plane_p);
	dist[2] = vector3_fdist_to_plane(in_tri->p[2].v, plane_n, plane_p);

	save_clipped_points(dist, (t_triangle *[3]){in_tri, &tri_inside, &tri_outside}, points);
	if (points[0] == 0)
		return 0;
	if (points[0] == 3)
	{
		out_tri[0] = *in_tri;
		return 1;
	}
	if (points[0] == 1 && points[2] == 2)
	{
		out_tri[0].clr = in_tri->clr;

		out_tri[0].p[0] = tri_inside.p[0];
		out_tri[0].t[0] = tri_inside.t[0];

		out_tri[0].p[1] = quaternion_intersectplane(plane_p, plane_n, tri_inside.p[0], tri_outside.p[0], &t);
		out_tri[0].t[1].u = t * (tri_outside.t[0].u - tri_inside.t[0].u) + tri_inside.t[0].u;
		out_tri[0].t[1].v = t * (tri_outside.t[0].v - tri_inside.t[0].v) + tri_inside.t[0].v;
		out_tri[0].t[1].w = t * (tri_outside.t[0].w - tri_inside.t[0].w) + tri_inside.t[0].w;

		out_tri[0].p[2] = quaternion_intersectplane(plane_p, plane_n, tri_inside.p[0], tri_outside.p[1], &t);
		out_tri[0].t[2].u = t * (tri_outside.t[1].u - tri_inside.t[0].u) + tri_inside.t[0].u;
		out_tri[0].t[2].v = t * (tri_outside.t[1].v - tri_inside.t[0].v) + tri_inside.t[0].v;
		out_tri[0].t[2].w = t * (tri_outside.t[1].w - tri_inside.t[0].w) + tri_inside.t[0].w;
		return 1;
	}
	if (points[0] == 2 && points[2] == 1)
	{
		out_tri[0].clr = in_tri->clr;
		out_tri[1].clr = in_tri->clr;

		out_tri[0].p[0] = tri_inside.p[0];
		out_tri[0].p[1] = tri_inside.p[1];
		out_tri[0].t[0] = tri_inside.t[0];
		out_tri[0].t[1] = tri_inside.t[1];

		out_tri[0].p[2] = quaternion_intersectplane(plane_p, plane_n, tri_inside.p[0], tri_outside.p[0], &t);
		out_tri[0].t[2].u = t * (tri_outside.t[0].u - tri_inside.t[0].u) + tri_inside.t[0].u;
		out_tri[0].t[2].v = t * (tri_outside.t[0].v - tri_inside.t[0].v) + tri_inside.t[0].v;
		out_tri[0].t[2].w = t * (tri_outside.t[0].w - tri_inside.t[0].w) + tri_inside.t[0].w;

		out_tri[1].p[0] = tri_inside.p[1];
		out_tri[1].t[0] = tri_inside.t[1];
		out_tri[1].p[1] = out_tri[0].p[2];
		out_tri[1].t[1] = out_tri[0].t[2];
		out_tri[1].p[2] = quaternion_intersectplane(plane_p, plane_n, tri_inside.p[1], tri_outside.p[0], &t);
		out_tri[1].t[2].u = t * (tri_outside.t[0].u - tri_inside.t[1].u) + tri_inside.t[1].u;
		out_tri[1].t[2].v = t * (tri_outside.t[0].v - tri_inside.t[1].v) + tri_inside.t[1].v;
		out_tri[1].t[2].w = t * (tri_outside.t[0].w - tri_inside.t[1].w) + tri_inside.t[1].w;

		return 2;
	}
	return(0);
}

static void draw_triangles(t_sdlcontext sdl, t_render render)
{
	int index = 0;
	while (index < render.draw_tri_count)
	{
		z_fill_tri(sdl, render.draw_triangles[index], *render.img);
		drawline(sdl, (t_point){render.draw_triangles[index].p[0].v.x, render.draw_triangles[index].p[0].v.y}, (t_point){render.draw_triangles[index].p[1].v.x, render.draw_triangles[index].p[1].v.y}, INT_MAX);
		drawline(sdl, (t_point){render.draw_triangles[index].p[2].v.x, render.draw_triangles[index].p[2].v.y}, (t_point){render.draw_triangles[index].p[1].v.x, render.draw_triangles[index].p[1].v.y}, INT_MAX);
		drawline(sdl, (t_point){render.draw_triangles[index].p[0].v.x, render.draw_triangles[index].p[0].v.y}, (t_point){render.draw_triangles[index].p[2].v.x, render.draw_triangles[index].p[2].v.y}, INT_MAX);
		index++;
	}
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

static void clipped(t_render render, t_sdlcontext sdl)
{
	int i = 0;
	int start = 0;
	int end = 0;

	t_triangle	triangles[200];
	t_triangle	clipped[2];

	while (i < render.calc_tri_count)
	{
		triangles[end++] = render.calc_triangles[i];
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
				case 0: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){0.0f, 1.0f, 0.0f}, &test, clipped); break;
				case 1: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, (float)sdl.window_h - 1.0f, 0.0f}, (t_vector3){0.0f, -1.0f, 0.0f}, &test, clipped); break;
				case 2: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){1.0f, 0.0f, 0.0f}, &test, clipped); break;
				case 3: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){(float)sdl.window_w - 1.0f, 0.0f, 0.0f}, (t_vector3){-1.0f, 0.0f, 0.0f}, &test, clipped); break;
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
			render.draw_triangles[render.draw_tri_count++] = triangles[start++];
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
	return (Triangle_ClipAgainstPlane(
	(t_vector3){0.0f, 0.0f, 0.2f},
	(t_vector3){0.0f, 0.0f, 1.0f},
	&triviewed, clipped));
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

static t_quaternion	vector3_to_quaternion(t_vector3 v)
{
	return((t_quaternion){v.x, v.y, v.z, 1.0f});
}

static t_texture	vector2_to_texture(t_vector2 v)
{
	t_texture t;
	t.u = v.x;
	t.v = v.y;
	t.w = 1.0f;
	return(t);
}

void engine3d(t_sdlcontext sdl, t_game game, t_render render)
{
	t_object		*obj;
	t_vector3		vtarget;
	t_mat4x4		matcamera;
	t_mat4x4		matview;
	int				i;
	int				index;

	vtarget = vector3_add(render.position, render.lookdir);
	matcamera = matrix_lookat(render.position, vtarget, (t_vector3){0, 0, 1});
	matview = matrix_quickinverse(matcamera);
	i = 0;
	while (i < sdl.objectcount)
	{
		obj = &sdl.objects[i];
		index = 0;
		while (index < obj->vertice_count)
		{
			render.q[index] = vector3_to_quaternion(obj->vertices[index]);
			render.q[index] = quaternion_mul_matrix(render.matworld, render.q[index]);
			index++;
		}
		index = 0;
		while (index < obj->face_count)
		{
			t_triangle tritransformed;
			t_vector3 normal;	
			t_vector3 vcameraray;	

			tritransformed = (t_triangle){render.q[obj->faces[index].v_indices[0] - 1], render.q[obj->faces[index].v_indices[1] - 1], render.q[obj->faces[index].v_indices[2] - 1]};
			if (obj->uv_count != 0)
			{
				tritransformed.t[0] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[0] - 1]);
				tritransformed.t[1] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[1] - 1]);
				tritransformed.t[2] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[2] - 1]);
			}
			normal = normal_calc(tritransformed);
			vcameraray = vector3_sub(tritransformed.p[0].v, render.position);
			if (vector3_dot(normal, vcameraray) < 0.0f || 1) //TODO: Currently ignoring normals with || 1
			{
				t_triangle clipped[2];
				int nclippedtriangles = clippedtriangles(tritransformed, matview, clipped);
				for (int n = 0; n < nclippedtriangles; n++)
				{
					render.calc_triangles[render.calc_tri_count++] = triangle_to_screenspace(render.matproj, clipped[n], sdl);
				}
			}
			index++;
		}
		render.img = obj->materials[0].img;
		if (!render.img)
			render.img = render.debug_img;
		clipped(render, sdl);
		render.calc_tri_count = 0;
		render.draw_tri_count = 0;
		i++;
	}
	i = 0;
	while(i < game.tri_count)//TODO:(legacy code)Remove whole while loop after walls are added to objects in sdl struct
	{
		t_triangle tritransformed;
		t_vector3 normal;
		t_vector3 vcameraray;

		tritransformed = transform_calc(render.matworld, game.triangles[i]);
		normal = normal_calc(tritransformed);
		vcameraray = vector3_sub(tritransformed.p[0].v, render.position);
		if (vector3_dot(normal, vcameraray) < 0.0f || 1) //TODO: Currently ignoring normals with || 1
		{
			t_triangle clipped[2];
			int nclippedtriangles = clippedtriangles(tritransformed, matview, clipped);
			for (int n = 0; n < nclippedtriangles; n++)
			{
				render.calc_triangles[render.calc_tri_count++] = triangle_to_screenspace(render.matproj, clipped[n], sdl);
			}
		}
		i++;
	}
	render.img = render.debug_img;
	clipped(render, sdl);
}
