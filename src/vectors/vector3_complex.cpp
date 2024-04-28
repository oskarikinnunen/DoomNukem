/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:43:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 11:44:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

float	vector3_sqr_magnitude(t_vector3 vec)
{
	return ((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

float	vector3_sqr_dist(t_vector3 first, t_vector3 second)
{
	t_vector3	diff;

	diff = vector3_sub(first, second);
	return (vector3_sqr_magnitude(diff));
}

float	vector3_magnitude(t_vector3 vec)
{
	return (sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z)));
}

float	vector3_dist(t_vector3 first, t_vector3 second)
{
	return (
		sqrt
		((first.x - second.x) * (first.x - second.x)
			+ (first.y - second.y) * (first.y - second.y)
			+ (first.z - second.z) * (first.z - second.z)));
}

float	vector3_dot(t_vector3 first, t_vector3 second)
{
	return ((first.x * second.x)
		+ (first.y * second.y) + (first.z * second.z));
}
