/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:43:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 12:06:14 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

int	point_sqr_magnitude(t_point point)
{
	return ((point.x * point.x) + (point.y * point.y));
}

float	point_sqr_fmagnitude(t_point point)
{
	return ((point.x * point.x) + (point.y * point.y));
}

t_point	point_normalise(t_point point)
{
	t_vector2	v2;

	v2 = point_to_vector2(point);
	v2 = vector2_normalise(v2);
	v2 = vector2_mul(v2, 3.0f);
	return (vector2_to_point(v2));
}
