/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_shorthands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:34:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 12:04:40 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_point	point_zero(void)
{
	return ((t_point){0, 0});
}

t_point	point_one(void)
{
	return ((t_point){1, 1});
}

t_point	point_negative(t_point vec)
{
	t_point	result;

	result.x = -vec.x;
	result.y = -vec.y;
	return (result);
}
