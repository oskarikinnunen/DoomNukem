/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:43:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/21 17:13:26 by vlaine           ###   ########.fr       */
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

float	vector2_cross(t_vector2 first, t_vector2 second)
{
	return ((first.x * second.y) + (first.y * second.x));
}

t_vector2	vector2_lerp(t_vector2 v1, t_vector2 v2, float lerp)
{
	t_vector2 result;

	//lerp = lerp * vector2_dist(v1, v2);
	result.x = v1.x - (lerp * (v1.x - v2.x));
	result.y = v1.y - (lerp * (v1.y - v2.y));
	return (result);
}

t_vector2 vector2_crossproduct(t_vector2 v1, t_vector2 v2)
{
	t_vector2 v;
	v.x = v1.y * v2.x - v1.x * v2.y;
	v.y = v1.x * v2.y - v1.y * v2.x;
	    //return (v1.X*v2.Y) - (v1.Y*v2.X);

	return v;
}

bool	vector2_cmp(t_vector2 first, t_vector2 second)
{
	return ((first.x == second.x) && (first.y == second.y));
}

float vector2_fdist_to_plane(t_vector2 p, t_vector2 plane_n, t_vector2 plane_p)
{
	t_vector2 n = vector2_normalise(p);
	return (plane_n.x * p.x + plane_n.y * p.y - vector2_dot(plane_n, plane_p));
}
