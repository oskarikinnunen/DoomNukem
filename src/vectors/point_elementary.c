/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_elementary.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:28:15 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 12:05:39 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_point	point_add(t_point first, t_point second)
{
	t_point	result;

	result.x = first.x + second.x;
	result.y = first.y + second.y;
	return (result);
}

t_point	point_sub(t_point first, t_point second)
{
	t_point	result;

	result.x = first.x - second.x;
	result.y = first.y - second.y;
	return (result);
}

t_point	point_add_xy(t_point vec, int add)
{
	t_point	result;

	result.x = vec.x + add;
	result.y = vec.y + add;
	return (result);
}

t_point	point_mul(t_point vec, int mul)
{
	t_point	result;

	result.x = vec.x * mul;
	result.y = vec.y * mul;
	return (result);
}

//TODO: maybe log this error or something
t_point	point_div(t_point vec, int div)
{
	t_point	result;

	if (div == 0)
		return (point_zero());
	result.x = vec.x / div;
	result.y = vec.y / div;
	return (result);
}
