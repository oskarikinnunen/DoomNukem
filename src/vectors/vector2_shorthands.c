/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_shorthands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:34:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/13 17:31:14 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector2	vector2_zero(void)
{
	return ((t_vector2){0, 0});
}

t_vector2	vector2_one(void)
{
	return ((t_vector2){1, 1});
}

t_vector2	vector2_negative(t_vector2 vec)
{
	t_vector2	result;

	result.x = -vec.x;
	result.y = -vec.y;
	return (result);
}
