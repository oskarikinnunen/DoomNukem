/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_elementary.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:28:15 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 12:05:39 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector2	vector2_add(t_vector2 first, t_vector2 second)
{
	t_vector2	result;

	result.x = first.x + second.x;
	result.y = first.y + second.y;
	return (result);
}

t_vector2	vector2_sub(t_vector2 first, t_vector2 second)
{
	t_vector2	result;

	result.x = first.x - second.x;
	result.y = first.y - second.y;
	return (result);
}

t_vector2	vector2_add_xyz(t_vector2 vec, float add)
{
	t_vector2	result;

	result.x = vec.x + add;
	result.y = vec.y + add;
	return (result);
}

//returns vector 'vec' multiplied by 'mul'
t_vector2	vector2_mul(t_vector2 vec, float mul)
{
	t_vector2	result;

	result.x = vec.x * mul;
	result.y = vec.y * mul;
	return (result);
}

//returns vector 'vec' divided by 'div'
t_vector2	vector2_div(t_vector2 vec, float div)
{
	t_vector2	result;

	result.x = vec.x / div;
	result.y = vec.y / div;
	return (result);
}