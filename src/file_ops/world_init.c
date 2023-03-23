/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:55:23 by raho              #+#    #+#             */
/*   Updated: 2023/03/23 19:53:13 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"

void	world_remove_all_room_entities(t_world *world)
{
	t_list	*l;
	t_area	*room;

	l = world->arealist;
	while (l != NULL)
	{
		room = (t_area *)l->content;
		room_remove_entities(room, world);
		l = l->next;
	}
}

void	world_sanitize_all_room_pointers(t_world *world)
{
	t_list	*l;
	t_area	*room;
	int		i;

	l = world->arealist;
	while (l != NULL)
	{
		room = (t_area *)l->content;
		i = 0;
		while (i < room->wallcount)
		{
			room->walls[i].edgeline.start = NULL;
			room->walls[i].edgeline.end = NULL;
			i++;
		}
		l = l->next;
	}
}

t_entitycache	entitycache_empty(uint32_t cachesize)
{
	t_entitycache	cache;
	int				i;

	ft_bzero(&cache, sizeof(t_entitycache));
	cache.alloc_count = cachesize;
	cache.entities = prot_memalloc(cache.alloc_count * sizeof(t_entity));
	cache.sorted_entities = prot_memalloc(cache.alloc_count * sizeof(t_entity *));
	cache.existing_entitycount = 0;
	i = 0;
	while (i < cache.alloc_count)
	{
		cache.entities[i].id = i;
		cache.sorted_entities[i] = &cache.entities[i];
		i++;
	}
	return (cache);
}

void	world_init_rooms(t_world *world)
{
	t_list	*l;
	t_area	*r;
	t_area	temp;
	int		i;

	l = world->arealist;
	world_sanitize_all_room_pointers(world);
	while (l != NULL)
	{
		r = (t_area *)l->content;
		room_init_shallow(r, world);
		l = l->next;
	}
	l = world->arealist;
	while (l != NULL)
	{
		r = (t_area *)l->content;
		room_init(r, world);
		l = l->next;
	}
}

void	world_init(t_world *world, t_sdlcontext *sdl)
{
	ft_bzero(world, sizeof(t_world));
	world->sdl = sdl;
	world->entitycache = entitycache_empty(2048);
	world->nav.clip_size = 250.0f;
	world->debug_gui = prot_memalloc(sizeof(t_autogui));
	ft_strcpy(world->name, DEFAULTLEVEL);
}
