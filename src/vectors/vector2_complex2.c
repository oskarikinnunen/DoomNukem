/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_complex2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:43:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 11:07:16 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "room.h" //Has line definition... TODO: move line to shapes.h

float	vector2_dot(t_vector2 first, t_vector2 second)
{
	return ((first.x * second.x) + (first.y * second.y));
}

float	vector2_cross(t_vector2 first, t_vector2 second)
{
	return ((first.x * second.y) + (first.y * second.x));
}

t_vector2	vector2_lerp(t_vector2 v1, t_vector2 v2, float lerp)
{
	t_vector2	result;

	result.x = v1.x - (lerp * (v1.x - v2.x));
	result.y = v1.y - (lerp * (v1.y - v2.y));
	return (result);
}

t_vector2	vector2_crossproduct(t_vector2 v1, t_vector2 v2)
{
	t_vector2	v;

	v.x = v1.y * v2.x - v1.x * v2.y;
	v.y = v1.x * v2.y - v1.y * v2.x;
	return (v);
}

bool	vector2_cmp(t_vector2 first, t_vector2 second)
{
	return ((first.x == second.x) && (first.y == second.y));
}
