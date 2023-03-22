/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_zbuffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:02:10 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/22 16:47:02 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	screen_edge_clip(t_point_triangle *buff, int side,
t_v2rectangle screen_edge, t_point_triangle triangle)
{
	int	tricount;

	if (side == 0)
		tricount = point_clip_triangle_against_plane(screen_edge.min, \
		(t_vector2){0.0f, 1.0f}, triangle, buff);
	else if (side == 1)
		tricount = point_clip_triangle_against_plane(screen_edge.max, \
		(t_vector2){0.0f, -1.0f}, triangle, buff);
	else if (side == 2)
		tricount = point_clip_triangle_against_plane(screen_edge.min, \
		(t_vector2){1.0f, 0.0f}, triangle, buff);
	else
		tricount = point_clip_triangle_against_plane(screen_edge.max, \
		(t_vector2){-1.0f, 0.0f}, triangle, buff);
	return (tricount);
}

static t_point	clip_triangle(t_point_triangle *triangles,
t_v2rectangle screen_edge)
{
	t_point_triangle	clipped[2];
	t_point				start_end;
	int					p;
	int					nnewtriangles;
	int					ntristoadd;

	start_end = (t_point){0, 1};
	nnewtriangles = 1;
	p = 0;
	while (p < 4)
	{
		ntristoadd = 0;
		while (nnewtriangles > 0)
		{
			nnewtriangles--;
			ntristoadd = screen_edge_clip(
					clipped, p, screen_edge, triangles[start_end.x++]);
			ft_memcpy(&triangles[start_end.y], clipped,
				ntristoadd * sizeof(t_point_triangle));
			start_end.y += ntristoadd;
		}
		nnewtriangles = start_end.y - start_end.x;
		p++;
	}
	return (start_end);
}

static void	clip_and_render_triangle(
	t_lighting *lighting, t_point_triangle triangle)
{
	t_point_triangle	triangles[32];
	t_point				start_end;

	triangles[0] = triangle;
	start_end = clip_triangle(triangles, lighting->screen_edge);
	while (start_end.x < start_end.y)
	{
		lighting->triangle = triangles[start_end.x];
		rasterize_zbuffer(lighting);
		start_end.x++;
	}
}

t_point_triangle	triangle_to_screen_point_triangle(
	t_mat4x4 matproj, t_triangle clipped, t_vector2 size)
{
	t_triangle			triprojected;
	t_point_triangle	tri;
	int					i;
	const t_vector3		voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};

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
	tri.clr = clipped.clr;
	return (tri);
}

void	render_zbuffer(t_lighting *l, t_entity *entity)
{
	t_triangle		clipped[2];
	t_triangle		world_triangle;
	int				nclippedtriangles;
	int				index;
	int				i;

	if (entity->obj == NULL || entity->world_triangles == NULL)
		return ;
	l->screen_edge.max = l->resolution;
	l->screen_edge.min = vector2_zero();
	i = 0;
	while (i < entity->obj->face_count)
	{
		world_triangle = triangle_to_viewspace(entity->world_triangles[i], \
		l->camera.matview);
		nclippedtriangles = clip_triangle_plane((t_vector3){0.0f, 0.0f, 0.1f}, \
		(t_vector3){0.0f, 0.0f, 1.0f}, world_triangle, clipped);
		index = 0;
		while (index < nclippedtriangles)
		{
			clip_and_render_triangle(l, triangle_to_screen_point_triangle(
					l->camera.matproj, clipped[index++], l->resolution));
		}
		i++;
	}
}
