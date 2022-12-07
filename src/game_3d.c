/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_3d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2022/12/07 06:45:42 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
 
#include "bresenham.h"
#include "objects.h"
#include "vectors.h"


static void draw_triangles(t_sdlcontext *sdl, t_render *render)
{
	int index = 0;
	while (index < render->screenspace_ptri_count)
	{
		if (!render->wireframe && render->img != NULL)
		{
			render_triangle(sdl, render->screenspace_ptris[index], render->img);
		}
		if (render->wireframe)
		{
			drawline(*sdl, render->screenspace_ptris[index].p[0], render->screenspace_ptris[index].p[1], render->gizmocolor);
			drawline(*sdl, render->screenspace_ptris[index].p[1], render->screenspace_ptris[index].p[2], render->gizmocolor);
			drawline(*sdl, render->screenspace_ptris[index].p[2], render->screenspace_ptris[index].p[0], render->gizmocolor);
		}
		if (render->img == NULL)
		{
			t_img	img;

			img.data = (uint32_t [1]) {render->screenspace_ptris[index].clr};
			img.size.x = 1;
			img.size.y = 1;
			img.length = 1;
			render->img = &img;
			render_triangle(sdl, render->screenspace_ptris[index], render->img);
			render->img = NULL;
		}
		render->rs.triangle_count++;
		index++;
	}
}

/*
static void draw_triangles(t_sdlcontext sdl, t_render *render)
{
	int index = 0;
	while (index < render->draw_tri_count)
	{
		if (!render->wireframe && render->img != NULL)
			z_fill_tri(sdl, render->draw_triangles[index], *render->img);
		if (render->wireframe)
		{
			drawline(sdl, (t_point){render->draw_triangles[index].p[0].v.x, render->draw_triangles[index].p[0].v.y}, (t_point){render->draw_triangles[index].p[1].v.x, render->draw_triangles[index].p[1].v.y}, render->gizmocolor);
			drawline(sdl, (t_point){render->draw_triangles[index].p[2].v.x, render->draw_triangles[index].p[2].v.y}, (t_point){render->draw_triangles[index].p[1].v.x, render->draw_triangles[index].p[1].v.y}, render->gizmocolor);
			drawline(sdl, (t_point){render->draw_triangles[index].p[0].v.x, render->draw_triangles[index].p[0].v.y}, (t_point){render->draw_triangles[index].p[2].v.x, render->draw_triangles[index].p[2].v.y}, render->gizmocolor);
		}
		if (render->img == NULL)
		{
			t_img	img;

			img.data = (uint32_t [1]) {render->draw_triangles[index].clr};
			img.size.x = 1;
			img.size.y = 1;
			img.length = 1;
			render->img = &img;
			z_fill_tri(sdl, render->draw_triangles[index], *render->img);
			render->img = NULL;
			//render->draw_triangles[index].clr = CLR_RED;
			//printf("color r %i \n", render->draw_triangles[index].clr & 0xFF);
			//z_fill_tri_solid(sdl, render->draw_triangles[index]);
			//drawline(sdl, (t_point){render->draw_triangles[index].p[0].v.x, render->draw_triangles[index].p[0].v.y}, (t_point){render->draw_triangles[index].p[1].v.x, render->draw_triangles[index].p[1].v.y}, render->draw_triangles[index].clr);
			//drawline(sdl, (t_point){render->draw_triangles[index].p[2].v.x, render->draw_triangles[index].p[2].v.y}, (t_point){render->draw_triangles[index].p[1].v.x, render->draw_triangles[index].p[1].v.y}, render->draw_triangles[index].clr);
			//drawline(sdl, (t_point){render->draw_triangles[index].p[0].v.x, render->draw_triangles[index].p[0].v.y}, (t_point){render->draw_triangles[index].p[2].v.x, render->draw_triangles[index].p[2].v.y}, render->draw_triangles[index].clr);
		}
		render->rs.triangle_count++;
		index++;
	}
}
*/
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

