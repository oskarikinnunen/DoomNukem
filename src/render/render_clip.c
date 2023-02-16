#include "doomnukem.h"

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

static void sort_quat_by_dist(float dist[3], t_quaternion q[3])
{
	int i, j;
	float			key;
	t_quaternion	temp_q;

	for (i = 1; i < 3; i++)
	{
		key = dist[i];
		temp_q = q[i];
		j = i - 1;
		while (j >= 0 && dist[j] > key)
		{
			dist[j + 1] = dist[j];
			q[j + 1] = q[j];
			j = j - 1;
		}
		dist[j + 1] = key;
		q[j + 1] = temp_q;
	}
}

static void sort_point_and_tex_by_dist(float dist[3], t_point p[3], t_texture t[3])
{
	int 			i;
	int				j;
	float			key;
	t_point			temp_p;
	t_texture		temp_t;

	for (i = 1; i < 3; i++)
	{
		key = dist[i];
		temp_p = p[i];
		temp_t = t[i];
		j = i - 1;
		while (j >= 0 && dist[j] > key)
		{
			dist[j + 1] = dist[j];
			p[j + 1] = p[j];
			t[j + 1] = t[j];
			j = j - 1;
		}
		dist[j + 1] = key;
		p[j + 1] = temp_p;
		t[j + 1] = temp_t;
	}
}

static int clip_triangle(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle *out_tri)
{
	float	t;

	out_tri->clr = in_tri.clr;
	t = line_intersect_plane(plane_p, plane_n, in_tri.p[0].v, in_tri.p[2].v);
	out_tri->p[0] = lerp_quaternion(in_tri.p[0], in_tri.p[2], t);
	out_tri->t[0] = lerp_texture(in_tri.t[0], in_tri.t[2], t);
	t = line_intersect_plane(plane_p, plane_n, in_tri.p[1].v, in_tri.p[2].v);
	out_tri->p[1] = lerp_quaternion(in_tri.p[1], in_tri.p[2], t);
	out_tri->t[1] = lerp_texture(in_tri.t[1], in_tri.t[2], t);
	out_tri->p[2] = in_tri.p[2];
	out_tri->t[2] = in_tri.t[2];

	return(1);
}

static int clip_quad_to_triangles(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle *out_tri)
{
	float	t;

	out_tri[0].clr = in_tri.clr;
	out_tri[1].clr = in_tri.clr;
	t = line_intersect_plane(plane_p, plane_n, in_tri.p[0].v, in_tri.p[1].v);
	out_tri[0].p[0] = lerp_quaternion(in_tri.p[0], in_tri.p[1], t);
	out_tri[0].t[0] = lerp_texture(in_tri.t[0], in_tri.t[1], t);
	t = line_intersect_plane(plane_p, plane_n, in_tri.p[0].v, in_tri.p[2].v);
	out_tri[0].p[1] = lerp_quaternion(in_tri.p[0], in_tri.p[2], t);
	out_tri[0].t[1] = lerp_texture(in_tri.t[0], in_tri.t[2], t);
	out_tri[0].p[2] = in_tri.p[1];
	out_tri[0].t[2] = in_tri.t[1];
	out_tri[1].p[0] = lerp_quaternion(in_tri.p[0], in_tri.p[2], t);
	out_tri[1].t[0] = lerp_texture(in_tri.t[0], in_tri.t[2], t);
	out_tri[1].p[1] = in_tri.p[1];
	out_tri[1].t[1] = in_tri.t[1];
	out_tri[1].p[2] = in_tri.p[2];
	out_tri[1].t[2] = in_tri.t[2];

	return(2);
}

static int point_clip_triangle(t_vector2 plane_p, t_vector2 plane_n, t_point_triangle in_tri, t_point_triangle *out_tri)
{
	float	t;

	out_tri->clr = in_tri.clr;
	t = vector2_line_intersect_plane(plane_p, plane_n, (t_vector2){in_tri.p[0].x, in_tri.p[0].y}, (t_vector2){in_tri.p[2].x, in_tri.p[2].y});
	out_tri->p[0] = lerp_point(in_tri.p[0], in_tri.p[2], t);
	out_tri->t[0] = lerp_texture(in_tri.t[0], in_tri.t[2], t);
	t = vector2_line_intersect_plane(plane_p, plane_n, (t_vector2){in_tri.p[1].x, in_tri.p[1].y}, (t_vector2){in_tri.p[2].x, in_tri.p[2].y});
	out_tri->p[1] = lerp_point(in_tri.p[1], in_tri.p[2], t);
	out_tri->t[1] = lerp_texture(in_tri.t[1], in_tri.t[2], t);
	out_tri->p[2] = in_tri.p[2];
	out_tri->t[2] = in_tri.t[2];

	return(1);
}

static int point_clip_quad_to_triangles(t_vector2 plane_p, t_vector2 plane_n, t_point_triangle in_tri, t_point_triangle *out_tri)
{
	float	t;

	out_tri[0].clr = in_tri.clr;
	out_tri[1].clr = in_tri.clr;
	t = vector2_line_intersect_plane(plane_p, plane_n, (t_vector2){in_tri.p[0].x, in_tri.p[0].y}, (t_vector2){in_tri.p[1].x, in_tri.p[1].y});
	out_tri[0].p[0] = lerp_point(in_tri.p[0], in_tri.p[1], t);
	out_tri[0].t[0] = lerp_texture(in_tri.t[0], in_tri.t[1], t);
	t = vector2_line_intersect_plane(plane_p, plane_n, (t_vector2){in_tri.p[0].x, in_tri.p[0].y}, (t_vector2){in_tri.p[2].x, in_tri.p[2].y});
	out_tri[0].p[1] = lerp_point(in_tri.p[0], in_tri.p[2], t);
	out_tri[0].t[1] = lerp_texture(in_tri.t[0], in_tri.t[2], t);
	out_tri[0].p[2] = in_tri.p[1];
	out_tri[0].t[2] = in_tri.t[1];
	out_tri[1].p[0] = lerp_point(in_tri.p[0], in_tri.p[2], t);
	out_tri[1].t[0] = lerp_texture(in_tri.t[0], in_tri.t[2], t);
	out_tri[1].p[1] = in_tri.p[1];
	out_tri[1].t[1] = in_tri.t[1];
	out_tri[1].p[2] = in_tri.p[2];
	out_tri[1].t[2] = in_tri.t[2];

	return(2);
}

