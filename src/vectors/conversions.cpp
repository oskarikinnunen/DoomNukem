/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 17:23:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 20:06:52 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_quaternion	vector3_to_quaternion(t_vector3 v)
{
	return ((t_quaternion){v.x, v.y, v.z, 1.0f});
}

float	rad_to_deg(float rad)
{
	return (rad * (180.0f / M_PI));
}

float	fov_deg_to_fov_rad(float fovdeg)
{
	return (1.0f / tanf(fovdeg * 0.5f / 180.0f * 3.14159f));
}

bool	float_cmp_epsilon(float v1, float v2, float epsilon)
{
	float	temp;

	temp = v1 - v2;
	temp = fabsf(temp);
	return (temp - epsilon <= 0.0f);
}
