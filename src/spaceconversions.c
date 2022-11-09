/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaceconversions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:31:43 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/09 05:22:03 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "vectors.h"

t_point worldtoeditorspace(t_editor *ed, t_vector2 worldcrd)
{
	t_point	result;

	result = vector2_to_point(worldcrd);
	//result = point_add(result, ed->offset);
	return (result);
}

t_point vector3_to_screenspace(t_render r, t_vector3 vec, t_sdlcontext sdl)
{
	t_quaternion	proj_q;
	t_point			result;

	proj_q = vector3_to_quaternion(vec);
	proj_q = quaternion_mul_matrix(r.matworld, proj_q);
	proj_q = quaternion_mul_matrix(r.matview, proj_q);
	proj_q = quaternion_mul_matrix(r.matproj, proj_q);
	proj_q.v = vector3_div(proj_q.v, proj_q.w);
	proj_q.v = vector3_negative(proj_q.v);

	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	proj_q.v = vector3_add(proj_q.v, voffsetview);

	proj_q.v.x *= 0.5f * (float)sdl.window_w;
	proj_q.v.y *= 0.5f * (float)sdl.window_h;

	if (proj_q.w < 0.0f)
		return ((t_point) {-100, -100});
	result = (t_point){proj_q.v.x, proj_q.v.y};
	return(result);
}
