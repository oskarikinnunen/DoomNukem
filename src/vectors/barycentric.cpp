/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   barycentric.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 18:49:16 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/17 18:49:56 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector2	barycentric_coordinates(t_vector2 *p, t_vector2 v)
{
	float		b1;
	float		b2;
	float		b3;
	float		b4;
	t_vector2	w;

	b1 = p[2].y - p[0].y;
	b2 = p[2].x - p[0].x;
	b3 = p[1].y - p[0].y;
	b4 = v.y - p[0].y;
	w.x = (p[0].x * b1 + b4 * b2 - v.x * b1)
		/ (b3 * b2 - (p[1].x - p[0].x) * b1);
	w.y = (b4 - w.x * b3) / b1;
	return (w);
}

inline t_vector3	get_vector3_from_barycentric(t_vector3 *p, t_vector2 bary)
{
	t_vector3	ab;
	t_vector3	ac;

	ab = vector3_lerp(p[0], p[1], bary.x);
	ac = vector3_mul(vector3_sub(p[2], p[0]), bary.y);
	return (vector3_add(ab, ac));
}
