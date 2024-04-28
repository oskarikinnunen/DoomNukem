/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_functions3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 12:34:29 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 20:10:41 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

/*t_mat4x4 matrix_lookat(t_vector3 pos, t_vector3 target, t_vector3 up)
{
	t_vector3 newForward = vector3_sub(target, pos);
	newForward = vector3_normalise(newForward);
	t_vector3 a = vector3_mul(newForward, vector3_dot(up, newForward));
	t_vector3 newUp = vector3_sub(up, a);
	newUp = vector3_normalise(newUp);
	t_vector3 newRight = vector3_crossproduct(newUp, newForward);
	t_mat4x4 matrix = matrix_zero();
	matrix.m[0][0] = newRight.x;
	matrix.m[0][1] = newRight.y;
	matrix.m[0][2] = newRight.z;
	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;
	matrix.m[1][1] = newUp.y;
	matrix.m[1][2] = newUp.z;
	matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;
	matrix.m[2][1] = newForward.y;
	matrix.m[2][2] = newForward.z;
	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;
	matrix.m[3][1] = pos.y;
	matrix.m[3][2] = pos.z;
	matrix.m[3][3] = 1.0f;
	return (matrix_quickinverse(matrix));
}*/
static t_vector3	get_new_forward(t_vector3 pos, t_vector3 target)
{
	t_vector3	new_forward;

	new_forward = vector3_sub(target, pos);
	return (vector3_normalise(new_forward));
}

static t_vector3	get_new_up(t_vector3 up, t_vector3 new_forward)
{
	t_vector3	a;
	t_vector3	new_up;

	a = vector3_mul(new_forward, vector3_dot(up, new_forward));
	new_up = vector3_sub(up, a);
	return (vector3_normalise(new_up));
}

static t_vector3	get_new_right(t_vector3 new_up, t_vector3 new_forward)
{
	return (vector3_cross_product(new_up, new_forward));
}

static t_mat4x4	get_matrix(t_vector3 new_right, t_vector3 new_up,
		t_vector3 new_forward, t_vector3 pos)
{
	t_mat4x4	matrix;

	matrix = matrix_zero();
	matrix.m[0][0] = new_right.x;
	matrix.m[0][1] = new_right.y;
	matrix.m[0][2] = new_right.z;
	matrix.m[1][0] = new_up.x;
	matrix.m[1][1] = new_up.y;
	matrix.m[1][2] = new_up.z;
	matrix.m[2][0] = new_forward.x;
	matrix.m[2][1] = new_forward.y;
	matrix.m[2][2] = new_forward.z;
	matrix.m[3][0] = pos.x;
	matrix.m[3][1] = pos.y;
	matrix.m[3][2] = pos.z;
	matrix.m[0][3] = 0.0f;
	matrix.m[1][3] = 0.0f;
	matrix.m[2][3] = 0.0f;
	matrix.m[3][3] = 1.0f;
	return (matrix_quick_inverse(matrix));
}

t_mat4x4	matrix_lookat(t_vector3 pos, t_vector3 target, t_vector3 up)
{
	t_vector3	new_forward;
	t_vector3	new_up;
	t_vector3	new_right;
	t_mat4x4	matrix;

	new_forward = get_new_forward(pos, target);
	new_up = get_new_up(up, new_forward);
	new_right = get_new_right(new_up, new_forward);
	matrix = get_matrix(new_right, new_up, new_forward, pos);
	return (matrix);
}
