/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_zbuffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:02:10 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 20:29:32 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	clip_and_render_triangle(
	t_lighting *lighting, t_screen_triangle triangle)
{
	t_screen_triangle	triangles[32];
	t_point				start_end;

	triangles[0] = triangle;
	start_end = clip_screen_triangle_against_screen_edge(\
						triangles, lighting->screen_edge);
	while (start_end.x < start_end.y)
	{
		lighting->triangle = triangles[start_end.x];
		rasterize_zbuffer(lighting);
		start_end.x++;
	}
}

t_screen_triangle	triangle_to_screen_point_triangle(
	t_mat4x4 matproj, t_world_triangle clipped, t_vector2 size)
{
	t_world_triangle			triprojected;
	t_screen_triangle			tri;
	int							i;
	const t_vector3				voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};

	i = 0;
	while (i < 3)
	{
		triprojected.p[i] = quaternion_mul_matrix(matproj, clipped.p[i]);
		triprojected.t[i] = clipped.t[i];
		tri.t[i].z = 1.0f / triprojected.p[i].w;
		triprojected.p[i].v = \
		vector3_div(triprojected.p[i].v, triprojected.p[i].w);
		triprojected.p[i].v = vector3_negative(triprojected.p[i].v);
		triprojected.p[i].v = vector3_add(triprojected.p[i].v, voffsetview);
		tri.p[i].x = triprojected.p[i].v.x * (size.x * 0.5f);
		tri.p[i].y = triprojected.p[i].v.y * (size.y * 0.5f);
		i++;
	}
	return (tri);
}

void	render_zbuffer(t_lighting *l, t_entity *entity)
{
	t_world_triangle		clipped[2];
	t_world_triangle		world_triangle;
	int						nclippedtriangles;
	int						index;
	int						i;

	if (entity->obj == NULL || entity->world_triangles == NULL)
		return ;
	l->screen_edge.max = l->resolution;
	l->screen_edge.min = vector2_zero();
	i = 0;
	while (i < entity->obj->face_count)
	{
		world_triangle = triangle_to_viewspace(entity->world_triangles[i], \
		l->camera.mat_view);
		nclippedtriangles = clip_triangle_plane((t_vector3){0.0f, 0.0f, 0.1f}, \
		(t_vector3){0.0f, 0.0f, 1.0f}, world_triangle, clipped);
		index = 0;
		while (index < nclippedtriangles)
		{
			clip_and_render_triangle(l, triangle_to_screen_point_triangle(
					l->camera.mat_proj, clipped[index++], l->resolution));
		}
		i++;
	}
}
