/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_simple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 13:35:14 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 13:49:32 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"
// TODO: name this file better

bool	collision_point_circle(t_vector2 point, t_vector2 circle, float radius)
{
	t_vector2	delta;

	delta = vector2_sub(circle, point);
	if (vector2_dot(delta, delta) <= radius * radius)
		return (true);
	return (false);
}

bool	collision_line_point(t_vector2 start, t_vector2 end, t_vector2 point)
{
	float	distance1;
	float	distance2;
	float	line_len;
	float	buffer;

	distance1 = vector2_dist(point, start);
	distance2 = vector2_dist(point, end);
	line_len = vector2_dist(start, end);
	buffer = 0.1f;
	if ((distance1 + distance2) >= (line_len - buffer) && \
			(distance1 + distance2) <= (line_len + buffer))
		return (true);
	return (false);
}

bool	collision_point_rectangle(t_point p, t_rectangle rect)
{
	return (p.x > rect.position.x && p.x < rect.position.x + rect.size.x
		&& p.y > rect.position.y && p.y < rect.position.y + rect.size.y);
}
