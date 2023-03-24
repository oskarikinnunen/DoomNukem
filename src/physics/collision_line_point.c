/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_point.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:01:10 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 15:04:18 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"

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
