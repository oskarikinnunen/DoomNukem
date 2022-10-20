/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_3d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/20 20:26:27 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"

#define PRINT_DEBUG 1
#define SH WINDOW_H
#define SW WINDOW_W

t_vector3 player_loc;
uint32_t debugcolor = CLR_PRPL;

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
	t.clr = CLR_GREEN;
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
//	in_tri->clr = CLR_GREEN; //debug colors
//	out_tri1->clr = CLR_GREEN; //debug colors
//	out_tri2->clr = CLR_GREEN; //debug colors
	//out_tri1->clr = CLR_PRPL; //debug colors
	//out_tri2->clr = CLR_TURQ; //debug colors

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

static void swap(int *a, int *b)
{
	int c;

	c = *a;
	*a = *b;
	*b = c;
}
static void fswap(float *a, float *b)
{
	float c;

	c = *a;
	*a = *b;
	*b = c;
}

static void texturetriangle(int x1, int y1, float u1, float v1, float w1,
							int x2, int y2, float u2, float v2, float w2,
							int x3, int y3, float u3, float v3, float w3, uint32_t *tex, t_sdlcontext sdl, uint32_t clr)
{
	debugcolor = clr;
	if (y2 < y1)
	{
		swap(&y1, &y2);
		swap(&x1, &x2);
		fswap(&u1, &u2);
		fswap(&v1, &v2);
		fswap(&w1, &w2);
	}

	if (y3 < y1)
	{
		swap(&y1, &y3);
		swap(&x1, &x3);
		fswap(&u1, &u3);
		fswap(&v1, &v3);
		fswap(&w1, &w3);
	}

	if (y3 < y2)
	{
		swap(&y2, &y3);
		swap(&x2, &x3);
		fswap(&u2, &u3);
		fswap(&v2, &v3);
		fswap(&w2, &w3);
	}

	int dy1 = y2 - y1;
	int dx1 = x2 - x1;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;

	float tex_u, tex_v, tex_w;

	float dax_step = 0, dbx_step = 0,
		du1_step = 0, dv1_step = 0,
		du2_step = 0, dv2_step = 0,
		dw1_step=0, dw2_step=0;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy2) du2_step = du2 / (float)abs(dy2);
	if (dy2) dv2_step = dv2 / (float)abs(dy2);
	if (dy2) dw2_step = dw2 / (float)abs(dy2);

	if (dy1)
	{
		for (int i = y1; i <= y2; i++)
		{
			int ax = x1 + (float)(i - y1) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u1 + (float)(i - y1) * du1_step;
			float tex_sv = v1 + (float)(i - y1) * dv1_step;
			float tex_sw = w1 + (float)(i - y1) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				swap(&ax, &bx);
				fswap(&tex_su, &tex_eu);
				fswap(&tex_sv, &tex_ev);
				fswap(&tex_sw, &tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;
				if (tex_w > sdl.zbuffer[i* WINDOW_W + j])
				{
					//Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w));
					draw(sdl.surface->pixels, (t_point){j, i}, debugcolor);
					sdl.zbuffer[i* WINDOW_W + j] = tex_w;
				}
				t += tstep;
			}

		}
	}

	dy1 = y3 - y2;
	dx1 = x3 - x2;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	du1_step = 0, dv1_step = 0;
	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy1)
	{
		for (int i = y2; i <= y3; i++)
		{
			int ax = x2 + (float)(i - y2) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u2 + (float)(i - y2) * du1_step;
			float tex_sv = v2 + (float)(i - y2) * dv1_step;
			float tex_sw = w2 + (float)(i - y2) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				swap(&ax, &bx);
				fswap(&tex_su, &tex_eu);
				fswap(&tex_sv, &tex_ev);
				fswap(&tex_sw, &tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;
				if (tex_w > sdl.zbuffer[i*WINDOW_W + j])
				{
					//Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w));
					draw(sdl.surface->pixels, (t_point){j, i}, debugcolor);
					sdl.zbuffer[i* WINDOW_W + j] = tex_w;
				}
				t += tstep;
			}
		}	
	}		
}

