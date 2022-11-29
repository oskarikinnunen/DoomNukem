/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_complex3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:21:41 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/21 16:23:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector3	vector3_movetowards(t_vector3 vec, t_vector3 dir, float delta)
{
	dir = vector3_clamp_magnitude(dir, delta);
	return (vector3_add(vec, dir));
}

t_quaternion quaternion_rotate_euler(t_vector3 original, t_vector3 eulers)
{
	t_quaternion	temp;
	t_mat4x4		matrotation;

	temp.v = original;
	temp.w = 1.0f;
	matrotation = matrix_makerotationy(eulers.y);
	temp = quaternion_mul_matrix(matrotation, temp);
	matrotation = matrix_makerotationz(eulers.x);
	temp = quaternion_mul_matrix(matrotation, temp);
	matrotation = matrix_makerotationx(eulers.z);
	temp = quaternion_mul_matrix(matrotation, temp);
	return (temp);
}

t_vector3 vector3_rotate_euler(t_vector3 original, t_vector3 eulers)
{
	t_quaternion	temp;
	t_mat4x4		matrotation;

	matrotation = matrix_makerotationy(eulers.y);
	temp.v = original;
	temp.w = 1.0f;
	temp = quaternion_mul_matrix(matrotation, temp);
	matrotation = matrix_makerotationz(eulers.x);
	temp = quaternion_mul_matrix(matrotation, temp);
	matrotation = matrix_makerotationx(eulers.z);
	temp = quaternion_mul_matrix(matrotation, temp);
	return (temp.v);
}