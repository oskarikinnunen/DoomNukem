/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_navmesh2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 22:12:06 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 22:13:12 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int	world_triangle_get_max_dist_index(t_world_triangle t)
{
	float	dist;
	int		index;
	int		i;

	index = 0;
	dist = vector3_dist(t.p[0].v, t.p[1].v);
	i = 1;
	while (i < 3)
	{
		if (dist < vector3_dist(t.p[i].v, t.p[(i + 1) % 3].v))
		{
			dist = vector3_dist(t.p[i].v, t.p[(i + 1) % 3].v);
			index = i;
		}
		i++;
	}
	return (index);
}
