/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_elementary.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:28:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/13 17:24:14 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector3	vector3_add(t_vector3 first, t_vector3 second)
{
	t_vector3	result;

	result.x = first.x + second.x;
	result.y = first.y + second.y;
	result.z = first.z + second.z;
	return (result);
}

t_vector3	vector3_sub(t_vector3 first, t_vector3 second)
{
	t_vector3	result;

	result.x = first.x - second.x;
	result.y = first.y - second.y;
	result.z = first.z - second.z;
	return (result);
}

t_vector3	vector3_add_xyz(t_vector3 vec, float add)
{
	t_vector3	result;

	result.x = vec.x + add;
	result.y = vec.y + add;
	result.z = vec.z + add;
	return (result);
}

//returns vector 'vec' multiplied by 'mul'
t_vector3	vector3_mul(t_vector3 vec, float mul)
{
	t_vector3	result;

	result.x = vec.x * mul;
	result.y = vec.y * mul;
	result.z = vec.z * mul;
	return (result);
}

//returns vector 'vec' divided by 'div'
t_vector3	vector3_div(t_vector3 vec, float div)
{
	t_vector3	result;

	result.x = vec.x / div;
	result.y = vec.y / div;
	result.z = vec.z / div;
	return (result);
}
