/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navmesh.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 12:04:40 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 12:10:00 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"

void	navmesh(t_world *world)
{
	t_vector3	start;
	t_vector3	end;
	int			i;
	int			j;

	world->nav.clip_size = ft_clampf(world->nav.clip_size, 50.0f, 10000.0f);
	world->nav.height = COL_STEP; // TODO: remove nav.height
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
	if (world->nav.openlist)
		free(world->nav.openlist);
	world->nav.openlist = prot_memalloc(world->nav.malloc_size * sizeof(t_navnode));
	if (!world->nav.openlist)
		doomlog(LOG_EC_MALLOC, NULL);
	ft_bzero(world->nav.openlist, world->nav.malloc_size * sizeof(t_navnode));
}
