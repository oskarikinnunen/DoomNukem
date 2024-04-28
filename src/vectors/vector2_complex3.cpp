/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_complex3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:43:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:49:27 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "room.h"

float	vector2_fdist_to_plane(t_vector2 p,
	t_vector2 plane_n, t_vector2 plane_p)
{
	t_vector2	n;

	n = vector2_normalise(p);
	return (plane_n.x * p.x + plane_n.y * p.y - vector2_dot(plane_n, plane_p));
}

t_vector2	vector2_abs(t_vector2 v)
{
	return ((t_vector2){fabsf(v.x), fabsf(v.y)});
}

float	vector2_dist_along_line(t_vector2 vec, t_line line)
{
	t_vector2	line_s;
	t_vector2	vec_s;

	line_s = vector2_sub(line.start, line.end);
	vec_s = vector2_sub(vec, line.start);
	return (ft_clampf(-vector2_dot(vec_s, line_s)
			/ vector2_dot(line_s, line_s), 0.0f, 1.0f));
}

bool	vector2_cmp_epsilon(t_vector2 v1, t_vector2 v2, float epsilon)
{
	t_vector2	temp;

	temp = vector2_sub(v1, v2);
	temp = vector2_abs(temp);
	return (temp.x - epsilon <= 0.0f && temp.y - epsilon <= 0.0f);
}
