/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3_basics1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:31:46 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 20:48:15 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../v3lib.h"

//Adds right side to left side
void	v3_add(float v3[3], float add[3])
{
	v3[X] += add[X];
	v3[Y] += add[Y];
	v3[Z] += add[Z];
}

void	v3_mul(float mul, float *v3)
{
	v3[X] = v3[X] * mul;
	v3[Y] = v3[Y] * mul;
	v3[Z] = v3[Z] * mul;
}

void	v3_matrix_mul(float matrix[3][3], float *v3)
{
	float	t[3];
	int		index;

	index = X;
	while (index <= Z)
	{
		t[index]
			= (v3[X] * matrix[0][index])
			+ (v3[Y] * matrix[1][index])
			+ (v3[Z] * matrix[2][index]);
		index++;
	}
	v3[X] = t[X];
	v3[Y] = t[Y];
	v3[Z] = t[Z];
}

//TODO: consistent naming
void	v3listmul(float matrix[3][3], float **v3s, int len)
{
	while (len >= 0)
		v3_matrix_mul(matrix, v3s[len--]);
}

void	v3listadd(float **v3s, float *add, int len)
{
	while (len >= 0)
		v3_add(v3s[len--], add);
}
