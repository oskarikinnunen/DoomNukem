/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_line_circle.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:51:17 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 17:29:24 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"
#include "movement_defs.h"

t_collision	calculate_new_pos(t_vector2 delta, float radius)
{
	t_collision	result;
	float		dist;

	dist = vector2_magnitude(delta);
	result.normal = vector2_mul(delta, (1.0f / dist));
	result.depth = radius - dist;
	return (result);
}

static t_vector2	get_closest_point(t_line line, t_vector2 circle)
{
	float		wall_len;
	float		dot;
	t_vector2	closest;

	wall_len = vector2_dist(line.start, line.end);
	dot = (((circle.x - line.start.x) * (line.end.x - line.start.x)) + \
			((circle.y - line.start.y) * (line.end.y - line.start.y))) / \
			(wall_len * wall_len);
	closest.x = line.start.x + (dot * (line.end.x - line.start.x));
	closest.y = line.start.y + (dot * (line.end.y - line.start.y));
	return (closest);
}

bool	collision_line_circle_save_collision(t_line line, t_vector2 circle,
									float radius, t_collision *collision)
{
	t_vector2	closest;
	t_vector2	delta;
	float		dist;

	if (collision_point_circle(line.start, circle, radius))
	{
		*collision = calculate_new_pos(vector2_sub(circle, line.start), radius);
		return (true);
	}
	if (collision_point_circle(line.end, circle, radius))
	{
		*collision = calculate_new_pos(vector2_sub(circle, line.end), radius);
		return (true);
	}
	closest = get_closest_point(line, circle);
	if (collision_line_point(line.start, line.end, closest))
	{
		if (collision_point_circle(closest, circle, radius))
		{
			*collision = calculate_new_pos(vector2_sub(circle, closest), \
														radius);
			return (true);
		}
	}
	return (false);
}

bool	collision_line_circle(t_line line, t_vector2 circle, float radius)
{
	float		wall_len;
	float		dot;
	t_vector2	closest;
	t_vector2	delta;
	float		dist;

	if (collision_point_circle(line.start, circle, radius))
		return (true);
	if (collision_point_circle(line.end, circle, radius))
		return (true);
	wall_len = vector2_dist(line.start, line.end);
	dot = (((circle.x - line.start.x) * (line.end.x - line.start.x)) + \
			((circle.y - line.start.y) * (line.end.y - line.start.y))) / \
			(wall_len * wall_len);
	closest.x = line.start.x + (dot * (line.end.x - line.start.x));
	closest.y = line.start.y + (dot * (line.end.y - line.start.y));
	if (collision_line_point(line.start, line.end, closest))
	{
		if (collision_point_circle(closest, circle, radius))
			return (true);
	}
	return (false);
}