static void clipped_point_triangle(t_render *render, t_sdlcontext sdl)
{
	int i = 0;
	int start = 0;
	int end = 0;

	t_point_triangle	triangles[200];
	t_point_triangle	clipped[2];
	while (i < render->worldspace_ptri_count)
	{
		triangles[end++] = render->worldspace_ptris[i];
		int nnewtriangles = 1;
		for (int p = 0; p < 4; p++)
		{
			int ntristoadd = 0;
			while (nnewtriangles > 0)
			{
				t_point_triangle test;
				test = triangles[start++];
				nnewtriangles--;
				switch (p)
				{
				case 0: ntristoadd = point_clip_triangle_against_plane((t_vector2){0.0f, 0.0f}, (t_vector2){0.0f, 1.0f}, test, clipped); break;
				case 1: ntristoadd = point_clip_triangle_against_plane((t_vector2){0.0f, (float)(sdl.window_h * sdl.resolution_scaling) - 1.0f}, (t_vector2){0.0f, -1.0f}, test, clipped); break;
				case 2: ntristoadd = point_clip_triangle_against_plane((t_vector2){0.0f, 0.0f}, (t_vector2){1.0f, 0.0f}, test, clipped); break;
				case 3: ntristoadd = point_clip_triangle_against_plane((t_vector2){(float)(sdl.window_w * sdl.resolution_scaling) - 1.0f, 0.0f}, (t_vector2){-1.0f, 0.0f}, test, clipped); break;
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
			render->screenspace_ptris[render->screenspace_ptri_count++] = triangles[start++];
		}
		start = 0;
		end = 0;
		i++;
	}
}

void clipped(t_render *render, t_sdlcontext sdl)
{
	int i = 0;
	int start = 0;
	int end = 0;

	t_triangle	triangles[200];
	t_triangle	clipped[2];
	while (i < render->occ_calc_tri_count)
	{
		triangles[end++] = render->occ_calc_tris[i];
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
				case 1: ntristoadd = clip_triangle_against_plane((t_vector3){0.0f, (float)(sdl.window_h * sdl.resolution_scaling) - 1.0f, 0.0f}, (t_vector3){0.0f, -1.0f, 0.0f}, test, clipped); break;
				case 2: ntristoadd = clip_triangle_against_plane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){1.0f, 0.0f, 0.0f}, test, clipped); break;
				case 3: ntristoadd = clip_triangle_against_plane((t_vector3){(float)(sdl.window_w * sdl.resolution_scaling) *  - 1.0f, 0.0f, 0.0f}, (t_vector3){-1.0f, 0.0f, 0.0f}, test, clipped); break;
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
			render->occ_draw_tris[render->occ_tri_count++] = triangles[start++];
		}
		start = 0;
		end = 0;
		i++;
	}
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
	triviewed.clr = tritransformed.clr;
	return (clip_triangle_against_plane(
	(t_vector3){.z = 0.1f},
	(t_vector3){.z = 1.0f},
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

	triprojected.p[0].v.x *= 0.5f * ((float)sdl.window_w * sdl.resolution_scaling);
	triprojected.p[0].v.y *= 0.5f * ((float)sdl.window_h * sdl.resolution_scaling);
	triprojected.p[1].v.x *= 0.5f * ((float)sdl.window_w * sdl.resolution_scaling);
	triprojected.p[1].v.y *= 0.5f * ((float)sdl.window_h * sdl.resolution_scaling);
	triprojected.p[2].v.x *= 0.5f * ((float)sdl.window_w * sdl.resolution_scaling);
	triprojected.p[2].v.y *= 0.5f * ((float)sdl.window_h * sdl.resolution_scaling);
	triprojected.clr = clipped.clr;

	return(triprojected);
}

static t_point_triangle triangle_to_screenspace_point_triangle(t_mat4x4 matproj, t_triangle clipped, t_sdlcontext sdl)
{
	t_triangle			triprojected;
	t_point_triangle	tri;

	triprojected.p[0] = quaternion_mul_matrix(matproj, clipped.p[0]);
	triprojected.p[1] = quaternion_mul_matrix(matproj, clipped.p[1]);
	triprojected.p[2] = quaternion_mul_matrix(matproj, clipped.p[2]);
	triprojected.t[0] = clipped.t[0];
	triprojected.t[1] = clipped.t[1];
	triprojected.t[2] = clipped.t[2];

	tri.t[0].u = triprojected.t[0].u / triprojected.p[0].w;
	tri.t[1].u = triprojected.t[1].u / triprojected.p[1].w;
	tri.t[2].u = triprojected.t[2].u / triprojected.p[2].w;

	tri.t[0].v = triprojected.t[0].v / triprojected.p[0].w;
	tri.t[1].v = triprojected.t[1].v / triprojected.p[1].w;
	tri.t[2].v = triprojected.t[2].v / triprojected.p[2].w;

	tri.t[0].w = 1.0f / triprojected.p[0].w;
	tri.t[1].w = 1.0f / triprojected.p[1].w;
	tri.t[2].w = 1.0f / triprojected.p[2].w;
	
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

	triprojected.p[0].v.x *= 0.5f * (float)(sdl.window_w * sdl.resolution_scaling);
	triprojected.p[0].v.y *= 0.5f * (float)(sdl.window_h * sdl.resolution_scaling);
	triprojected.p[1].v.x *= 0.5f * (float)(sdl.window_w * sdl.resolution_scaling);
	triprojected.p[1].v.y *= 0.5f * (float)(sdl.window_h * sdl.resolution_scaling);
	triprojected.p[2].v.x *= 0.5f * (float)(sdl.window_w * sdl.resolution_scaling);
	triprojected.p[2].v.y *= 0.5f * (float)(sdl.window_h * sdl.resolution_scaling);

	tri.p[0].x = triprojected.p[0].v.x;
	tri.p[0].y = triprojected.p[0].v.y;
	tri.p[1].x = triprojected.p[1].v.x;
	tri.p[1].y = triprojected.p[1].v.y;
	tri.p[2].x = triprojected.p[2].v.x;
	tri.p[2].y = triprojected.p[2].v.y;
	tri.clr = clipped.clr;

	return(tri);
}

static t_quaternion quaternion_to_screenspace(t_mat4x4 matproj, t_quaternion q, t_sdlcontext sdl)
{
	t_quaternion	proj_q;

	proj_q = quaternion_mul_matrix(matproj, q);


	proj_q.v = vector3_div(proj_q.v, proj_q.w);

	proj_q.v = vector3_negative(proj_q.v);

	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	proj_q.v = vector3_add(proj_q.v, voffsetview);

	proj_q.v.x *= 0.5f * (float)sdl.window_w;
	proj_q.v.y *= 0.5f * (float)sdl.window_h;

	return(proj_q);
}

static t_texture	vector2_to_texture(t_vector2 v)
{
	t_texture t;
	t.u = v.x;
	t.v = v.y;
	t.w = 1.0f;
	return(t);
}

uint32_t shade(uint32_t clr, float norm)
{
	float		mul;
	uint32_t	final;

	//return (clr);
	mul = ft_clampf(norm, 0.75f, 1.0f);
	final = (clr & 0xFF) * mul;
	final += (uint32_t)((clr >> 8 & 0xFF) * mul) << 8;
	final += (uint32_t)((clr >> 16 & 0xFF) * mul) << 16;
	return (final);
}

void render_entity(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	int				index;
	t_object		*obj;
	t_quaternion	temp;

	obj = entity->obj;
	render->occ_calc_tri_count = 0;
	render->occ_tri_count = 0;
	render->worldspace_ptri_count = 0;
	render->screenspace_ptri_count = 0;
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
		render->q[index] = quaternion_mul_matrix(render->matworld, render->q[index]);
		index++;
	}
	index = 0;
	while (index < obj->face_count)
	{
		bool		draw = false;
		t_triangle	tritransformed;
		t_vector3	normal;	
		t_vector3	vcameraray;	

		if (index + 1 == obj->face_count || obj->faces[index].materialindex != obj->faces[index + 1].materialindex)
			draw = true;
		tritransformed = (t_triangle){render->q[obj->faces[index].v_indices[0] - 1], render->q[obj->faces[index].v_indices[1] - 1], render->q[obj->faces[index].v_indices[2] - 1]};
		if (obj->uv_count != 0 && !render->wireframe)
		{
			tritransformed.t[0] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[0] - 1]);
			tritransformed.t[1] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[1] - 1]);
			tritransformed.t[2] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[2] - 1]);
		}
		tritransformed.clr = obj->materials[obj->faces[index].materialindex].kd;
		normal = normal_calc(tritransformed);
		vcameraray = vector3_sub(tritransformed.p[0].v, render->position);
		tritransformed.clr = shade(tritransformed.clr,
									1.0f + (vector3_dot(normal, vector3_normalise(vcameraray)) / 2.0f));
		if (vector3_dot(normal, vcameraray) < 0.0f || 1)
		{
			t_triangle clipped[2];
			int nclippedtriangles = clippedtriangles(tritransformed, render->matview, clipped);
			for (int n = 0; n < nclippedtriangles; n++)
				render->worldspace_ptris[render->worldspace_ptri_count++] = triangle_to_screenspace_point_triangle(render->matproj, clipped[n], sdl);
		}
		if (draw == true)
		{
			render->img = obj->materials[obj->faces[index].materialindex].img;
			clipped_point_triangle(render, sdl);
			draw_triangles(&sdl, render);
			render->img = NULL;
			render->worldspace_ptri_count = 0;
			render->screenspace_ptri_count = 0;
		}
		index++;
	}
	render->rs.render_count++;
}

