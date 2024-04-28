/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navmesh.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 12:04:40 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 20:55:38 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"

static void	malloc_space_for_navigation_buffer(t_world *world)
{
	if (world->nav.openlist)
		free(world->nav.openlist);
	world->nav.openlist = \
	prot_memalloc(world->nav.malloc_size * sizeof(t_nav_node));
	ft_bzero(world->nav.openlist, world->nav.malloc_size * sizeof(t_nav_node));
}

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

void	navmesh(t_world *world)
{
	t_vector3	start;
	t_vector3	end;
	int			i;
	int			j;

	create_navmesh(world);
	i = 0;
	while (i < world->nav.node_amount)
	{
		world->nav.navmesh[i].neighbors = 0;
		j = 0;
		while (j < 3)
		{
			start = world->nav.navmesh[i].vertex[j];
			end = world->nav.navmesh[i].vertex[(j + 1) % 3];
			check_neighbors(&world->nav, i, start, end);
			j++;
		}
		i++;
	}
	malloc_space_for_navigation_buffer(world);
}
