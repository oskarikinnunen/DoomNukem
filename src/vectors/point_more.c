/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_more.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:03:50 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/28 14:04:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_point	point_abs(t_point point)
{
	return ((t_point){ft_abs(point.x), ft_abs(point.y)});
}