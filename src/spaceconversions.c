/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaceconversions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:31:43 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:43:46 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "vectors.h"

static t_vector3	normalized_look_dir(t_vector2 vector2)
{
	t_vector3	result;

	result = look_direction(vector2);
	return (vector3_normalise(result));
}

t_quaternion	transformed_vector3(t_transform trsform, t_vector3 v)
{
	t_quaternion	res;
	t_vector3		fwd;
	t_vector3		right;
	t_vector3		up;

	res.v = vector3_mul_vector3(trsform.scale, v);
	res = quaternion_rotate_euler(res.v, trsform.rotation);
	if (trsform.parent != NULL)
	{
		res = quaternion_rotate_euler(res.v, trsform.parent->rotation);
		fwd = normalized_look_dir((t_vector2){trsform.parent->rotation.x,
				trsform.parent->rotation.y});
		up = normalized_look_dir((t_vector2){trsform.parent->rotation.x,
				trsform.parent->rotation.y + RAD90});
		right = vector3_cross_product(fwd, vector3_up());
		right = vector3_normalise(right);
		res.v = vector3_add(trsform.parent->position, res.v);
		res.v = vector3_add(res.v, vector3_mul(fwd, trsform.position.y));
		res.v = vector3_add(res.v, vector3_mul(right, trsform.position.x));
		res.v = vector3_add(res.v, vector3_mul(up, trsform.position.z));
	}
	else
		res.v = vector3_add(trsform.position, res.v);
	return (res);
}

t_point	vector3_to_screenspace(t_vector3 vec, t_sdlcontext sdl)
{
	t_camera		c;
	t_quaternion	proj_q;
	t_point			result;
	t_vector3		voffsetview;

	c = sdl.render.camera;
	proj_q = vector3_to_quaternion(vec);
	proj_q = quaternion_mul_matrix(c.mat_view, proj_q);
	proj_q = quaternion_mul_matrix(c.mat_proj, proj_q);
	proj_q.v = vector3_div(proj_q.v, proj_q.w);
	proj_q.v = vector3_negative(proj_q.v);
	voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	proj_q.v = vector3_add(proj_q.v, voffsetview);
	proj_q.v.x *= 0.5f * (float)sdl.window_w;
	proj_q.v.y *= 0.5f * (float)sdl.window_h;
	if (proj_q.w < 0.0f)
		return ((t_point){-100, -100});
	result = (t_point){proj_q.v.x, proj_q.v.y};
	return (result);
}
