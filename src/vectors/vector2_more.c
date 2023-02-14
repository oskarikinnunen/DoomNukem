/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_more.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 01:54:00 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/11 15:58:33 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

int			int_snap(int i, int interval)
{
	
}

t_vector2	vector2_snap(t_vector2 vec, int interval)
{
	t_vector2	result;
	t_vector2	remains;

	remains.x = fmod(vec.x, interval);
	remains.y = fmod(vec.y, interval);
	result.x = vec.x - remains.x;
	result.y = vec.y - remains.y;
	result.x += (remains.x > interval / 2) * interval;
	result.y += (remains.y > interval / 2) * interval;
	return (result);
}

static bool	vector2_nan_or_inf(t_vector2 vec)
{
	if (isinf(vec.x) || isnan(vec.x))
		return (true);
	if (isinf(vec.y) || isnan(vec.y))
		return (true);
	return (false);
}

float vector2_anglebetween(t_vector2 first, t_vector2 second)
{
	t_vector2 temp = vector2_sub(second, first);
	if (vector2_nan_or_inf(first) || vector2_nan_or_inf(second))
	{
		exit(0);
	}
	return (atan2f(temp.y, temp.x));
}