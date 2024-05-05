/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_more.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 01:54:00 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/17 18:39:26 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "render.h"

t_vector2	vector2_snap(t_vector2 vec, int interval)
{
	t_vector2	result;
	t_vector2	remains;

	remains.x = fmod(vec.x, interval);
	remains.y = fmod(vec.y, interval);
	result.x = vec.x - remains.x;
	result.y = vec.y - remains.y;
	result.x += (remains.x > interval / 2) * interval;
	result.y += (remains.y > interval / 2) * interval;
	return (result);
}

static bool	vector2_nan_or_inf(t_vector2 vec)
{
	if (isinf(vec.x) || isnan(vec.x))
		return (true);
	if (isinf(vec.y) || isnan(vec.y))
		return (true);
	return (false);
}

t_vector2	vector2_add_xy(t_vector2 vec, float add)
{
	t_vector2	result;

	result.x = vec.x + add;
	result.y = vec.y + add;
	return (result);
}

float	vector2_anglebetween(t_vector2 first, t_vector2 second)
{
	t_vector2	temp;

	temp = vector2_sub(second, first);
	if (vector2_nan_or_inf(first) || vector2_nan_or_inf(second))
	{
		exit(0);
	}
	return (atan2f(temp.y, temp.x));
}

t_point	screenlerp(float xlerp, float ylerp, t_sdlcontext *sdl)
{
	return ((t_point){sdl->screensize.x * xlerp, sdl->screensize.y * ylerp});
}
