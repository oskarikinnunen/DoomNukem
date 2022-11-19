/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_more.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 01:54:00 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/07 01:55:46 by okinnune         ###   ########.fr       */
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
