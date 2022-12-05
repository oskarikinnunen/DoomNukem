/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 17:40:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/05 20:52:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"

void	update_npcs(t_world *world)
{
	int	i;

	i = 0;
	while (i < 128)
	{
		if (world->npcpool[i].active)
		{
			//if (world.npcpool->entity.animation.active)
			t_npc *cur;
			cur = &world->npcpool[i];
			t_vector3 dir = vector3_sub(cur->destination, cur->entity.transform.position);
			cur->entity.transform.position = vector3_movetowards(cur->entity.transform.position, dir, world->clock.delta * MOVESPEED * 0.25f);
			cur->entity.transform.rotation.x = vector2_anglebetween((t_vector2){cur->entity.transform.position.x, cur->entity.transform.position.y},
														(t_vector2){cur->destination.x, cur->destination.y});
			if (vector3_cmp(cur->destination, cur->entity.transform.position))
			{
				if (cur->destination.x == 200.0f)
					cur->destination = (t_vector3) {500.0f, 500.0f, 0.0f};
				else
					cur->destination = (t_vector3) {200.0f, 200.0f, 0.0f};
			}
			update_anim(&(cur->entity.animation), world->clock.delta);
		}
		i++;
	}
}

void	update_entitycache(t_sdlcontext *sdl, t_world *world, t_render *render)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		if (world->entitycache.entities[i].status != es_free)
		{
			if (world->entitycache.entities[i].status == es_active)
				render_entity(*sdl, *render, &world->entitycache.entities[i]);
			found++;
		}
		i++;
	}
}

void update_world3d(t_sdlcontext sdl, t_world *world, t_render *render)
{
	t_list		*l;
	t_entity	*ent;
	t_wall		wall;
	int			i;
	
	//
	update_npcs(world);
	i = 0;
	while (i < 128)
	{
		if (world->npcpool[i].active)
		{
			t_npc npc = world->npcpool[i];
			t_vector3 dir = vector3_sub(world->npcpool[i].entity.transform.position, world->npcpool[i].destination);
			render_ray(sdl, *render, npc.entity.transform.position, npc.destination);
			render_entity(sdl, *render, &world->npcpool[i].entity);
		}
		i++;
	}
	update_entitycache(&sdl, world, render);
	render_entity(sdl, *render, &world->skybox);
	gui_start(world->debug_gui);
	gui_labeled_int("Entity count:", world->entitycache.existing_entitycount, world->debug_gui);
	gui_end(world->debug_gui);
}

static void	entity_init(t_world *world, t_sdlcontext sdl)
{
	/*t_list		*l;
	t_entity	*ent;

	l = world->entitylist;
	while (l != NULL)
	{
		ent = (t_entity *)l->content;
		if (ent->object_index < sdl.objectcount)
			ent->obj = &sdl.objects[ent->object_index]; //TODO: getobjectbyindex
		else
			ent->obj = &sdl.objects[0];
		ent->animation.frame = 0;
		ent->animation.active = false;
		l = l->next;
	}*/
}

void	scale_skybox_uvs(t_object *obj)
{
	int	i;

	i = 0;
	while (i < obj->uv_count)
	{
		obj->uvs[i] = vector2_mul(obj->uvs[i], 10.0f);
		i++;
	}
}

/*int	fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0666);
	if (fd == -1)
		error_log(EC_WRITE);
	return (fd);
}*/

static void load_walltextures(t_world *world, t_sdlcontext sdl) //TODO: Deprecated
{
	t_list		*l;
	t_wall		*wall;
	t_material	*mat;

	/*l = world->wall_list;
	while (l != NULL)
	{
		wall = (t_wall *)l->content;
		mat = &wall->object.materials[0];
		mat->img = get_image_by_name(sdl, mat->texturename);
		l = l->next;
	}*/
}

