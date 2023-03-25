/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_line_line_intersect.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 13:40:42 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 14:05:03 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"

// TODO: name variables better
bool	collision_line_line_intersect(t_line line1, t_line line2)
{
	t_line_line_intersect	lli;
	float					res;

	lli.x_diff1 = line1.start.x - line1.end.x;
	lli.x_diff2 = line2.start.x - line2.end.x;
	lli.y_diff1 = line1.start.y - line1.end.y;
	lli.y_diff2 = line2.start.y - line2.end.y;
	res = lli.x_diff1 * lli.y_diff2 - lli.y_diff1 * lli.x_diff2;
	if (fabsf(res) > 0.001f)
	{
		lli.fa = line1.start.x * line1.end.y - line1.start.y * line1.end.x;
		lli.fb = line2.start.x * line2.end.y - line2.start.y * line2.end.x;
		lli.x = (lli.fa * lli.x_diff2 - lli.fb * lli.x_diff1) / res;
		lli.y = (lli.fa * lli.y_diff2 - lli.fb * lli.y_diff1) / res;
		if ((ft_minf(line1.start.x, line1.end.x) < lli.x && \
				ft_maxf(line1.start.x, line1.end.x) > lli.x && \
				ft_minf(line2.start.x, line2.end.x) < lli.x && \
				ft_maxf(line2.start.x, line2.end.x) > lli.x) || \
				(ft_minf(line1.start.y, line1.end.y) < lli.y && \
				ft_maxf(line1.start.y, line1.end.y) > lli.y && \
				ft_minf(line2.start.y, line2.end.y) < lli.y && \
				ft_maxf(line2.start.y, line2.end.y) > lli.y))
			return (true);
	}
	return (false);
}
