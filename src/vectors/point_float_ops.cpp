/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_float_ops.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:19:29 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 11:51:46 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_point	point_fmul(t_point point, float mul)
{
	t_point	result;

	result.x = point.x * mul;
	result.y = point.y * mul;
	return (result);
}

t_point	point_fdiv(t_point point, float div)
{
	t_point	result;

	result.x = point.x / div;
	result.y = point.y / div;
	return (result);
}
