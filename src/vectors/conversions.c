/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 17:23:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 13:27:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_point	vector2_to_point(t_vector2 vec)
{
	t_point	result;

	result.x = (int)vec.x;
	result.y = (int)vec.y;
	return (result);
}

t_vector2	point_to_vector2(t_point point)
{
	t_vector2	result;

	result.x = (float)point.x;
	result.y = (float)point.y;
	return (result);
}