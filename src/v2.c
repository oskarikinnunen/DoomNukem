/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 06:23:41 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/06 14:10:09 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

/* INT VECTORS */

//ov = other vector
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

//takes difference of v and ov and stores it in rv
// rv = result vector
void	v2diff(int v[2], int ov[2], int rv[2])
{
	rv[X] = v[X] - ov[X];
	rv[Y] = v[Y] - ov[Y];
}

void	v2mul(int v[2], int mul)
{
	v[X] *= mul;
	v[Y] *= mul;
}

//Adds right side to left side
void	v2add(int v[2], int ov[2])
{
	v[X] += ov[X];
	v[Y] += ov[Y];
}

/* FLOATING POINT VECTORS */

float	f2dist(float f[2], float of[2])
{
	float	dist[2];

	dist[X] = f[X] - of[X];
	dist[Y] = f[Y] - of[Y];
	return (sqrt((dist[X] * dist[X]) + (dist[Y] * dist[Y])));
}

/*float	f2dot(float f[2], float of[2])
{
	float	dist;

	dist = f2dist(f, of);
	return ()
}*/

void	f2tov2(float f[2], int v[2])
{
	v[X] = (int)f[X];
	v[Y] = (int)f[Y];
}

void	f2cpy(float to[2], float from[2])
{
	to[X] = from[X];
	to[Y] = from[Y];
}

void	f2mul(float f[2], float mul)
{
	f[X] *= mul;
	f[Y] *= mul;
}

//Adds right side to left side
void	f2add(float f[2], float of[2])
{
	f[X] += of[X];
	f[Y] += of[Y];
}