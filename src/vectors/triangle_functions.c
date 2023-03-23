/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:37:35 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/23 18:45:59 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector2	get_max_vector2_from_vector3_triangle(t_vector2 max, t_vector3 t[3])
{
	t_vector3	uv;
	int			i;

	i = 0;
	while (i < 3)
	{
		uv = t[i];
		if (uv.x > max.x)
			max.x = uv.x;
		if (uv.y > max.y)
			max.y = uv.y;
		i++;
	}
	return (max);
}

t_vector2	get_max_from_vector2_triangle(t_vector2 p[3])
{
	int			i;
	t_vector2	max;

	max = p[0];
	i = 1;
	while (i < 3)
	{
		max.x = ft_max(max.x, p[i].x);
		max.y = ft_max(max.y, p[i].y);
		i++;
	}
	return (max);
}

t_vector2	get_min_from_vector2_triangle(t_vector2 p[3])
{
	int			i;
	t_vector2	min;

	min = p[0];
	i = 1;
	while (i < 3)
	{
		min.x = ft_min(min.x, p[i].x);
		min.y = ft_min(min.y, p[i].y);
		i++;
	}
	return (min);
}

float	clockwise(t_vector2 p[3])
{
	return ((p[1].x - p[0].x) * (p[2].y - p[0].y) - \
	(p[2].x - p[0].x) * (p[1].y - p[0].y));
}
