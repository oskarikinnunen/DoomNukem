/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 17:23:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/14 14:23:59 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_point	vector2_to_point(t_vector2 vec)
{
	return (*(t_point *)&vec);
}

t_vector2	point_to_vector2(t_point point)
{
	return (*(t_vector2 *)&point);
}