/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_load.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 14:55:20 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/13 14:54:39 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"

t_debugconsole	debugconsole_default()
{
	t_debugconsole	console;

	ft_bzero(&console, sizeof(t_debugconsole));
	console.messages = ft_memalloc(sizeof(char *) * 1024);
	console.messagecount = 0;
	console.show_anim.framerate = 30;
	console.show_anim.lastframe = 30 * 5;
	console.hidden = true;
	//start_anim(&console.show_anim, anim_forwards);
	//debugconsole_addmessage(&console, "Initialized debugconsole!");
	return (console);
}

t_entitycache	entitycache_empty(uint32_t cachesize)
{
	t_entitycache	cache;
	int				i;

	ft_bzero(&cache, sizeof(t_entitycache));
	cache.alloc_count = cachesize;
	cache.entities = ft_memalloc(cache.alloc_count * sizeof(t_entity));
	cache.sorted_entities = ft_memalloc(cache.alloc_count * sizeof(t_entity *));
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
	world->sdl = sdl;
	world->debugconsole = debugconsole_default();
	world->entitycache = entitycache_empty(1024);
	_world_init_skybox(world);
	world->lighting.ambient_light = 20;
	world->nav.clip_size = 250.0f;
	world->debug_gui = ft_memalloc(sizeof(t_autogui));
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

void	world_load_amap(t_world *world)
{
	int		fd;
	char	*amap_fname;

	amap_fname = world_filename(world->name, ".amap");
	fd = open(amap_fname, O_RDONLY);
	if (fd != -1)
	{
		doomlog_mul(LOG_NORMAL, (char *[3]){\
			"found .amap for world:", world->name, NULL});
		world->roomlist = load_chunk(amap_fname, "AREA", sizeof(t_area));
		_world_sanitize_all_room_pointers(world);
		_world_init_rooms(world);
	}
	else
	{
		doomlog_mul(LOG_WARNING, (char *[3]){\
			"no .amap for world:", world->name, NULL});
	}
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

void	world_load_basic_ent(t_world *world)
{
	int		fd;
	char	*bent_fname;
	t_list	*e_list;

	bent_fname = world_filename(world->name, ".basic_ent");
	fd = open(bent_fname, O_RDONLY);
	if (fd != -1)
	{
		doomlog_mul(LOG_NORMAL, (char *[3]){\
			"found .basic_ent for world:", world->name, NULL});
		e_list = load_chunk(bent_fname, "BENT", sizeof(t_transform) + sizeof(t_gamestring));
		load_basic_ent_cache_from_list(world, e_list);
		//world->roomlist = load_chunk(amap_fname, "AREA", sizeof(t_area));
		//_world_sanitize_all_room_pointers(world);
		//_world_init_rooms(world);
	}
	else
	{
		doomlog_mul(LOG_WARNING, (char *[3]){\
			"no .amap for world:", world->name, NULL});
	}
}

void	world_load_full_ent(t_world *world)
{
	int		fd;
	char	*bent_fname;
	t_list	*e_list;

	bent_fname = world_filename(world->name, ".full_ent");
	fd = open(bent_fname, O_RDONLY);
	if (fd != -1)
	{
		doomlog_mul(LOG_NORMAL, (char *[3]){\
			"found .full_ent for world:", world->name, NULL});
		e_list = load_chunk(bent_fname, "FENT", sizeof(t_entity));
		load_full_ent_cache_from_list(world, e_list);
	}
	else
	{
		doomlog_mul(LOG_WARNING, (char *[3]){\
			"no .amap for world:", world->name, NULL});
	}
}

t_world	load_world_args(char *filename, t_sdlcontext *sdl, t_load_arg arg)
{
	t_world	world;

	doomlog(LOG_NORMAL, "LOADING WORLD");
	ft_bzero(&world, sizeof(t_world));
	//ft_strcpy(world.name, worldname(filename));
	//worldname(filen)
	ft_strcpy(world.name, filename);
	world_init(&world, sdl);
	world_load_amap(&world);
	//int fd = load_filecontent_fd("world", "sound.wav");
	if (arg == LOAD_ARG_AMAP_AND_BASIC)
		world_load_basic_ent(&world);
	else if (arg == LOAD_ARG_FULL)
		world_load_full_ent(&world);
	return (world);
}
