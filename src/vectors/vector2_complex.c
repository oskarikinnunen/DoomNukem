/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:43:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 20:08:37 by vlaine           ###   ########.fr       */
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
	return (sqrt((first.x - second.x) * (first.x - second.x)+ (first.y - second.y) * (first.y - second.y)));
}

float	vector2_dot(t_vector2 first, t_vector2 second)
{
	return ((first.x * second.x) + (first.y * second.y));
}

bool	vector2_cmp(t_vector2 first, t_vector2 second)
{
	return ((first.x == second.x) && (first.y == second.y));
}

t_vector2 vector2_lerp(t_vector2 from, t_vector2 to, float delta)
{
	t_vector2	result;

	result.x = flerp(from.x, to.x, delta);	
	result.y = flerp(from.y, to.y, delta);

	return(result);
}