void	spawn_npc(t_world *world, char *objectname, t_vector3 position, t_sdlcontext *sdl)
{
	int	i;

	i = 0;
	while (i < 128)
	{
		if (!world->npcpool[i].active)
		{
			world->npcpool[i].active = true;
			world->npcpool[i].entity.obj = get_object_by_name(*sdl, objectname);
			world->npcpool[i].entity.transform.position = position;
			entity_start_anim(&world->npcpool[i].entity, "walk");
			world->npcpool[i].entity.transform.scale = vector3_one();
			return ;
		}
		i++;
	}
}

t_room	load_room(char *filename)
{
	t_room	result;
	t_list	*temp;
	int		fd;

	fd = fileopen(filename, O_RDONLY);
	ft_bzero(&result, sizeof(t_room));
	ft_strcpy(result.name, filename);
	temp = load_chunk(filename, "WALL", sizeof(t_wall)); //FREE!
	result.walls = list_to_ptr(temp, &result.wallcount);
	listdel(&temp);
	temp = load_chunk(filename, "FLOR", sizeof(t_meshtri));
	result.floors = list_to_ptr(temp, &result.floorcount);
	listdel(&temp);
	return (result);
}

void	load_rooms(t_world *world, t_sdlcontext *sdl)
{
	return ;
	t_list	*l;
	t_room	*r;

	l = world->roomlist;
	while (l != NULL)
	{
		r = (t_room *)l->content;
		*r = load_room(r->name);
		init_roomwalls(world, r);
		//init_room_meshes(r, sdl);
		l = l->next;
	}
}

void	init_guns(t_world *world, t_sdlcontext *sdl)
{
	t_list	*l;
	t_gun	*gun;

	l = world->guns;
	while (l != NULL)
	{
		gun = l->content;
		gun->entity.obj = get_object_by_name(*sdl, gun->entity.object_name);
		l = l->next;
	}
}

t_debugconsole	init_debugconsole()
{
	t_debugconsole	console;

	ft_bzero(&console, sizeof(t_debugconsole));
	console.messages = ft_memalloc(sizeof(char *) * 1024);
	console.messagecount = 0;
	console.show_anim.framerate = 30;
	console.show_anim.lastframe = 30 * 5;
	console.hidden = true;
	start_anim(&console.show_anim, anim_forwards);
	debugconsole_addmessage(&console, "Initialized debugconsole!");
	return (console);
}

t_entitycache	init_entitycache(uint32_t cachesize)
{
	t_entitycache	cache;
	int				i;

	ft_bzero(&cache, sizeof(t_entitycache));
	cache.alloc_count = cachesize;
	cache.entities = ft_memalloc(cache.alloc_count * sizeof(t_entity));
	cache.existing_entitycount = 0;
	i = 0;
	while (i < cache.alloc_count)
	{
		cache.entities[i].id = i;
		i++;
	}
	return (cache);
}

void		erase_entity(t_world *world, t_entity *ent)
{
	t_entitycache	*cache;

	cache = &world->entitycache;
	//protect id here? if greater than alloccount
	cache->entities[ent->id].status = es_free;
	if (cache->existing_entitycount == 0)
		error_log(EC_MALLOC);
	cache->existing_entitycount--;
}

/*void		erase_entity(t_entitycache *cache, uint16_t entity_id)
{
	cache->entities[entity_id].status = es_free;
	if (cache->existing_entitycount == 0)
		error_log(EC_MALLOC);
	cache->existing_entitycount--;
}*/

t_entity	*raise_entity(t_world	*world)
{
	int	i;
	t_entitycache	*cache;

	cache = &world->entitycache;
	i = 0;
	while (i < cache->alloc_count)
	{
		if (cache->entities[i].status == es_free)
		{
			cache->entities[i].status = es_active;
			cache->entities[i].transform.position = vector3_zero();
			cache->entities[i].transform.scale = vector3_one();
			cache->entities[i].id = i;
			//cache->entities[i].transform.scale = vector3_zero();
			cache->existing_entitycount++;
			if (cache->existing_entitycount >= cache->alloc_count)
				error_log(EC_MALLOC);
			return (&cache->entities[i]);
		}
		i++;
	}
	printf("ENTITYCACHE TOO SMALL, ABORT!");
	error_log(EC_MALLOC);
	return (NULL); //never gets here
}

