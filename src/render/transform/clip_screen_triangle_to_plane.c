/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clip_screen_triangle_to_plane.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 18:49:09 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 11:15:56 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	vector2_clip_triangle(t_vector2 plane_p, t_vector2 plane_n, \
t_screen_triangle in_tri, t_screen_triangle *out_tri)
{
	float		t;
	t_vector2	start;
	t_vector2	end;

	start = in_tri.p[0];
	end = in_tri.p[2];
	t = vector2_line_intersect_plane(plane_p, plane_n, start, end);
	out_tri->p[0] = vector2_lerp(in_tri.p[0], in_tri.p[2], t);
	out_tri->t[0] = vector3_lerp(in_tri.t[0], in_tri.t[2], t);
	start = in_tri.p[1];
	end = in_tri.p[2];
	t = vector2_line_intersect_plane(plane_p, plane_n, start, end);
	out_tri->p[1] = vector2_lerp(in_tri.p[1], in_tri.p[2], t);
	out_tri->t[1] = vector3_lerp(in_tri.t[1], in_tri.t[2], t);
	out_tri->p[2] = in_tri.p[2];
	out_tri->t[2] = in_tri.t[2];
	return (1);
}

static int	vector2_clip_quad_to_triangles(t_vector2 plane_p,
	t_vector2 plane_n, t_screen_triangle in_tri, t_screen_triangle *out_tri)
{
	float		t;
	t_vector2	start;
	t_vector2	end;

	start = in_tri.p[0];
	end = in_tri.p[1];
	t = vector2_line_intersect_plane(plane_p, plane_n, start, end);
	out_tri[0].p[0] = vector2_lerp(in_tri.p[0], in_tri.p[1], t);
	out_tri[0].t[0] = vector3_lerp(in_tri.t[0], in_tri.t[1], t);
	start = in_tri.p[0];
	end = in_tri.p[2];
	t = vector2_line_intersect_plane(plane_p, plane_n, start, end);
	out_tri[0].p[1] = vector2_lerp(in_tri.p[0], in_tri.p[2], t);
	out_tri[0].t[1] = vector3_lerp(in_tri.t[0], in_tri.t[2], t);
	out_tri[0].p[2] = in_tri.p[1];
	out_tri[0].t[2] = in_tri.t[1];
	out_tri[1].p[0] = vector2_lerp(in_tri.p[0], in_tri.p[2], t);
	out_tri[1].t[0] = vector3_lerp(in_tri.t[0], in_tri.t[2], t);
	out_tri[1].p[1] = in_tri.p[1];
	out_tri[1].t[1] = in_tri.t[1];
	out_tri[1].p[2] = in_tri.p[2];
	out_tri[1].t[2] = in_tri.t[2];
	return (2);
}

static int	set_distance(t_screen_triangle in_tri, t_vector2 plane_p, \
t_vector2 plane_n, float dist[3])
{
	int	i;
	int	outside;

	outside = 0;
	i = 0;
	while (i < 3)
	{
		dist[i] = vector2_fdist_to_plane(in_tri.p[i], plane_n, plane_p);
		if (dist[i] < 0.0f)
			outside++;
		i++;
	}
	return (outside);
}

int	clip_screen_triangle_plane(t_vector2 plane_p, t_vector2 plane_n, \
t_screen_triangle in_tri, t_screen_triangle out_tri[2])
{
	float		dist[3];
	int			outside;

	plane_n = vector2_normalise(plane_n);
	outside = set_distance(in_tri, plane_p, plane_n, dist);
	if (outside == 3)
		return (0);
	if (outside == 0)
	{
		out_tri[0] = in_tri;
		return (1);
	}
	sort_vector2_vector3_by_dist(dist, in_tri.p, in_tri.t);
	if (outside == 1)
		return (vector2_clip_quad_to_triangles(
				plane_p, plane_n, in_tri, out_tri));
	if (outside == 2)
		return (vector2_clip_triangle(plane_p, plane_n, in_tri, out_tri));
	return (0);
}
