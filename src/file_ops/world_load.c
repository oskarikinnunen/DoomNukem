/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_load.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 14:55:20 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 21:58:58 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"
#include "doomnukem.h"

static void	load_basic_ent_cache_from_list(t_world *world, t_list *l)
{
	t_entity	*list_entity;
	t_entity	*world_entity;
	t_object	*obj;
	char		comp_filename[64];
	char		*str;

	while (l != NULL)
	{
		list_entity = l->content;
		world_entity = spawn_entity(world,
				get_object_by_name(*world->sdl, list_entity->object_name.str));
		world_entity->object_name = list_entity->object_name;
		world_entity->transform = list_entity->transform;
		world_entity->component.type = list_entity->component.type;
		world_entity->component.data_preset
			= list_entity->component.data_preset;
		world_entity->transform.parent = NULL;
		entity_set_component_functions(world_entity, world);
		l = l->next;
	}
}

static void	world_load_basic_ent(char *level, t_world *world)
{
	int		fd;
	t_list	*e_list;

	doomlog(LOG_NORMAL, "LOADING BASIC_ENT");
	e_list = load_chunk(level, "BENT", sizeof(t_gamestring)
			+ sizeof(t_transform) + sizeof(t_componenttype) + sizeof(uint32_t));
	load_basic_ent_cache_from_list(world, e_list);
	listdel(&e_list);
}

static void	world_load_amap(char *level, t_world *world)
{
	int	fd;

	doomlog(LOG_NORMAL, "LOADING AMAP");
	world->arealist = load_chunk(level, "AREA", sizeof(t_area));
	world_sanitize_all_room_pointers(world);
	world_init_rooms(world);
}

t_world	load_world(char *level_name, t_sdlcontext *sdl)
{
	t_world	world;
	char	level_path[256];
	int		fd;

	doomlog(LOG_NORMAL, "LOADING WORLD");
	ft_strcpy(level_path, "worlds/");
	ft_strncat(level_path, level_name, 200);
	world_init(&world, sdl);
	ft_strcpy(world.name, level_name);
	world_load_amap(level_path, &world);
	world_load_basic_ent(level_path, &world);
	return (world);
}
