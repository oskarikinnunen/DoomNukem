/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_vectors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 13:13:28 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/26 15:06:57 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

//TODO: REMOVE When doomnukem is finished, debug functions are using illegal functions like printf
void printf_matrix(t_mat4x4 m)
{
	if (DEBUG_ON == 0)
		return;
	int i = 0;
	int j = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			printf("	mat[%d][%d] = %f", i, j, m.m[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

void printf_point(t_point p)
{
	if (DEBUG_ON == 0)
		return;
	printf("x %d y %d\n", p.x, p.y);
}

void printf_vec(t_vector3 v)
{
	if (DEBUG_ON == 0)
		return;
	printf("x %f y %f z %f\n", v.x, v.y, v.z);
}

void printf_quat(t_quaternion v)
{
	if (DEBUG_ON == 0)
		return;
	printf("w %f ", v.w);
	printf_vec(v.v);
}

void printf_tex(t_texture t)
{
	printf("u %f v %f w %f\n", t.u, t.v, t.w);
}

void printf_tri(t_triangle tri)
{
	if (DEBUG_ON == 0)
		return;
	printf("triangle start\n");
	printf_quat(tri.p[0]);
	printf_quat(tri.p[1]);
	printf_quat(tri.p[2]);
	printf_tex(tri.t[0]);
	printf_tex(tri.t[1]);
	printf_tex(tri.t[2]);
	printf("triangle ends\n");
}
