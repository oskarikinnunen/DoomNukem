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

t_point	point_normalise(t_point point)
{
	int l = point_magnitude(point);
	if (l != 0)
		return((t_point){point.x / l, point.y / l});
	else
		return (point_zero());
}

int	point_magnitude(t_point point)
{
	return (sqrt((point.x * point.x) + (point.y * point.y)));
}

int	point_dist(t_point first, t_point second)
{
	return (
		sqrt
		(first.x - second.x) * (first.x - second.x)
		+ (first.y - second.y) * (first.y - second.y));
}

int	point_dot(t_point first, t_point second)
{
	return ((first.x * second.x) + (first.y * second.y));
}

bool	point_cmp(t_point first, t_point second)
{
	return ((first.x == second.x) && (first.y == second.y));
}
