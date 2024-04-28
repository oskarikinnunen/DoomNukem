/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:22:51 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 20:03:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_quaternion	quaternion_mul_matrix(t_mat4x4 m, t_quaternion i)
{
	t_quaternion	v;

	v.v.x = i.v.x * m.m[0][0] + i.v.y * m.m[1][0] + i.v.z * m.m[2][0]
		+ i.w * m.m[3][0];
	v.v.y = i.v.x * m.m[0][1] + i.v.y * m.m[1][1] + i.v.z * m.m[2][1]
		+ i.w * m.m[3][1];
	v.v.z = i.v.x * m.m[0][2] + i.v.y * m.m[1][2] + i.v.z * m.m[2][2]
		+ i.w * m.m[3][2];
	v.w = i.v.x * m.m[0][3] + i.v.y * m.m[1][3] + i.v.z * m.m[2][3]
		+ i.w * m.m[3][3];
	return (v);
}

t_vector3	look_direction(t_vector2 angle)
{
	t_quaternion	temp;
	t_mat4x4		matcamerarot;

	matcamerarot = matrix_make_rotation_y(angle.y);
	temp = quaternion_mul_matrix(matcamerarot,
			(t_quaternion){1.0f, 0.0f, 0.0f, 1.0f});
	matcamerarot = matrix_make_rotation_z(angle.x);
	temp = quaternion_mul_matrix(matcamerarot, temp);
	return (temp.v);
}
