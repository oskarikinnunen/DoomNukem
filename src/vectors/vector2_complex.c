/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:43:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/07 03:21:08 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

float	vector2_sqr_magnitude(t_vector2 vec)
{
	return ((vec.x * vec.x) + (vec.y * vec.y));
}

t_vector2 vector2_normalise(t_vector2 vec)
{
	float l = vector2_magnitude(vec);
	return((t_vector2){ vec.x / l, vec.y / l});
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

float	vector2_dot(t_vector2 first, t_vector2 second)
{
	return ((first.x * second.x) + (first.y * second.y));
}

bool	vector2_cmp(t_vector2 first, t_vector2 second)
{
	return ((first.x == second.x) && (first.y == second.y));
}
