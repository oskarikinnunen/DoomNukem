/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_functions2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 19:31:26 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/23 18:45:13 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

t_vector3	normal_calc_quaternion(t_quaternion p[3])
{
	t_vector3	normal;
	t_vector3	line1;
	t_vector3	line2;

	line1 = vector3_sub(p[1].v, p[0].v);
	line2 = vector3_sub(p[2].v, p[0].v);
	normal = vector3_crossproduct(line1, line2);
	normal = vector3_normalise(normal);
	return (normal);
}

t_vector3	normal_calc(t_vector3 p[3])
{
	t_vector3	normal;
	t_vector3	line1;
	t_vector3	line2;

	line1 = vector3_sub(p[1], p[0]);
	line2 = vector3_sub(p[2], p[0]);
	normal = vector3_crossproduct(line1, line2);
	normal = vector3_normalise(normal);
	return (normal);
}

void	sort_quaternion_vector3_by_dist(
	float dist[3], t_quaternion q[3], t_vector3 t[3])
{
	int				i;
	int				j;
	float			key;
	t_quaternion	temp_q;
	t_vector3		temp_t;

	i = 1;
	while (i < 3)
	{
		key = dist[i];
		temp_q = q[i];
		temp_t = t[i];
		j = i - 1;
		while (j >= 0 && dist[j] > key)
		{
			dist[j + 1] = dist[j];
			q[j + 1] = q[j];
			t[j + 1] = t[j];
			j = j - 1;
		}
		dist[j + 1] = key;
		q[j + 1] = temp_q;
		t[j + 1] = temp_t;
		i++;
	}
}

void	sort_vector2_vector3_by_dist(
	float dist[3], t_vector2 p[3], t_vector3 t[3])
{
	int				i;
	int				j;
	float			key;
	t_vector2		temp_p;
	t_vector3		temp_t;

	i = 1;
	while (i < 3)
	{
		key = dist[i];
		temp_p = p[i];
		temp_t = t[i];
		j = i - 1;
		while (j >= 0 && dist[j] > key)
		{
			dist[j + 1] = dist[j];
			p[j + 1] = p[j];
			t[j + 1] = t[j];
			j = j - 1;
		}
		dist[j + 1] = key;
		p[j + 1] = temp_p;
		t[j + 1] = temp_t;
		i++;
	}
}
