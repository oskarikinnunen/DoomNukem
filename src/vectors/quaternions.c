/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:34:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/01 16:22:02 by vlaine           ###   ########.fr       */
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

t_texture lerp_texture(t_texture from, t_texture to, float delta)
{
	from.u = ft_flerp(from.u, to.u, delta);
	from.v = ft_flerp(from.v, to.v, delta);
	from.w = ft_flerp(from.w, to.w, delta);

	return(from);
}

t_quaternion lerp_quaternion(t_quaternion from, t_quaternion to, float delta)
{
	from.v.x = ft_flerp(from.v.x, to.v.x, delta);
	from.v.y = ft_flerp(from.v.y, to.v.y, delta);
	from.v.z = ft_flerp(from.v.z, to.v.z, delta);
	from.w = ft_flerp(from.w, to.w, delta);
	return(from);
}

t_point lerp_point(t_point from, t_point to, float delta)
{
	from.x = ft_flerp(from.x, to.x, delta);
	from.y = ft_flerp(from.y, to.y, delta);
	return(from);
}

//TODO: Check if quaternion linestart and lineend could be replaced with t_vector3
t_quaternion quaternion_intersectplane(t_vector3 plane_p, t_vector3 plane_n, t_quaternion lineStart, t_quaternion lineEnd, float *t)
{
	plane_n = vector3_normalise(plane_n);
	float plane_d = -vector3_dot(plane_n, plane_p);
	float ad = vector3_dot(lineStart.v, plane_n);
	float bd = vector3_dot(lineEnd.v, plane_n);
	*t = (-plane_d - ad) / (bd - ad);
	t_vector3 lineStartToEnd = vector3_sub(lineEnd.v, lineStart.v);
	t_vector3 lineToIntersect = vector3_mul(lineStartToEnd, *t);
	return ((t_quaternion){vector3_add(lineStart.v, lineToIntersect), 1.0f});
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