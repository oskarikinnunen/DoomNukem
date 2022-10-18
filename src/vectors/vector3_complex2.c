/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_complex2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 19:06:07 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/18 21:05:38 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector3 vector3_normalise(t_vector3 v)
{
	float l = vector3_magnitude(v);
	return((t_vector3){ v.x / l, v.y / l, v.z / l});
}

t_vector3 vector3_crossproduct(t_vector3 v1, t_vector3 v2)
{
	t_vector3 v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

t_vector3	vector3_mul_vector3(t_vector3 v1, t_vector3 v2)
{
	return ((t_vector3){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z});
}

t_vector3	vector3_clamp_magnitude(t_vector3 vec, float max_magnitude)
{
	float	len;
	float	mul;

	len = vector3_magnitude(vec);
	mul = 1.0f;
	if (ft_absf(len) > max_magnitude)
		mul = max_magnitude / len;
	return ((t_vector3){vec.x * mul, vec.y * mul, vec.z * mul});
}