t_entity	*raise_basic_entity(t_world *world, char *objectname, t_vector3 position)
{
	t_entity	*ent;

	ent = raise_entity(world);
	ent->obj = get_object_by_name(*world->sdl, objectname);
	ent->transform.position = position;
	ent->transform.scale = vector3_one();
	return (ent);
}

void	for_all_active_entities(t_world	*world, void	(*func)(t_entity *ent, t_world *world))
{
	int				i;
	int				found;
	t_entitycache	*cache;

	i = 0;
	found = 0;
	cache = &world->entitycache;
	while (found < cache->existing_entitycount)
	{
		if (cache->entities[i].status != es_free)
		{
			if (cache->entities[i].status == es_active)
			{
				func(&cache->entities[i], world);
			}
			found++;
		}
		i++;
	}
}

#include "editor_tools.h"

t_world	load_world(char *filename, t_sdlcontext *sdl)
{
	t_world	world;

	ft_bzero(&world, sizeof(t_world));
	world.sdl = sdl;
	//world.entitylist = load_chunk(filename, "ENT_", sizeof(t_entity));
	world.wall_list = load_chunk(filename, "WALL", sizeof(t_wall));
	world.roomlist = load_chunk(filename, "RMNM", sizeof(t_room));
	world.guns = load_chunk(filename, "GUNS", sizeof(t_gun));
	world.debugconsole = init_debugconsole();
	world.entitycache = init_entitycache(1024);
	world.debug_gui = ft_memalloc(sizeof(t_autogui));
	t_entity	*ent = raise_basic_entity(&world, "cyborg.obj", vector3_zero());
	t_entity	*ent2 = raise_entity(&world);
	load_rooms(&world, sdl);
	entity_init(&world, *sdl);
	init_guns(&world, sdl);
	load_walltextures(&world, *sdl);
	ft_bzero(&world.skybox, sizeof(t_entity));
	world.skybox.obj = get_object_by_name(*sdl, "cube");
	world.skybox.obj->materials[0].img = get_image_by_name(*sdl, "grid_d.png");
	//scale_skybox_uvs(world.skybox.obj);
	world.skybox.transform.scale = vector3_mul(vector3_one(), 3000.0f);
	world.skybox.transform.position = (t_vector3){1500.0f, 1500.0f, 1499.0f};
	//spawn_npc(&world, "cyborg", (t_vector3){500.0f, 500.0f, 0.0f}, &sdl);
	//world.npcpool[0].destination = (t_vector3){200.0f, 200.0f, 0.0f};
	return (world);
}


/*
TODO: 
loadrooms function that gets filename strings for each room
and loads them into world->roomlist from the files
*/

void	save_room(t_room room)
{
	int	fd;

	fd = fileopen(room.name, O_RDWR | O_CREAT | O_TRUNC); //Empty the file or create a new one if it doesn't exist
	close(fd);
	t_list *walls_list = ptr_to_list(room.walls, room.wallcount, sizeof(t_wall));
	save_chunk(room.name, "WALL", walls_list);
	t_list *floorlist = ptr_to_list(room.floors, room.floorcount, sizeof(t_meshtri));
	save_chunk(room.name, "FLOR", floorlist);
}

void	save_world(char *filename, t_world world)
{
	int	fd;

	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC); //Empty the file or create a new one if it doesn't exist
	close(fd);
	//save_chunk(filename, "ENT_", world.entitylist);
	save_chunk(filename, "WALL", world.wall_list);
	save_chunk(filename, "GUNS", world.guns);
	t_list	*r;

	r = world.roomlist;
	save_chunk(filename, "RMNM", world.roomlist);
	while (r != NULL)
	{
		save_room(*(t_room *)r->content);
		r = r->next;
	}
	//unscale_skybox_uvs(world.skybox.obj);
}