/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_gizmo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:49:05 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 11:46:07 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"

static t_vector2	proj_quaternion_to_screenspace(t_sdlcontext *sdl,
													t_quaternion proj_q)
{
	t_vector3	v_offset_view;

	proj_q.v = vector3_div(proj_q.v, proj_q.w);
	proj_q.v = vector3_negative(proj_q.v);
	v_offset_view = (t_vector3){1.0f, 1.0f, 0.0f};
	proj_q.v = vector3_add(proj_q.v, v_offset_view);
	proj_q.v.x *= 0.5f * (float)sdl->window_w;
	proj_q.v.y *= 0.5f * (float)sdl->window_h;
	return ((t_vector2){proj_q.v.x, proj_q.v.y});
}

bool	clip_quatline_to_zplane(t_quat_line *ql)
{
	t_vector3		plane_p;
	t_vector3		plane_n;
	float			lerp;
	float			dist[2];

	plane_p = (t_vector3){0.0f, 0.0f, 0.1f};
	plane_n = (t_vector3){0.0f, 0.0f, 1.0f};
	dist[0] = vector3_fdist_to_plane(ql->start.v, plane_n, plane_p);
	dist[1] = vector3_fdist_to_plane(ql->end.v, plane_n, plane_p);
	if (dist[0] < 0.0f && dist[1] < 0.0f)
		return (false);
	if (dist[0] < 0.0f || dist[1] < 0.0f)
	{
		if (dist[0] > dist[1])
			ft_swap(&ql->start, &ql->end, sizeof(t_quaternion));
		lerp = line_intersect_plane(plane_p, plane_n, ql->start.v, ql->end.v);
		ql->start = lerp_quaternion(ql->start, ql->end, lerp);
	}
	return (true);
}

static t_line	newline(t_vector2 start, t_vector2 end)
{
	t_line	l;

	l.start = start;
	l.end = end;
	return (l);
}

// XY
void	render_circle(t_sdlcontext *sdl, t_vector3 pos,
						float radius, uint32_t clr)
{
	t_vector3	edges[RCRCL_SIDES + 1];
	int			i;
	float		angl;

	i = 0;
	angl = 0.0f;
	sdl->render.gizmocolor = clr;
	while (i < RCRCL_SIDES + 1)
	{
		edges[i].x = pos.x + (sinf(angl) * radius);
		edges[i].y = pos.y + (cosf(angl) * radius);
		edges[i].z = pos.z;
		if (i >= 1)
			render_ray3d(sdl, edges[i - 1], edges[i], clr);
		angl += FULLRAD / RCRCL_SIDES;
		i++;
	}
}

bool	vector3_has_nan(t_vector3 vec)
{
	return (isnan(vec.x) || isinf(vec.x)
		|| isnan(vec.y) || isinf(vec.y)
		|| isnan(vec.z) || isinf(vec.z));
}

void render_ray3d(t_sdlcontext *sdl, t_vector3 from,
					t_vector3 to, uint32_t clr)
{
	t_quaternion	q1;
	t_quaternion	q2;
	t_line			l;
	t_quat_line		ql;

	if (vector3_has_nan(from) || vector3_has_nan(to))
		return ;
	q1 = vector3_to_quaternion(from);
	q2 = vector3_to_quaternion(to);
	q1 = quaternion_mul_matrix(sdl->render.camera.matview, q1);
	q2 = quaternion_mul_matrix(sdl->render.camera.matview, q2);
	q1 = quaternion_mul_matrix(sdl->render.camera.matproj, q1);
	q2 = quaternion_mul_matrix(sdl->render.camera.matproj, q2);
	ql.start = q1;
	ql.end = q2;
	if (!clip_quatline_to_zplane(&ql))
		return ;
	l.start = proj_quaternion_to_screenspace(sdl, ql.start);
	l.end = proj_quaternion_to_screenspace(sdl, ql.end);
	drawline(*sdl, vector2_to_point(l.start), \
			vector2_to_point(l.end), clr);
}

void render_gizmo3d(t_sdlcontext *sdl, t_vector3 pos,
					int size, uint32_t color)
{
	drawcircle(*sdl, vector3_to_screenspace(pos, *sdl), size, color);
}

void render_gizmo2d(t_sdlcontext *sdl, t_vector2 pos,
						int size, uint32_t color)
{
	drawcircle(*sdl,
		vector3_to_screenspace(v2tov3(pos), *sdl), size, color);
}

static t_vector3 lookdirection2(t_vector2 angle)
{
	t_quaternion	temp;
	t_mat4x4		matcamerarot;

	matcamerarot = matrix_makerotationz(angle.x);
	temp = quaternion_mul_matrix(matcamerarot, \
			(t_quaternion){1.0f, 0.0f, 0.0f, 1.0f});
	return (temp.v);
}
