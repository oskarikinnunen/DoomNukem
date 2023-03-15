/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 17:23:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 13:08:25 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_point	vector2_to_point(t_vector2 vec)
{
	t_point	result;

	result.x = (int)vec.x;
	result.y = (int)vec.y;
	return (result);
}

t_vector3	v2tov3(t_vector2 vec)
{
	return ((t_vector3){vec.x, vec.y, 0.0f});
}

t_vector2	v3tov2(t_vector3 vec)
{
	return ((t_vector2){vec.x, vec.y});
}

t_vector2	point_to_vector2(t_point point)
{
	t_vector2	result;

	result.x = (float)point.x;
	result.y = (float)point.y;
	return (result);
}

t_quaternion	vector3_to_quaternion(t_vector3 v)
{
	return((t_quaternion){v.x, v.y, v.z, 1.0f});
}

static char	*floatstr(float f)
{
	static char	final[8];
	char	*strinteger;
	char	*strdecimal;
	int		neg;
	int		i;

	i = (int)f;
	f = f - i;
	f = f * (ft_pow(10, 1));
	neg = (f < 0.0f);
	strinteger = ft_itoa(ft_abs(i));
	strdecimal = ft_itoa(ft_abs((int)f));
	ft_bzero(final, sizeof(final));
	if (neg)
		ft_strcat(final, "-");
	ft_strcat(final, strinteger);
	ft_strcat(final, ",");
	ft_strcat(final, strdecimal);
	free(strinteger);
	free(strdecimal);
	return (final);
}

float	radtodeg(float rad)
{
	return (rad * (180.0f / M_PI));
}

float fov_deg_to_fov_rad(float fovdeg)
{
	return (1.0f / tanf(fovdeg * 0.5f / 180.0f * 3.14159f));
}


char	*vector_string(t_vector3 vec)
{
	static char str[128]; //TODO: this is too long

	*str = '\0';
	ft_strcat(str, "x:");
	ft_strcat(str, floatstr(vec.x));
	ft_strcat(str, " y:");
	ft_strcat(str, floatstr(vec.y));
	ft_strcat(str, " z:");
	ft_strcat(str, floatstr(vec.z));
	return (str);
}

bool		float_cmp_epsilon(float v1, float v2, float epsilon)
{
	float	temp;

	temp = v1 - v2;
	temp = fabsf(temp);
	return (temp - epsilon <= 0.0f);
}