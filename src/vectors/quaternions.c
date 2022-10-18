/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:34:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/18 20:42:25 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_quaternion	quaternion_angle_axis(float angle, t_vector3 axis)
{
	float			sine;
	t_quaternion	result;

	sine = sinf(angle/2.0f);
	//TODO: normalise axis since we don't care about it's magnitude
	result.v.x = axis.x * sine;
	result.v.y = axis.y * sine;
	result.v.z = axis.z * sine;
	result.w = cosf(angle/2.0f);
	return(result);
}

t_quaternion	quaternion_mul(t_quaternion a, t_quaternion b)
{
	return ((t_quaternion)
	{
		a.w * b.w - a.v.x * b.v.x - a.v.y * b.v.y - a.v.z * b.v.z,
		a.w * b.v.x + a.v.x * b.w + a.v.y * b.v.z - a.v.z * b.v.y,
		a.w * b.v.y - a.v.x * b.v.z + a.v.y * b.w + a.v.z * b.v.x,
		a.w * b.v.z + a.v.x * b.v.y - a.v.y * b.v.x + a.v.z * b.w
	}
	);
}

//TODO: Check if quaternion linestart and lineend could be replaced with t_vector3
t_quaternion quaternion_intersectplane(t_vector3 plane_p, t_vector3 plane_n, t_quaternion lineStart, t_quaternion lineEnd)
{
	plane_n = vector3_normalise(plane_n);
	float plane_d = -vector3_dot(plane_n, plane_p);
	float ad = vector3_dot(lineStart.v, plane_n);
	float bd = vector3_dot(lineEnd.v, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	t_vector3 lineStartToEnd = vector3_sub(lineEnd.v, lineStart.v);
	t_vector3 lineToIntersect = vector3_mul(lineStartToEnd, t);
	return ((t_quaternion){vector3_add(lineStart.v, lineToIntersect), 1});
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