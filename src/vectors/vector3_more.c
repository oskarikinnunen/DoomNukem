/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_more.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 11:40:43 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/11 12:31:53 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector3 vector3_snap(t_vector3 vec, int interval)
{
	t_vector3	result;
	t_vector3	remains;

	remains.x = fmod(vec.x, interval);
	remains.y = fmod(vec.y, interval);
	remains.z = fmod(vec.z, interval);
	result.x = vec.x - remains.x;
	result.y = vec.y - remains.y;
	result.z = vec.z - remains.z;
	result.x += (remains.x > interval / 2) * interval;
	result.y += (remains.y > interval / 2) * interval;
	result.z += (remains.z > interval / 2) * interval;
	return (result);
}