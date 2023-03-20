/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_load.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 14:55:20 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 15:30:08 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"

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

static void	_world_init_skybox(t_world *world)
{
	ft_bzero(&world->skybox, sizeof(t_entity));
	world->skybox.obj = get_object_by_name(*world->sdl, "cube");
	world->skybox.obj->materials[0].img = get_image_by_name(*world->sdl, "grid_d.cng");
	world->skybox.transform.scale = vector3_mul(vector3_one(), 300.0f);
	world->skybox.transform.rotation = vector3_zero();
	world->skybox.transform.position = (t_vector3){1500.0f, 1500.0f, 1497.5f};
}

void	world_init(t_world *world, t_sdlcontext *sdl)
{
	ft_bzero(world, sizeof(t_world));
	world->sdl = sdl;
	world->entitycache = entitycache_empty(1024);
	world->lighting.ambient_light = 20;
	world->nav.clip_size = 250.0f;
	world->debug_gui = prot_memalloc(sizeof(t_autogui));
	ft_strcpy(world->name, "default");
}

void	_world_init_rooms(t_world *world)
{
	t_list	*l;
	t_area	*r;
	t_area	temp;
	int		i;

	l = world->roomlist;
	_world_sanitize_all_room_pointers(world);
	while (l != NULL)
	{
		r = (t_area *)l->content;
		room_init_shallow(r, world); //TODO: might not be needed at if applywallmesh is protected
		l = l->next;
	}
	l = world->roomlist;
	while (l != NULL)
	{
		r = (t_area *)l->content;
		room_init(r, world);
		l = l->next;
	}
}

void	world_load_amap(char *level, t_world *world)
{
	int	fd;

	doomlog(LOG_NORMAL, "LOADING AMAP");
	world->roomlist = load_chunk(level, "AREA", sizeof(t_area));
	_world_sanitize_all_room_pointers(world);
	_world_init_rooms(world);
}

void	load_basic_ent_cache_from_list(t_world *world, t_list *l)
{
	t_entity	*list_entity;
	t_entity	*world_entity;
	char		comp_filename[64];
	char		*str;

	while (l != NULL)
	{
		list_entity = l->content;
		/*if (list_entity->component.type != COMP_NONE)
		{
			load_component(list_entity, filename);
		}*/
		//list_entity
		//load_filecontent(filename, "")
		world_entity = spawn_entity(world);
		world_entity->object_name = list_entity->object_name;
		world_entity->transform = list_entity->transform;
		world_entity->transform.parent = NULL; //TODO: Might be fine if we don't allow parenting in the editor
		world_entity->obj = get_object_by_name(*world->sdl, world_entity->object_name.str);
		l = l->next;
	}
}

void	load_full_ent_cache_from_list(t_world *world, t_list *l)
{
	t_entity	*list_entity;
	t_entity	*world_entity;
	char		comp_filename[64];
	char		*str;

	while (l != NULL)
	{
		list_entity = l->content;
		/*if (list_entity->component.type != COMP_NONE)
		{
			load_component(list_entity, filename);
		}*/
		//list_entity
		//load_filecontent(filename, "")
		world_entity = spawn_entity(world);
		world_entity->object_name = list_entity->object_name;
		world_entity->transform = list_entity->transform;
		world_entity->transform.parent = NULL; //TODO: Might be fine if we don't allow parenting in the editor
		world_entity->component.type = list_entity->component.type;
		//world_entity->comp
		entity_set_component_functions(world_entity, world);
		world_entity->obj = get_object_by_name(*world->sdl, world_entity->object_name.str);
		l = l->next;
	}
}

void	world_load_basic_ent(char *level, t_world *world)
{
	int		fd;
	t_list	*e_list;

	doomlog(LOG_NORMAL, "LOADING BASIC_ENT");
	e_list = load_chunk(level, "BENT", sizeof(t_transform) + sizeof(t_gamestring));
	load_basic_ent_cache_from_list(world, e_list);
	//world->roomlist = load_chunk(amap_fname, "AREA", sizeof(t_area));
	//_world_sanitize_all_room_pointers(world);
	//_world_init_rooms(world);
}

void	world_load_full_ent(char *level, t_world *world)
{
	t_list	*e_list;

	doomlog(LOG_NORMAL, "LOADING FULL_ENT");
	e_list = load_chunk(level, "FENT", sizeof(t_entity));
	load_full_ent_cache_from_list(world, e_list);
}

t_world	load_world(char *level_name, t_sdlcontext *sdl)
{
	t_world	world;
	char	level_path[256];

	ft_strcpy(level_path, "worlds/");
	ft_strncat(level_path, level_name, 200);
	doomlog(LOG_NORMAL, "LOADING WORLD");
	world_init(&world, sdl);
	ft_strcpy(world.name, level_name);
	world_load_amap(level_path, &world);
	world_load_basic_ent(level_path, &world);
	world_load_full_ent(level_path, &world);
	return (world);
}
