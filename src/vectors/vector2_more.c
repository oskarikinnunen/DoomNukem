/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_more.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 01:54:00 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/11 13:14:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

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

float vector2_anglebetween(t_vector2 first, t_vector2 second)
{
	t_vector2 temp = vector2_sub(second, first);
	return (atan2f(temp.y, temp.x));
}