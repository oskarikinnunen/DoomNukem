/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clip_world_triangle_to_plane.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 18:48:58 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/24 15:56:34 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	clip_triangle(t_vector3 plane_p, t_vector3 plane_n, \
t_world_triangle in_tri, t_world_triangle *out_tri)
{
	float	t;

	t = line_intersect_plane(plane_p, plane_n, in_tri.p[0].v, in_tri.p[2].v);
	out_tri->p[0] = lerp_quaternion(in_tri.p[0], in_tri.p[2], t);
	out_tri->t[0] = vector3_lerp(in_tri.t[0], in_tri.t[2], t);
	t = line_intersect_plane(plane_p, plane_n, in_tri.p[1].v, in_tri.p[2].v);
	out_tri->p[1] = lerp_quaternion(in_tri.p[1], in_tri.p[2], t);
	out_tri->t[1] = vector3_lerp(in_tri.t[1], in_tri.t[2], t);
	out_tri->p[2] = in_tri.p[2];
	out_tri->t[2] = in_tri.t[2];
	return (1);
}

static int	clip_quad_to_triangles(t_vector3 plane_p, t_vector3 plane_n, \
t_world_triangle in_tri, t_world_triangle *out_tri)
{
	float	t;

	t = line_intersect_plane(plane_p, plane_n, in_tri.p[0].v, in_tri.p[1].v);
	out_tri[0].p[0] = lerp_quaternion(in_tri.p[0], in_tri.p[1], t);
	out_tri[0].t[0] = vector3_lerp(in_tri.t[0], in_tri.t[1], t);
	t = line_intersect_plane(plane_p, plane_n, in_tri.p[0].v, in_tri.p[2].v);
	out_tri[0].p[1] = lerp_quaternion(in_tri.p[0], in_tri.p[2], t);
	out_tri[0].t[1] = vector3_lerp(in_tri.t[0], in_tri.t[2], t);
	out_tri[0].p[2] = in_tri.p[1];
	out_tri[0].t[2] = in_tri.t[1];
	out_tri[1].p[0] = lerp_quaternion(in_tri.p[0], in_tri.p[2], t);
	out_tri[1].t[0] = vector3_lerp(in_tri.t[0], in_tri.t[2], t);
	out_tri[1].p[1] = in_tri.p[1];
	out_tri[1].t[1] = in_tri.t[1];
	out_tri[1].p[2] = in_tri.p[2];
	out_tri[1].t[2] = in_tri.t[2];
	return (2);
}

static int	set_distance(t_world_triangle in_tri, t_vector3 plane_p, \
t_vector3 plane_n, float dist[3])
{
	int	i;
	int	outside;

	outside = 0;
	i = 0;
	while (i < 3)
	{
		dist[i] = vector3_fdist_to_plane(in_tri.p[i].v, plane_n, plane_p);
		if (dist[i] < 0.0f)
			outside++;
		i++;
	}
	return (outside);
}

int	clip_triangle_plane(t_vector3 plane_p, t_vector3 plane_n, \
t_world_triangle in_tri, t_world_triangle out_tri[2])
{
	float	dist[3];
	int		outside;

	plane_n = vector3_normalise(plane_n);
	outside = set_distance(in_tri, plane_p, plane_n, dist);
	if (outside == 3)
		return (0);
	if (outside == 0)
	{
		out_tri[0] = in_tri;
		return (1);
	}
	sort_quaternion_vector3_by_dist(dist, in_tri.p, in_tri.t);
	if (outside == 1)
		return (clip_quad_to_triangles(plane_p, plane_n, in_tri, out_tri));
	if (outside == 2)
		return (clip_triangle(plane_p, plane_n, in_tri, out_tri));
	return (0);
}
