/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:34:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/14 17:51:28 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_quaternion	quaternion_angle_axis(float angle, t_vector3 axis)
{
	float			sine;
	t_quaternion	result;

	sine = sinf(angle/2.0f);
	//TODO: normalise axis since we don't care about it's magnitude
	result.x = axis.x * sine;
	result.y = axis.y * sine;
	result.z = axis.z * sine;
	result.w = cosf(angle/2.0f);
	return(result);
}

t_quaternion	quaternion_mul(t_quaternion a, t_quaternion b)
{
	return ((t_quaternion)
	{
		a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
		a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
		a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
		a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w
	}
	);
}

/*static t_vec3d angle_axis(float angle, t_vec3d axis)
{
	double s;
	t_vec3d v = Initv3();

	s = sinf(angle/2.0f);
	v.x = axis.x * s;
	v.y = axis.y * s;
	v.z = axis.z * s;
	v.w = cosf(angle/2.0f);

	return(v);
}

static t_vec3d quaternion_mul(t_vec3d a, t_vec3d b) {
    return ((t_vec3d){
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,  // 1
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,  // i
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,  // j
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w}  // k
	);
}*/