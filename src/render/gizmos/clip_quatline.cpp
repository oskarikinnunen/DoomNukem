/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clip_quatline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:07:22 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 12:13:32 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

t_vector2	proj_quaternion_to_screenspace(
	t_sdlcontext *sdl, t_quaternion proj_q)
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
