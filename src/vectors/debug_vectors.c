/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_vectors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 13:13:28 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/09 14:57:12 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

//TODO: REMOVE When doomnukem is finished, debug functions are using illegal functions like printf
void print_matrix(t_mat4x4 m)
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

void print_point(t_point p)
{
	if (DEBUG_ON == 0)
		return;
	printf("x %d y %d\n", p.x, p.y);
}

void print_vector3(t_vector3 v)
{
	if (DEBUG_ON == 0)
		return;
	printf("%f, %f, %f\n", v.x, v.y, v.z);
}

void print_vector2(t_vector2 v)
{
	if (DEBUG_ON == 0)
		return;
	printf("%f, %f\n", v.x, v.y);
}

void print_quat(t_quaternion q)
{
	if (DEBUG_ON == 0)
		return;
	printf("%f, %f, %f, %f\n", q.v.x, q.v.y, q.v.z, q.w);
}

void print_tri(t_triangle tri)
{
	if (DEBUG_ON == 0)
		return;
	printf("printf t_triangle\n");
	print_quat(tri.p[0]);
	print_quat(tri.p[1]);
	print_quat(tri.p[2]);
}

void print_texture(t_texture t)
{
	if (DEBUG_ON == 0)
		return;
	printf("%f, %f, %f\n", t.u, t.v, t.w);
}

void print_face(void *face)
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

void print_render_statistics(t_render_statistics rs)
{
	if (DEBUG_ON == 0)
		return;
	printf("render count %d\ntriangle count %d\n", rs.render_count, rs.triangle_count);
}
