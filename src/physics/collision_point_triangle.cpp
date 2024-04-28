/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_point_triangle.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 13:37:59 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 16:26:48 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"

float	sign(t_point p1, t_vector3 p2, t_vector3 p3)
{
	return (((float)p1.x - p3.x) * (p2.y - p3.y) - \
			(p2.x - p3.x) * ((float)p1.y - p3.y));
}

float	sign2(t_point p1, t_point p2, t_point p3)
{
	return ((p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y));
}

bool	collision_point_triangle(t_point point, t_world_triangle tri)
{
	float	d1;
	float	d2;
	float	d3;
	bool	has_neg;
	bool	has_pos;

	d1 = sign(point, tri.p[0].v, tri.p[1].v);
	d2 = sign(point, tri.p[1].v, tri.p[2].v);
	d3 = sign(point, tri.p[2].v, tri.p[0].v);
	has_neg = ((d1 < 0) || (d2 < 0) || (d3 < 0));
	has_pos = ((d1 > 0) || (d2 > 0) || (d3 > 0));
	return (!(has_neg && has_pos));
}
