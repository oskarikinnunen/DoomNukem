/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_gizmo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:49:05 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 23:17:20 by raho             ###   ########.fr       */
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

// TODO: Replace with a new function dont fix norm
bool	clip_line_against_lineplane(t_line *line, t_line plane)
{
	float	f_dist1;
	float	f_dist2;
	float	lerp;
	t_line	temp;

	temp.start = plane.end;
	temp.end = plane.start;
	f_dist1 = vector2_fdist_to_plane(line->start, plane.end, plane.start);
	f_dist2 = vector2_fdist_to_plane(line->end, plane.end, plane.start);
	if (f_dist1 < 0.0f && f_dist2 < 0.0f)
		return (false);
	if (f_dist1 < 0.0f)
	{
		lerp = vector2_line_intersect_plane(\
				plane.start, plane.end, line->start, line->end);
		line->start = vector2_lerp(line->start, line->end, lerp);
	}
	else if (f_dist2 < 0.0f)
	{
		lerp = vector2_line_intersect_plane(\
				plane.start, plane.end, line->start, line->end);
		line->end = vector2_lerp(line->start, line->end, lerp);
	}
	return (true);
}

// TODO: Replace with a new function dont fix norm
bool	clip_quatline_to_zplane(t_quat_line *ql)
{
	t_vector3		pp;
	t_vector3		pn;
	float			lerp;
	float			fdist1;
	float			fdist2;

	pp = (t_vector3){.z = 0.1f};
	pn = (t_vector3){.z = 1.0f};
	fdist1 = vector3_fdist_to_plane(ql->start.v, pn, pp);
	fdist2 = vector3_fdist_to_plane(ql->end.v, pn, pp);
	if (fdist1 < 0.0f && fdist2 < 0.0f)
		return (false);
	if (fdist1 < 0.0f || fdist2 < 0.0f)
	{
		if (fdist1 < 0.0f)
		{
			lerp = line_intersect_plane(pp, pn, ql->start.v, ql->end.v);
			ql->start = lerp_quaternion(ql->start, ql->end, lerp);
		}
		else
		{
			lerp = line_intersect_plane(pp, pn, ql->end.v, ql->start.v);
			ql->end = lerp_quaternion(ql->end, ql->start, lerp);
		}
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
			render_ray(sdl, edges[i - 1], edges[i]);
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

// TODO: Name better
static int	render_ray_helper(t_line *l, t_sdlcontext *sdl)
{
	t_line	plane1;
	t_line	plane2;
	t_line	plane3;
	t_line	plane4;

	plane1 = newline((t_vector2){0.0f, 0.0f}, (t_vector2){0.0f, 1.0f});
	plane2 = newline((t_vector2){0.0f, (float)(sdl->window_h) - 1.0f}, \
					(t_vector2){0.0f, -1.0f});
	plane3 = newline((t_vector2){0.0f, 0.0f}, (t_vector2){1.0f, 0.0f});
	plane4 = newline((t_vector2){(float)(sdl->window_w) - 1.0f, 0.0f}, \
					(t_vector2){-1.0f, 0.0f});
	if (!clip_line_against_lineplane(l, plane1))
		return (1);
	if (!clip_line_against_lineplane(l, plane2))
		return (1);
	if (!clip_line_against_lineplane(l, plane3))
		return (1);
	if (!clip_line_against_lineplane(l, plane4))
		return (1);
	return (0);
}

// TODO: Redo this?
void	render_ray(t_sdlcontext *sdl, t_vector3 from, t_vector3 to)
{
	t_quaternion	q1;
	t_quaternion	q2;
	t_line			l;
	t_quat_line		ql;
	
	if (vector3_has_nan(from) || vector3_has_nan(to))
		return ;
	q1 = vector3_to_quaternion(from);
	q2 = vector3_to_quaternion(to);
	q1 = quaternion_mul_matrix(sdl->render.camera.mat_view, q1);
	q2 = quaternion_mul_matrix(sdl->render.camera.mat_view, q2);
	q1 = quaternion_mul_matrix(sdl->render.camera.mat_proj, q1);
	q2 = quaternion_mul_matrix(sdl->render.camera.mat_proj, q2);
	ql.start = q1;
	ql.end = q2;
	if (!clip_quatline_to_zplane(&ql))
		return ;
	l.start = proj_quaternion_to_screenspace(sdl, ql.start);
	l.end = proj_quaternion_to_screenspace(sdl, ql.end);
	if (render_ray_helper(&l, sdl) == 1)
		return ;
	drawline(*sdl, vector2_to_point(l.start), \
			vector2_to_point(l.end), sdl->render.gizmocolor);
}

void render_ray3d(t_sdlcontext *sdl, t_vector3 from,
					t_vector3 to, uint32_t clr)
{
	sdl->render.gizmocolor = clr;
	render_ray(sdl, from, to);
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

static t_vector3 look_direction2(t_vector2 angle)
{
	t_quaternion	temp;
	t_mat4x4		matcamerarot;

	matcamerarot = matrix_make_rotation_z(angle.x);
	temp = quaternion_mul_matrix(matcamerarot, \
			(t_quaternion){1.0f, 0.0f, 0.0f, 1.0f});
	return (temp.v);
}
