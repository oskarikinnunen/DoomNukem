/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_3d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/27 18:46:08 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"

#define PRINT_DEBUG 1

static t_quaternion Initquater()
{
	t_vector3 v;
	t_quaternion q;

	v.x = 0.0f;
	v.y = 0.0f;
	v.z = 0.0f;
	q.v = v;
	q.w = 1.0f;
	return (q);
}

static t_triangle Inittri()
{
	t_triangle t;

	t.p[0] = Initquater();
	t.p[1] = Initquater();
	t.p[2] = Initquater();
	t.clr = CLR_GRAY;
	return(t);
}

static int Triangle_ClipAgainstPlane(t_vector3 plane_p, t_vector3 plane_n, t_triangle *in_tri, t_triangle *out_tri1, t_triangle *out_tri2)
{
	// Make sure plane normal is indeed normal
	plane_n = vector3_normalise(plane_n);

	// Create two temporary storage arrays to classify points either side of plane
	// If dist1ance sign is positive, point lies on "inside" of plane
	t_quaternion inside_points[3];  int nInsidePointCount = 0;
	t_quaternion outside_points[3]; int nOutsidePointCount = 0;
	t_texture inside_tex[3]; int nInsideTexCount = 0;
	t_texture outside_tex[3]; int nOutsideTexCount = 0;

	for (int e = 0; e < 3; e++)
	{
		inside_points[e] = Initquater();
		outside_points[e] = Initquater();
	}
	// Get signed dist1ance of each point in triangle to plane
	float d0 = vector3_fdist_to_plane(in_tri->p[0].v, plane_n, plane_p);
	float d1 = vector3_fdist_to_plane(in_tri->p[1].v, plane_n, plane_p);
	float d2 = vector3_fdist_to_plane(in_tri->p[2].v, plane_n, plane_p);

	if (d0 >= 0)
	{
		inside_points[nInsidePointCount++] = in_tri->p[0];
		inside_tex[nInsideTexCount++] = in_tri->t[0];
	}
	else
	{
		outside_points[nOutsidePointCount++] = in_tri->p[0];
		outside_tex[nOutsideTexCount++] = in_tri->t[0];
	}
	if (d1 >= 0)
	{
		inside_points[nInsidePointCount++] = in_tri->p[1];
		inside_tex[nInsideTexCount++] = in_tri->t[1];
	}
	else
	{
		outside_points[nOutsidePointCount++] = in_tri->p[1];
		outside_tex[nOutsideTexCount++] = in_tri->t[1];
	}
	if (d2 >= 0)
	{
		inside_points[nInsidePointCount++] = in_tri->p[2];
		inside_tex[nInsideTexCount++] = in_tri->t[2];
	}
	else
	{
		outside_points[nOutsidePointCount++] = in_tri->p[2];
		outside_tex[nOutsideTexCount++] = in_tri->t[2];
	}

	// Now classify triangle points, and break the input triangle into 
	// smaller output triangles if required. There are four possible
	// outcomes...

	if (nInsidePointCount == 0)
	{
		// All points lie on the outside of plane, so clip whole triangle
		// It ceases to exist

		return 0; // No returned triangles are valid
	}

	if (nInsidePointCount == 3)
	{
		// All points lie on the inside of plane, so do nothing
		// and allow the triangle to simply pass through
		*out_tri1 = *in_tri;

		return 1; // Just the one returned original triangle is valid
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{

		// Triangle should be clipped. As two points lie outside
		// the plane, the triangle simply becomes a smaller triangle

		// Copy appearance info to new triangle
		out_tri1->clr = in_tri->clr;
		//out_tri1->sym = in_tri->sym;

		// The inside point is valid, so keep that...
		out_tri1->p[0] = inside_points[0];
		out_tri1->t[0] = inside_tex[0];

		// but the two new points are at the locations where the 
		// original sides of the triangle (lines) intersect with the plane
		float t;
		out_tri1->p[1] = quaternion_intersectplane(plane_p, plane_n, inside_points[0], outside_points[0], &t);
		out_tri1->t[1].u = t * (outside_tex[0].u - inside_tex[0].u) + inside_tex[0].u;
		out_tri1->t[1].v = t * (outside_tex[0].v - inside_tex[0].v) + inside_tex[0].v;
		out_tri1->t[1].w = t * (outside_tex[0].w - inside_tex[0].w) + inside_tex[0].w;

		out_tri1->p[2] = quaternion_intersectplane(plane_p, plane_n, inside_points[0], outside_points[1], &t);
		out_tri1->t[2].u = t * (outside_tex[1].u - inside_tex[0].u) + inside_tex[0].u;
		out_tri1->t[2].v = t * (outside_tex[1].v - inside_tex[0].v) + inside_tex[0].v;
		out_tri1->t[2].w = t * (outside_tex[1].w - inside_tex[0].w) + inside_tex[0].w;
		return 1; // Return the newly formed single triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		// Copy appearance info to new triangles
		//out_tri1.col =  in_tri.col;
		//out_tri1.sym = in_tri.sym;

		//out_tri2.col =  in_tri.col;
		//out_tri2.sym = in_tri.sym;

		out_tri1->clr = in_tri->clr;
		out_tri2->clr = in_tri->clr;

		// The first triangle consists of the two inside points and a new
		// point determined by the location where one side of the triangle
		// intersects with the plane
		out_tri1->p[0] = inside_points[0];
		out_tri1->p[1] = inside_points[1];
		out_tri1->t[0] = inside_tex[0];
		out_tri1->t[1] = inside_tex[1];

		float t;
		out_tri1->p[2] = quaternion_intersectplane(plane_p, plane_n, inside_points[0], outside_points[0], &t);
		out_tri1->t[2].u = t * (outside_tex[0].u - inside_tex[0].u) + inside_tex[0].u;
		out_tri1->t[2].v = t * (outside_tex[0].v - inside_tex[0].v) + inside_tex[0].v;
		out_tri1->t[2].w = t * (outside_tex[0].w - inside_tex[0].w) + inside_tex[0].w;

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		out_tri2->p[0] = inside_points[1];
		out_tri2->t[0] = inside_tex[1];
		out_tri2->p[1] = out_tri1->p[2];
		out_tri2->t[1] = out_tri1->t[2];
		out_tri2->p[2] = quaternion_intersectplane(plane_p, plane_n, inside_points[1], outside_points[0], &t);
		out_tri2->t[2].u = t * (outside_tex[0].u - inside_tex[1].u) + inside_tex[1].u;
		out_tri2->t[2].v = t * (outside_tex[0].v - inside_tex[1].v) + inside_tex[1].v;
		out_tri2->t[2].w = t * (outside_tex[0].w - inside_tex[1].w) + inside_tex[1].w;

		return 2; // Return two newly formed triangles which form a quad
	}
	return(0);
}

static void draw_triangles(t_sdlcontext sdl, t_triangle *triangles, int index, int end, t_img img)
{
	while (index < end)
	{
		z_fill_tri(sdl, triangles[index], img);
		index++;
	}
}

static void clipped(int count, t_triangle *triangles_calc, t_sdlcontext sdl, t_img img)
{
	int i = 0;
	int start = 0;
	int end = 0;

	t_triangle triangles[200];


	while (i < count && 1)
	{
		t_triangle clipped[2];

		triangles[end++] = triangles_calc[i];
		int nnewtriangles = 1;
		for (int p = 0; p < 4; p++)
		{
			int ntristoadd = 0;
			while (nnewtriangles > 0)
			{
				clipped[0] = Inittri();
				clipped[1] = Inittri();
				t_triangle test = Inittri();
				test = triangles[start++];
				nnewtriangles--;
				switch (p)
				{
				case 0: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){0.0f, 1.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				case 1: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, (float)sdl.window_h - 1.0f, 0.0f}, (t_vector3){0.0f, -1.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				case 2: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){1.0f, 0.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				case 3: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){(float)sdl.window_w - 1.0f, 0.0f, 0.0f}, (t_vector3){-1.0f, 0.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				}
				for (int w = 0; w < ntristoadd; w++)
				{
					triangles[end++] = clipped[w];
				}
			}
			nnewtriangles = end - start;
		}
		draw_triangles(sdl, triangles, start, end, img);
		start = 0;
		end = 0;
		i++;
	}
}

static t_triangle transform_calc(t_mat4x4 matworld, t_triangle triangles)
{
	t_triangle tritransformed = Inittri();

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
	t_triangle triviewed = Inittri();

	triviewed.p[0] = quaternion_mul_matrix(matview, tritransformed.p[0]);
	triviewed.p[1] = quaternion_mul_matrix(matview, tritransformed.p[1]);
	triviewed.p[2] = quaternion_mul_matrix(matview, tritransformed.p[2]);
	triviewed.t[0] = tritransformed.t[0];
	triviewed.t[1] = tritransformed.t[1];
	triviewed.t[2] = tritransformed.t[2];
	return (Triangle_ClipAgainstPlane(
	(t_vector3){0.0f, 0.0f, 0.1f},
	(t_vector3){0.0f, 0.0f, 1.0f},
	&triviewed, &(clipped[0]), &(clipped[1])));
}

static t_triangle triangle_to_screenspace(t_mat4x4 matproj, t_triangle clipped, t_sdlcontext sdl)
{
	t_triangle triprojected = Inittri();

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

void engine3d(t_sdlcontext sdl, t_game game)
{
	static t_img debug_img;
	t_triangle	triangles_calc[200];
	int			i;
	int			count = 0;
	t_vector3 vtarget;
	t_mat4x4 matworld = matrix_makeidentity();
	t_mat4x4 matproj = matrix_makeprojection(90.0f, (float)sdl.window_h / (float)sdl.window_w, 2.0f, 1000.0f);

	vtarget = vector3_add(game.player.position, game.player.lookdir);
	t_mat4x4 matcamera = matrix_lookat(game.player.position, vtarget, (t_vector3){0, 0, 1});
	t_mat4x4 matview = matrix_quickinverse(matcamera);
	i = 0;
	while(i < game.tri_count)
	{
		t_triangle tritransformed;
		t_vector3 normal;
		t_vector3 vcameraray;

		tritransformed = transform_calc(matworld, game.triangles[i]);
		normal = normal_calc(tritransformed);
		vcameraray = vector3_sub(tritransformed.p[0].v, game.player.position);
		if (vector3_dot(normal, vcameraray) < 0.0f || 1) //TODO: Currently ignoring normals with || 1
		{
			t_triangle clipped[2];
			int nclippedtriangles = clippedtriangles(tritransformed, matview, clipped);
			for (int n = 0; n < nclippedtriangles; n++)
			{
				triangles_calc[count++] = triangle_to_screenspace(matproj, clipped[n], sdl);
			}
		}
		i++;
	}
	if (debug_img.length == 0)
		debug_img = get_image_by_name(sdl, "");
	clipped(count, triangles_calc, sdl, debug_img);
}
