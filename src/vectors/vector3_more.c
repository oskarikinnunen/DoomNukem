/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_more.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 11:40:43 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 20:10:57 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector3	vector3_snap(t_vector3 vec, int interval)
{
	t_vector3	result;
	t_vector3	remains;

	remains.x = fmod(vec.x, interval);
	remains.y = fmod(vec.y, interval);
	remains.z = fmod(vec.z, interval);
	result.x = vec.x - remains.x;
	result.y = vec.y - remains.y;
	result.z = vec.z - remains.z;
	result.x += (remains.x > interval / 2) * interval;
	result.y += (remains.y > interval / 2) * interval;
	result.z += (remains.z > interval / 2) * interval;
	return (result);
}

t_vector3	vector3_move_towards(t_vector3 vec, t_vector3 to, float delta)
{
	t_vector3	result;

	result.x = ft_fmovetowards(vec.x, to.x, delta);
	result.y = ft_fmovetowards(vec.y, to.y, delta);
	result.z = ft_fmovetowards(vec.z, to.z, delta);
	return (result);
}

t_vector3	vector3_abs(t_vector3 v)
{
	return ((t_vector3){fabsf(v.x), fabsf(v.y), fabsf(v.z)});
}

bool	vector3_cmp_epsilon(t_vector3 v1, t_vector3 v2, float epsilon)
{
	t_vector3	temp;

	temp = vector3_sub(v1, v2);
	temp = vector3_abs(temp);
	return (temp.x - epsilon <= 0.0f
		&& temp.y - epsilon <= 0.0f && temp.z - epsilon <= 0.0f);
}
