/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_vectors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 13:13:28 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/31 17:22:26 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

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
	printf("%f, %f, %f\n", v.x, v.y, v.z);
}

void printf_quat(t_quaternion q)
{
	if (DEBUG_ON == 0)
		return;
	printf("%f, %f, %f, %f\n", q.v.x, q.v.y, q.v.z, q.w);
}

void printf_tri(t_triangle tri)
{
	if (DEBUG_ON == 0)
		return;
	printf("printf t_triangle\n");
	printf_quat(tri.p[0]);
	printf_quat(tri.p[1]);
	printf_quat(tri.p[2]);
}

void printf_texture(t_texture t)
{
	if (DEBUG_ON == 0)
		return;
	printf("%f, %f, %f\n", t.u, t.v, t.w);
}

void printf_face(void *face)
{
	if (DEBUG_ON == 0)
		return;
	uint32_t *f;

	printf("printf t_face\n");
	f = ((t_face *)face)->v_indices;
	printf("v_indices %d, %d, %d\n", f[0], f[1], f[2]);
	f = ((t_face *)face)->uv_indices;
	printf("uv_indices %d, %d, %d\n", f[0], f[1], f[2]);
}
