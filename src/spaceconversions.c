/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaceconversions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:31:43 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/29 14:25:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "vectors.h"

t_quaternion	transformed_vector3(t_transform transform, t_vector3 v)
{
	t_quaternion	result;
	t_vector3		forward;
	t_vector3		right;
	t_vector3		up;

	result.v = vector3_mul_vector3(transform.scale, v);
	result = quaternion_rotate_euler(result.v, transform.rotation);
	if (transform.parent != NULL)
	{
		result = quaternion_rotate_euler(result.v, transform.parent->rotation);
		forward = lookdirection((t_vector2){transform.parent->rotation.x, transform.parent->rotation.y});
		up = lookdirection((t_vector2){transform.parent->rotation.x, transform.parent->rotation.y + RAD90});
		right = vector3_crossproduct(forward, vector3_up());
		forward = vector3_normalise(forward);
		right = vector3_normalise(right);
		up = vector3_normalise(up);
		result.v = vector3_add(result.v, vector3_mul(forward, transform.position.y));
		result.v = vector3_add(result.v, vector3_mul(right, transform.position.x));
		result.v = vector3_add(result.v, vector3_mul(up, transform.position.z));
		result.v = vector3_add(transform.parent->position, result.v);
	}
	else
	{
		result.v = vector3_add(transform.position, result.v);
	}
	return (result);
}

t_vector3	anim_transformed_vector3(t_entity *entity, t_vector3 v)
{
	t_vector3	result;

	/*if (entity->animation.active)
	{
		result = vector3_add(entity->obj->o_anim.frames[entity->animation.frame].deltavertices[index].delta, temp.v);
	}*/
}

t_point vector3_to_screenspace(t_vector3 vec, t_sdlcontext sdl) //TODO: clip
{
	t_camera		c;
	t_quaternion	proj_q;
	t_point			result;

	c = sdl.render.camera;
	proj_q = vector3_to_quaternion(vec);
	proj_q = quaternion_mul_matrix(c.matworld, proj_q);
	proj_q = quaternion_mul_matrix(c.matview, proj_q);
	//clip
	proj_q = quaternion_mul_matrix(c.matproj, proj_q);
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
