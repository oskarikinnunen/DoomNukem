/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_shorthands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 18:43:49 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 22:13:30 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_mat4x4	matrix_zero(void)
{
	return ((t_mat4x4){0.0f});
}

t_mat4x4	matrix_make_identity(void)
{
	t_mat4x4	matrix;

	matrix = matrix_zero();
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return (matrix);
}
