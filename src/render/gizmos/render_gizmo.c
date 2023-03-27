/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_gizmo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:49:05 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 12:24:06 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"

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

void	render_ray3d(
	t_sdlcontext *sdl, t_vector3 from, t_vector3 to, uint32_t clr)
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
	drawline(*sdl, vector2_to_point(l.start), \
			vector2_to_point(l.end), clr);
}

void	render_gizmo3d(t_sdlcontext *sdl, t_vector3 pos,
					int size, uint32_t color)
{
	drawcircle(*sdl, vector3_to_screenspace(pos, *sdl), size, color);
}

void	render_gizmo2d(t_sdlcontext *sdl, t_vector2 pos,
						int size, uint32_t color)
{
	drawcircle(*sdl,
		vector3_to_screenspace(v2tov3(pos), *sdl), size, color);
}
