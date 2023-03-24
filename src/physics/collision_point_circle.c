/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_circle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:02:02 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 15:02:51 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"

bool	collision_point_circle(t_vector2 point, t_vector2 circle, float radius)
{
	t_vector2	delta;

	delta = vector2_sub(circle, point);
	if (vector2_dot(delta, delta) <= radius * radius)
		return (true);
	return (false);
}
