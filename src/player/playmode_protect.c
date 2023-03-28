/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_protect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 10:01:27 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 11:25:36 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "editor_tools.h"
#include "movement_defs.h"

void	playmode_protect_npcs(t_world *world)
{
	t_entitycache	*cache;
	int				i;
	t_object		*human;

	cache = &world->entitycache;
	i = 0;
	human = get_object_by_name(*world->sdl, "Human.obj");
	while (i < cache->alloc_count)
	{
		if (cache->entities[i].component.type == COMP_NPC
			&& cache->entities[i].obj != human)
		{
			doomlog(LOG_WARNING, "Removed invalid npc component and entity\n");
			destroy_entity(world, &cache->entities[i]);
		}
		i++;
	}
}
