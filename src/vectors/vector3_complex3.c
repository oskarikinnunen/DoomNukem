/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_complex3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:21:41 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 20:03:55 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_quaternion	quaternion_rotate_euler(t_vector3 original, t_vector3 eulers)
{
	t_quaternion	temp;
	t_mat4x4		matrotation;

	temp.v = original;
	temp.w = 1.0f;
	matrotation = matrix_make_rotation_y(eulers.y);
	temp = quaternion_mul_matrix(matrotation, temp);
	matrotation = matrix_make_rotation_z(eulers.x);
	temp = quaternion_mul_matrix(matrotation, temp);
	matrotation = matrix_make_rotation_x(eulers.z);
	temp = quaternion_mul_matrix(matrotation, temp);
	return (temp);
}

t_vector3	vector3_lerp(t_vector3 v1, t_vector3 v2, float lerp)
{
	t_vector3	result;

	result.x = v1.x - (lerp * (v1.x - v2.x));
	result.y = v1.y - (lerp * (v1.y - v2.y));
	result.z = v1.z - (lerp * (v1.z - v2.z));
	return (result);
}

float	line_intersect_plane(t_vector3 plane_p,
		t_vector3 plane_n, t_vector3 start, t_vector3 end)
{
	float	plane_d;
	float	ad;
	float	bd;

	plane_n = vector3_normalise(plane_n);
	plane_d = -vector3_dot(plane_n, plane_p);
	ad = vector3_dot(start, plane_n);
	bd = vector3_dot(end, plane_n);
	if (bd - ad == 0.0f)
		return (1.0f);
	return ((-plane_d - ad) / (bd - ad));
}

float	vector2_line_intersect_plane(t_vector2 plane_p,
	t_vector2 plane_n, t_vector2 start, t_vector2 end)
{
	float	plane_d;
	float	ad;
	float	bd;

	plane_n = vector2_normalise(plane_n);
	plane_d = -vector2_dot(plane_n, plane_p);
	ad = vector2_dot(start, plane_n);
	bd = vector2_dot(end, plane_n);
	if (bd - ad == 0.0f)
		return (1.0f);
	return ((-plane_d - ad) / (bd - ad));
}
