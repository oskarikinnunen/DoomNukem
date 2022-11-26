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
