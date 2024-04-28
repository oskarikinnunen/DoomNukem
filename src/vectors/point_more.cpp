/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_more.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:03:50 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 11:53:41 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_point	point_abs(t_point point)
{
	return ((t_point){ft_abs(point.x), ft_abs(point.y)});
}

t_point	point_mod(t_point point, int mod)
{
	return ((t_point){point.x % mod, point.y % mod});
}

t_point	point_snap(t_point point, int interval)
{
	t_point	result;
	t_point	remains;

	remains.x = point.x % interval;
	remains.y = point.y % interval;
	result.x = point.x - remains.x;
	result.y = point.y - remains.y;
	result.x += (remains.x > interval / 2) * interval;
	result.y += (remains.y > interval / 2) * interval;
	return (result);
}
