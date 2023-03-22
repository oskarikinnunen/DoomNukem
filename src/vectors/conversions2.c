/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversions2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 17:23:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/17 18:47:56 by okinnune         ###   ########.fr       */
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

t_vector3	v2tov3(t_vector2 vec)
{
	return ((t_vector3){vec.x, vec.y, 0.0f});
}

t_vector2	v3tov2(t_vector3 vec)
{
	return ((t_vector2){vec.x, vec.y});
}

t_vector2	point_to_vector2(t_point point)
{
	t_vector2	result;

	result.x = (float)point.x;
	result.y = (float)point.y;
	return (result);
}