static void draw_triangles(t_sdlcontext sdl, t_triangle *triangles, int index, int end)
{
	while (index < end)
	{
		if (0)
		texturetriangle(triangles[index].p[0].v.x, triangles[index].p[0].v.y, triangles[index].t[0].u, triangles[index].t[0].v, triangles[index].t[0].w,
					triangles[index].p[1].v.x, triangles[index].p[1].v.y, triangles[index].t[1].u, triangles[index].t[1].v, triangles[index].t[1].w,
					triangles[index].p[2].v.x, triangles[index].p[2].v.y, triangles[index].t[2].u, triangles[index].t[2].v, triangles[index].t[2].w, NULL, sdl, triangles[index].clr);
		else
			z_fill_tri(sdl, triangles[index]);
		/*
		z_fill_tri((int [3][3])
		{
		{triangles[index].p[0].v.x, triangles[index].p[0].v.y, triangles[index].p[0].w},
		{triangles[index].p[1].v.x, triangles[index].p[1].v.y, triangles[index].p[1].w},
		{triangles[index].p[2].v.x, triangles[index].p[2].v.y, triangles[index].p[2].w}},
		sdl, triangles[index].clr, player_loc);*/
		//drawline(sdl.surface->pixels, (t_point){triangles[index].p[0].v.x, triangles[index].p[0].v.y}, (t_point){triangles[index].p[1].v.x, triangles[index].p[1].v.y}, INT_MAX);
		//drawline(sdl.surface->pixels, (t_point){triangles[index].p[1].v.x, triangles[index].p[1].v.y}, (t_point){triangles[index].p[2].v.x, triangles[index].p[2].v.y}, INT_MAX);
		//drawline(sdl.surface->pixels, (t_point){triangles[index].p[2].v.x, triangles[index].p[2].v.y}, (t_point){triangles[index].p[0].v.x, triangles[index].p[0].v.y}, INT_MAX);
		index++;
	}
}

static void clipped(int count, t_triangle *triangles_calc, t_sdlcontext sdl)
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
				case 1: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, (float)WINDOW_H - 1.0f, 0.0f}, (t_vector3){0.0f, -1.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				case 2: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){1.0f, 0.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				case 3: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){(float)WINDOW_W - 1.0f, 0.0f, 0.0f}, (t_vector3){-1.0f, 0.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				}
				for (int w = 0; w < ntristoadd; w++)
				{
					triangles[end++] = clipped[w];
				}
			}
			nnewtriangles = end - start;
		}
		draw_triangles(sdl, triangles, start, end);
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
	return (Triangle_ClipAgainstPlane(
	(t_vector3){0.0f, 0.0f, 0.1f},
	(t_vector3){0.0f, 0.0f, 1.0f},
	&triviewed, &(clipped[0]), &(clipped[1])));
}

static t_triangle triangle_to_screenspace(t_mat4x4 matproj, t_triangle clipped)
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

	//triprojected.p[0].v = vector3_negative(triprojected.p[0].v); // normally shouldnt turn z to negative, might cause problems
	//triprojected.p[1].v = vector3_negative(triprojected.p[1].v);
	//triprojected.p[2].v = vector3_negative(triprojected.p[2].v);

	triprojected.p[0].v.x *= -1.0f;
	triprojected.p[1].v.x *= -1.0f;
	triprojected.p[2].v.x *= -1.0f;
	triprojected.p[0].v.y *= -1.0f;
	triprojected.p[1].v.y *= -1.0f;
	triprojected.p[2].v.y *= -1.0f;


	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	triprojected.p[0].v = vector3_add(triprojected.p[0].v, voffsetview);
	triprojected.p[1].v = vector3_add(triprojected.p[1].v, voffsetview);
	triprojected.p[2].v = vector3_add(triprojected.p[2].v, voffsetview);

	triprojected.p[0].v.x *= 0.5f * (float)SW;
	triprojected.p[0].v.y *= 0.5f * (float)SH;
	triprojected.p[1].v.x *= 0.5f * (float)SW;
	triprojected.p[1].v.y *= 0.5f * (float)SH;
	triprojected.p[2].v.x *= 0.5f * (float)SW;
	triprojected.p[2].v.y *= 0.5f * (float)SH;

	return(triprojected);
}

void engine3d(t_sdlcontext sdl, t_game game)
{
	t_triangle	triangles_calc[200];
	int			i;
	int			count = 0;
	t_vector3 vtarget;
	t_mat4x4 matworld = matrix_makeidentity();
	t_mat4x4 matproj = matrix_makeprojection(90.0f, (float)WINDOW_H / (float)WINDOW_W, 2.0f, 1000.0f);

	vtarget = vector3_add(game.player.position, game.player.lookdir);
	player_loc = game.player.position;
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
				triangles_calc[count++] = triangle_to_screenspace(matproj, clipped[n]);
				triangles_calc[count - 1].clr = game.triangles[i].clr;
			}
		}
		i++;
	}
	clipped(count, triangles_calc, sdl);
}