void render_ray(t_sdlcontext sdl, t_render render, t_vector3 from, t_vector3 to)
{
	render.q[0] = vector3_to_quaternion(from);
	render.q[0] = quaternion_mul_matrix(render.matworld, render.q[0]);
	render.q[0] = quaternion_mul_matrix(render.matview, render.q[0]);
	render.q[0] = quaternion_to_screenspace(render.matproj, render.q[0], sdl);
	render.q[1] = vector3_to_quaternion(to);
	render.q[1] = quaternion_mul_matrix(render.matworld, render.q[1]);
	render.q[1] = quaternion_mul_matrix(render.matview, render.q[1]);
	render.q[1] = quaternion_to_screenspace(render.matproj, render.q[1], sdl);
	drawline(sdl,
		(t_point) {render.q[0].v.x, render.q[0].v.y},
		(t_point) {render.q[1].v.x, render.q[1].v.y}, render.gizmocolor);
		
	render.occ_calc_tri_count = 0;
	render.occ_tri_count = 0;
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
	render.q[0] = quaternion_mul_matrix(render.matworld, render.q[0]);
	render.q[0] = quaternion_mul_matrix(render.matview, render.q[0]);
	render.q[0] = quaternion_to_screenspace(render.matproj, render.q[0], sdl);
	if (render.q[0].w > 0.0f
		&& render.q[0].v.x >= 0.0f && render.q[0].v.x < sdl.window_w
		&& render.q[0].v.y >= 0.0f && render.q[0].v.y < sdl.window_h)
		drawcircle(sdl, (t_point){render.q[0].v.x, render.q[0].v.y}, size, render.gizmocolor);
	render.occ_calc_tri_count = 0;
	render.occ_tri_count = 0;
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
		ent.transform.position = curstep;
		ent.transform.scale = vector3_one();
		//render_gizmo(sdl, render, &ent);
		debug_iter++;
	}
}