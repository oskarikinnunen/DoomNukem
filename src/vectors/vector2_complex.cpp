/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:43:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:50:52 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

float	vector2_sqr_magnitude(t_vector2 vec)
{
	return ((vec.x * vec.x) + (vec.y * vec.y));
}

float	vector2_sqr_dist(t_vector2 first, t_vector2 second)
{
	t_vector2	diff;

	diff = vector2_sub(first, second);
	return (vector2_sqr_magnitude(diff));
}

t_vector2	vector2_normalise(t_vector2 vec)
{
	float	l;

	l = vector2_magnitude(vec);
	return ((t_vector2){vec.x / l, vec.y / l});
}

float	vector2_magnitude(t_vector2 vec)
{
	return (sqrt((vec.x * vec.x) + (vec.y * vec.y)));
}

float	vector2_dist(t_vector2 first, t_vector2 second)
{
	return (
		sqrt
		((first.x - second.x) * (first.x - second.x)
			+ (first.y - second.y) * (first.y - second.y)));
}
