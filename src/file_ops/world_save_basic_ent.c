/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_save_basic_ent.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:01:34 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 22:57:38 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "doomnukem.h"

static t_list	*entitycache_to_list(t_entitycache *cache)
{
	int				i;
	int				found;
	t_list			*entitylist;

	i = 0;
	found = 0;
	entitylist = NULL;
	while (found < cache->existing_entitycount && i < cache->alloc_count)
	{
		if (cache->entities[i].status != es_free
			&& cache->entities[i].dont_save == false)
		{
			cache->entities[i].id = found;
			list_push(&entitylist, &cache->entities[i], sizeof(t_entity));
			found++;
		}
		i++;
	}
	return (entitylist);
}

static void	entitylist_basicify(t_list *ent_list)
{
	t_list		*l;
	t_entity	*e;

	l = ent_list;
	while (l != NULL)
	{
		e = (t_entity *)l->content;
		l->content_size = sizeof(t_game_string)
			+ sizeof(t_transform) + sizeof(t_component_type) + sizeof(uint32_t);
		l = l->next;
	}
}

void	world_save_basic_ent(char *level, t_world world)
{
	t_list	*entitylist;

	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"saving .basic_ent to", level, NULL});
	entitylist = entitycache_to_list(&world.entitycache);
	entitylist_basicify(entitylist);
	save_chunk(level, "BENT", entitylist);
	listdel(&entitylist);
}
