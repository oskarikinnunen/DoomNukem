/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3_basics.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:31:46 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/11 09:25:03 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../v3lib.h"

float	*v3_new(float x, float y, float z)
{
	float	*v3;

	v3 = (float *)malloc(sizeof(float) * 3);
	v3[X] = x;
	v3[Y] = y;
	v3[Z] = z;
	return (v3);
}

void	v3_add(float *v3, float *add)
{
	v3[X] += add[X];
	v3[Y] += add[Y];
	v3[Z] += add[Z];
}

void	v3_mul(float matrix[3][3], float *v3)
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
		v3_mul(matrix, v3s[len--]);
}

void	v3listadd(float **v3s, float *add, int len)
{
	while (len >= 0)
		v3_add(v3s[len--], add);
}
