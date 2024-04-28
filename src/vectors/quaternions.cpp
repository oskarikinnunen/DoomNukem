/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:34:40 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 18:28:17 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_quaternion	quaternion_angle_axis(float angle, t_vector3 axis)
{
	float			sine;
	t_quaternion	result;

	sine = sinf(angle / 2.0f);
	result.v.x = axis.x * sine;
	result.v.y = axis.y * sine;
	result.v.z = axis.z * sine;
	result.w = cosf(angle / 2.0f);
	return (result);
}

t_quaternion	quaternion_mul(t_quaternion a, t_quaternion b)
{
	return ((t_quaternion)
		{
			a.w * b.w - a.v.x * b.v.x - a.v.y * b.v.y - a.v.z * b.v.z,
			a.w * b.v.x + a.v.x * b.w + a.v.y * b.v.z - a.v.z * b.v.y,
			a.w * b.v.y - a.v.x * b.v.z + a.v.y * b.w + a.v.z * b.v.x,
			a.w * b.v.z + a.v.x * b.v.y - a.v.y * b.v.x + a.v.z * b.w
		}
	);
}

t_quaternion	lerp_quaternion(t_quaternion from, t_quaternion to, float delta)
{
	from.v.x = ft_flerp(from.v.x, to.v.x, delta);
	from.v.y = ft_flerp(from.v.y, to.v.y, delta);
	from.v.z = ft_flerp(from.v.z, to.v.z, delta);
	from.w = ft_flerp(from.w, to.w, delta);
	return (from);
}
