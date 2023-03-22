/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_clamp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 12:30:19 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 11:45:44 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector2	vector2_clamp_magnitude(t_vector2 vec, float max_magnitude)
{
	float	len;
	float	mul;

	len = vector2_magnitude(vec);
	mul = 1.0f;
	if (ft_absf(len) > max_magnitude)
		mul = max_magnitude / len;
	return ((t_vector2){vec.x * mul, vec.y * mul});
}
