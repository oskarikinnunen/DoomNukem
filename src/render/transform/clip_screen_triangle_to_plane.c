/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clip_screen_triangle.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 18:49:09 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/23 18:54:02 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int point_clip_triangle(t_vector2 plane_p, t_vector2 plane_n, t_screen_triangle in_tri, t_screen_triangle *out_tri)
{
	float	t;

	t = vector2_line_intersect_plane(plane_p, plane_n, (t_vector2){in_tri.p[0].x, in_tri.p[0].y}, (t_vector2){in_tri.p[2].x, in_tri.p[2].y});
	out_tri->p[0] = vector2_lerp(in_tri.p[0], in_tri.p[2], t);
	out_tri->t[0] = vector3_lerp(in_tri.t[0], in_tri.t[2], t);
	t = vector2_line_intersect_plane(plane_p, plane_n, (t_vector2){in_tri.p[1].x, in_tri.p[1].y}, (t_vector2){in_tri.p[2].x, in_tri.p[2].y});
	out_tri->p[1] = vector2_lerp(in_tri.p[1], in_tri.p[2], t);
	out_tri->t[1] = vector3_lerp(in_tri.t[1], in_tri.t[2], t);
	out_tri->p[2] = in_tri.p[2];
	out_tri->t[2] = in_tri.t[2];

	return(1);
}

static int point_clip_quad_to_triangles(t_vector2 plane_p, t_vector2 plane_n, t_screen_triangle in_tri, t_screen_triangle *out_tri)
{
	float	t;

	t = vector2_line_intersect_plane(plane_p, plane_n, (t_vector2){in_tri.p[0].x, in_tri.p[0].y}, (t_vector2){in_tri.p[1].x, in_tri.p[1].y});
	out_tri[0].p[0] = vector2_lerp(in_tri.p[0], in_tri.p[1], t);
	out_tri[0].t[0] = vector3_lerp(in_tri.t[0], in_tri.t[1], t);
	t = vector2_line_intersect_plane(plane_p, plane_n, (t_vector2){in_tri.p[0].x, in_tri.p[0].y}, (t_vector2){in_tri.p[2].x, in_tri.p[2].y});
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

	return(2);
}

int clip_screen_triangle_plane(t_vector2 plane_p, t_vector2 plane_n, t_screen_triangle in_tri, t_screen_triangle out_tri[2])
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
		dist[i] = vector2_fdist_to_plane(in_tri.p[i], plane_n, plane_p);
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
	sort_vector2_vector3_by_dist(dist, in_tri.p, in_tri.t);
	if (outside == 1)
		return (point_clip_quad_to_triangles(plane_p, plane_n, in_tri, out_tri));
	if (outside == 2)
		return (point_clip_triangle(plane_p, plane_n, in_tri, out_tri));
	return(0);
}