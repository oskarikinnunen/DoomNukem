/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_shorthands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:34:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/13 17:23:14 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector3	vector3_zero(void)
{
	return ((t_vector3){0.0f, 0.0f, 0.0f});
}

t_vector3	vector3_up(void)
{
	return ((t_vector3){0.0f, 0.0f, 1.0f});
}

t_vector3	vector3_down(void)
{
	return ((t_vector3){0.0f, 0.0f, -1.0f});
}

t_vector3	vector3_one(void)
{
	return ((t_vector3){1.0f, 1.0f, 1.0f});
}

t_vector3	vector3_negative(t_vector3 vec)
{
	t_vector3	result;

	result.x = -vec.x;
	result.y = -vec.y;
	result.z = -vec.z;
	return (result);
}
