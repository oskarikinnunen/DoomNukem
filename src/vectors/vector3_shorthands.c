/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_shorthands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:34:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/05 16:23:18 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector3	vector3_zero()
{
	return ((t_vector3) {0.0f, 0.0f, 0.0f});
}

t_vector3	vector3_up()
{
	return ((t_vector3) {0.0f, 0.0f, 1.0f});
}

t_vector3	vector3_down()
{
	return ((t_vector3) {0.0f, 0.0f, -1.0f});
}

t_vector3	vector3_one()
{
	return ((t_vector3) {1.0f, 1.0f, 1.0f});
}

t_vector3	vector3_negative(t_vector3 vec)
{
	t_vector3	result;

	result.x = -vec.x;
	result.y = -vec.y;
	result.z = -vec.z;
	return (result);
}

