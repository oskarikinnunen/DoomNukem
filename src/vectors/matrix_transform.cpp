/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_transform.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 12:07:19 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:51:49 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_mat4x4	matrix_make_scale(t_vector3 v)
{
	t_mat4x4	matrix;

	matrix = matrix_make_identity();
	matrix.m[0][0] = v.x;
	matrix.m[1][1] = v.y;
	matrix.m[2][2] = v.z;
	return (matrix);
}

t_mat4x4	matrix_make_translation(t_vector3 v)
{
	t_mat4x4	matrix;

	matrix = matrix_make_identity();
	matrix.m[3][0] = v.x;
	matrix.m[3][1] = v.y;
	matrix.m[3][2] = v.z;
	return (matrix);
}

static t_vector3	relative(t_vector2 angle, t_vector3 pos)
{
	t_quaternion	temp;
	t_mat4x4		matcamerarot;

	matcamerarot = matrix_make_rotation_y(angle.y);
	temp = quaternion_mul_matrix(matcamerarot,
			(t_quaternion){pos.x, pos.y, pos.z, 1.0f});
	matcamerarot = matrix_make_rotation_z(angle.x);
	temp = quaternion_mul_matrix(matcamerarot, temp);
	return (temp.v);
}
