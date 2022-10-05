/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 06:23:41 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/05 14:01:14 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool	v2cmp(int v[2], int ov[2])
{
	return (v[X] == ov[X] && v[Y] == ov[Y]);
}

void	v2cpy(int to[2], int from[2]) 
{
	to[X] = from[X];
	to[Y] = from[Y];
}

void	v2mul_to_f2(int v[2], float mul, float f[2])
{
	f[X] = (float)v[X] * mul;
	f[Y] = (float)v[Y] * mul;
}

void	v2mul(int v[2], int mul)
{
	v[X] *= mul;
	v[Y] *= mul;
}

void	f2mul(float f[2], float mul)
{
	f[X] *= mul;
	f[Y] *= mul;
}

void	v2add(int v[2], int ov[2])
{
	v[X] += ov[X];
	v[Y] += ov[Y];
}