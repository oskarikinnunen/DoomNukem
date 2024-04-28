/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 19:24:23 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 20:28:42 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_mat4x4	matrix_make_projection(float fovdegrees, float aspect_ratio,
							float near, float far)
{
	float		fovrad;
	t_mat4x4	matrix;

	matrix = matrix_zero();
	fovrad = 1.0f / tanf(fovdegrees * 0.5f / 180.0f * 3.14159f);
	matrix.m[0][0] = aspect_ratio * fovrad;
	matrix.m[1][1] = fovrad;
	matrix.m[2][2] = far / (far - near);
	matrix.m[3][2] = (-far * near) / (far - near);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;
	return (matrix);
}

t_mat4x4	matrix_multiply_matrix(t_mat4x4 m1, t_mat4x4 m2)
{
	t_mat4x4	matrix;
	int			c;
	int			r;

	matrix = matrix_zero();
	c = 0;
	while (c < 4)
	{
		r = 0;
		while (r < 4)
		{
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1]
				* m2.m[1][c] + m1.m[r][2]
				* m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
			r++;
		}
		c++;
	}
	return (matrix);
}

static inline t_mat4x4	matrix_flip(t_mat4x4 m)
{
	t_mat4x4	matrix;

	matrix.m[0][0] = m.m[0][0];
	matrix.m[0][1] = m.m[1][0];
	matrix.m[0][2] = m.m[2][0];
	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m.m[0][1];
	matrix.m[1][1] = m.m[1][1];
	matrix.m[1][2] = m.m[2][1];
	matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m.m[0][2];
	matrix.m[2][1] = m.m[1][2];
	matrix.m[2][2] = m.m[2][2];
	matrix.m[2][3] = 0.0f;
	return (matrix);
}

// Only for Rotation/Translation Matrices
t_mat4x4	matrix_quick_inverse(t_mat4x4 m)
{
	t_mat4x4	matrix;

	matrix = matrix_flip(m);
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0]
			+ m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1]
			+ m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2]
			+ m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return (matrix);
}

t_vector3	vector3_mul_matrix(t_mat4x4 m, t_vector3 i)
{
	t_vector3	v;

	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + 1 * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + 1 * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + 1 * m.m[3][2];
	return (v);
}