int point_clip_triangle_against_plane(t_vector2 plane_p, t_vector2 plane_n, t_point_triangle in_tri, t_point_triangle out_tri[2])
{
	float		dist[3];
	int			i;
	int			outside = 0;
	bool		inside;
	float		t;

	plane_n = vector2_normalise(plane_n);
	i = 0;
	while (i < 3)
	{
		dist[i] = vector2_fdist_to_plane((t_vector2){in_tri.p[i].x, in_tri.p[i].y}, plane_n, plane_p);
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
	sort_point_and_tex_by_dist(dist, in_tri.p, in_tri.t);
	if (outside == 1)
		return (point_clip_quad_to_triangles(plane_p, plane_n, in_tri, out_tri));
	if (outside == 2)
		return (point_clip_triangle(plane_p, plane_n, in_tri, out_tri));
	return(0);
}

int clip_triangle_against_occluder_plane(t_vector2 plane_p, t_vector2 plane_n, t_point_triangle in_tri, t_point_triangle out_tri[2])
{
	float		dist[3];
	int			i;
	int			outside = 0;
	bool		inside;
	float		t;

	plane_n = vector2_normalise(plane_n);
	i = 0;
	while (i < 3)
	{
		dist[i] = vector2_fdist_to_plane((t_vector2){in_tri.p[i].x, in_tri.p[i].y}, plane_n, plane_p);
		if (dist[i] < 1.0f)
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
	sort_point_and_tex_by_dist(dist, in_tri.p, in_tri.t);
	if (outside == 1)
		return (point_clip_quad_to_triangles(plane_p, plane_n, in_tri, out_tri));
	if (outside == 2)
		return (point_clip_triangle(plane_p, plane_n, in_tri, out_tri));
	return(0);
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

static int clip_bitmask_triangle(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle *out_tri)
{
	float	t;

	t = line_intersect_plane(plane_p, plane_n, in_tri.p[0].v, in_tri.p[2].v);
	out_tri->p[0] = lerp_quaternion(in_tri.p[0], in_tri.p[2], t);
	t = line_intersect_plane(plane_p, plane_n, in_tri.p[1].v, in_tri.p[2].v);
	out_tri->p[1] = lerp_quaternion(in_tri.p[1], in_tri.p[2], t);
	out_tri->p[2] = in_tri.p[2];

	return(1);
}

static int clip_bitmask_quad_to_triangles(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle *out_tri)
{
	float	t;

	t = line_intersect_plane(plane_p, plane_n, in_tri.p[0].v, in_tri.p[1].v);
	out_tri[0].p[0] = lerp_quaternion(in_tri.p[0], in_tri.p[1], t);
	t = line_intersect_plane(plane_p, plane_n, in_tri.p[0].v, in_tri.p[2].v);
	out_tri[0].p[1] = lerp_quaternion(in_tri.p[0], in_tri.p[2], t);
	out_tri[0].p[2] = in_tri.p[1];
	out_tri[1].p[0] = lerp_quaternion(in_tri.p[0], in_tri.p[2], t);
	out_tri[1].p[1] = in_tri.p[1];
	out_tri[1].p[2] = in_tri.p[2];

	return(2);
}

int clip_bitmask_triangle_against_plane(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle out_tri[2])
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
		return (clip_bitmask_quad_to_triangles(plane_p, plane_n, in_tri, out_tri));
	if (outside == 2)
		return (clip_bitmask_triangle(plane_p, plane_n, in_tri, out_tri));
	return(0);
}

void clipped_point_triangle(t_render *render, t_sdlcontext sdl)
{
	t_point_triangle	triangles[200];
	t_point_triangle	clipped[2];
	int i = render->start;
	int start = 0;
	int end = 0;

	if (!render->world_triangles)
		return;
	while (i <= render->end)
	{
		t_triangle world_tri;

		world_tri = render->world_triangles[i];
		int nnewtriangles = 0;
		if (!is_triangle_backface(world_tri, render))
		{
			t_triangle clipped[2];
			world_tri = triangle_to_viewspace(world_tri, render->camera.matview);
			int nclippedtriangles = clip_triangle_against_plane((t_vector3){.z = 0.1f}, (t_vector3){.z = 1.0f}, world_tri, clipped);
			for (int n = 0; n < nclippedtriangles; n++)
				triangles[end++] = triangle_to_screenspace_point_triangle(render->camera.matproj, clipped[n], sdl);
			nnewtriangles = nclippedtriangles;
		}
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
				case 0: ntristoadd = point_clip_triangle_against_plane(render->screen_edge.min, (t_vector2){0.0f, 1.0f}, test, clipped); break;
				case 1: ntristoadd = point_clip_triangle_against_plane(render->screen_edge.max, (t_vector2){0.0f, -1.0f}, test, clipped); break;
				case 2: ntristoadd = point_clip_triangle_against_plane(render->screen_edge.min, (t_vector2){1.0f, 0.0f}, test, clipped); break;
				case 3: ntristoadd = point_clip_triangle_against_plane(render->screen_edge.max, (t_vector2){-1.0f, 0.0f}, test, clipped); break;
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
