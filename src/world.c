/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 17:40:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/27 13:17:15 by raho             ###   ########.fr       */
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
	//is_entity_occlusion_culled()
	while (i < 128)
	{
		if (world->npcpool[i].active)
		{
			//if (world.npcpool->entity.animation.active)
			t_npc *cur;
			cur = &world->npcpool[i];
			t_vector3 dir = vector3_sub(cur->destination, cur->entity->transform.position);
			cur->entity->transform.position = vector3_movetowards(cur->entity->transform.position, dir, world->clock.delta * MOVESPEED * 0.25f);
			cur->entity->transform.rotation.x = vector2_anglebetween((t_vector2){cur->entity->transform.position.x, cur->entity->transform.position.y},
														(t_vector2){cur->destination.x, cur->destination.y});
			if (vector3_cmp(cur->destination, cur->entity->transform.position))
			{
				if (cur->destination.x == 200.0f)
					cur->destination = (t_vector3) {500.0f, 500.0f, 0.0f};
				else
					cur->destination = (t_vector3) {200.0f, 200.0f, 0.0f};
			}
			update_anim(&(cur->entity->animation), world->clock.delta);
			update_entity_bounds(cur->entity);
		}
		i++;
	}
}

void	update_entitycache(t_sdlcontext *sdl, t_world *world, t_render *render)
{
	int			i;
	int			found;
	t_entity	*ent;

	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = &world->entitycache.entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
				render_entity(sdl, render, ent);
			found++;
		}
		i++;
	}
}

void update_world3d(t_world *world, t_render *render)
{
	t_list			*l;
	t_entity		*ent;
	t_wall			wall;
	t_sdlcontext	*sdl;
	int				i;
	
	sdl = world->sdl;
	ft_bzero(&render->rs, sizeof(t_render_statistics));
	/*update_npcs(world);
	i = 0;
	while (i < 128)
	{
		if (world->npcpool[i].active)
		{
			t_npc npc = world->npcpool[i];
			t_vector3 dir = vector3_sub(world->npcpool[i].entity->transform.position, world->npcpool[i].destination);
			render_ray(*sdl, *render, npc.entity->transform.position, npc.destination);
			render_entity(*sdl, render, &world->npcpool[i].entity);
		}
		i++;
	}*/
	update_entitycache(sdl, world, render);
	if (!sdl->global_wireframe && !world->skybox.hidden)
		render_entity(sdl, render, &world->skybox);
	rescale_surface(sdl);
	gui_start(world->debug_gui);
	gui_labeled_int("Tri count:", render->rs.triangle_count, world->debug_gui);
	gui_labeled_int("Render count:", render->rs.render_count, world->debug_gui);
	gui_labeled_int("Entity count:", world->entitycache.existing_entitycount, world->debug_gui);
	gui_labeled_float_slider("Resolution scale:", &world->sdl->resolution_scaling, 0.01f, world->debug_gui);
	gui_labeled_float_slider("Audio max:", &world->sdl->audio.max_volume, 0.01f, world->debug_gui);
	update_volume(&world->sdl->audio);
	if (gui_button("Test audio", world->debug_gui))
	{
		static bool	statement = false;

		if (statement)
		{
			//pause_music(&sdl->audio, statement);
			//pause_audio(&sdl->audio, statement);
			//play_worldsound(&sdl->audio, "bubbles.wav", &world->entitycache.entities[0].transform.position);
			play_localsound(&sdl->audio, "pistol1.wav");
			statement = false;
		}
		else
		{
			//pause_music(&sdl->audio, statement);
			//pause_audio(&sdl->audio, statement);
			play_localsound(&sdl->audio, "bubbles.wav");
			statement = true;
		}
		
	}
	world->sdl->audio.max_volume = ft_clampf(world->sdl->audio.max_volume, 0.25f, 1.0f);
	world->sdl->resolution_scaling = ft_clampf(world->sdl->resolution_scaling, 0.25f, 1.0f);
	t_point	res;
	res = point_fmul(sdl->screensize, sdl->resolution_scaling);
	gui_labeled_point("3D Resolution:", res, world->debug_gui);
	gui_labeled_bool_edit("Wireframe:", &world->sdl->global_wireframe, world->debug_gui);
	if (gui_shortcut_button("Toggle Lighting", 'L', world->debug_gui))
		sdl->lighting_toggled = !sdl->lighting_toggled;
	//gui_labeled_bool_edit("Lighting:", &world->sdl->lighting_toggled, world->debug_gui);
	gui_labeled_int_slider("PS1 tri div:", &sdl->ps1_tri_div, 2.0f, world->debug_gui);
	if (gui_shortcut_button("Toggle Skybox", 'H', world->debug_gui))
		world->skybox.hidden = !world->skybox.hidden;
	if (gui_shortcut_button("Bake lighting", 'b', world->debug_gui))
		bake_lighting(render, world);
	if (gui_shortcut_button("Bake lighting (new)", 'v', world->debug_gui))
		start_lightbake(&world->sdl->render, world);
	sdl->ps1_tri_div = ft_clamp(sdl->ps1_tri_div, 1, 4);
	gui_end(world->debug_gui);
}

void	init_entity(t_entity *entity, t_world *world)
{
	entity->obj = get_object_by_name(*world->sdl, entity->object_name);
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
	/*int	i;

	i = 0;
	while (i < 128)
	{
		if (!world->npcpool[i].active)
		{
			world->npcpool[i].active = true;
			world->npcpool[i].entity->obj = get_object_by_name(*sdl, objectname);
			world->npcpool[i].entity->transform.position = position;
			entity_start_anim(world->npcpool[i].entity, "walk");
			world->npcpool[i].entity->transform.scale = vector3_one();
			update_entity_bounds(world->npcpool[i].entity);
			default_entity_occlusion_settings(world->npcpool[i].entity, world);
			return ;
		}
		i++;
	}*/
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
	temp = load_chunk(filename, "AREA", sizeof(t_floor_area));
	result.floor_areas = list_to_ptr(temp, &result.floor_areacount);
	listdel(&temp);

	temp = load_chunk(filename, "EDGE", sizeof(t_vector2));
	result.edges = list_to_ptr(temp, &result.edgecount);
	listdel(&temp);
	return (result);
}

static void	startup_init_room(t_world *world, t_room *r)
{
	int			i;
	t_wall		*w;
	t_meshtri	*f;

	i = 0;
	while (i < r->wallcount)
	{
		w = &r->walls[i];
		w->edgeline.start = &r->edges[w->edgeline.start_index];
		w->edgeline.end = &r->edges[w->edgeline.end_index];
		w->entity = &world->entitycache.entities[w->saved_entityid]; //TODO: make function "get_entity_from_cache_by_id" (with a shorter name, lol)
		//w->entity->obj = object_plane(world->sdl);
		entity_assign_object(world, w->entity, object_plane(world->sdl));
		w->entity->obj->materials[0].img = get_image_by_name(*world->sdl, w->texname);
		applywallmesh(w, r, world);
		i++;
	}
	i = 0;
	while (i < r->floorcount)
	{
		f = &r->floors[i];
		f->entity = &world->entitycache.entities[f->saved_entityid]; //TODO: make function "get_entity_from_cache_by_id" (with a shorter name, lol)
		f->entity->obj = object_tri(world->sdl);
		f->entity->obj->materials[0].img = get_image_by_name(*world->sdl, f->texname);
		applytrimesh(*f, f->entity->obj);
		i++;
	}
}

void	load_rooms(t_world *world, t_sdlcontext *sdl)
{
	t_list	*l;
	t_room	*r;

	l = world->roomlist;
	while (l != NULL)
	{
		r = (t_room *)l->content;
		*r = load_room(r->name);
		startup_init_room(world, r);
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

void		destroy_entity(t_world *world, t_entity *ent)
{
	t_entitycache	*cache;

	cache = &world->entitycache;
	//protect id here? if greater than alloccount
	cache->entities[ent->id].status = es_free;
	cache->entities[ent->id].obj = NULL;
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

t_entity	*spawn_entity(t_world	*world)
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

void		entity_assign_object(t_world *world, t_entity *entity, t_object *obj)
{
	entity->obj = obj;
	create_lightmap_for_entity(entity, world);
	create_map_for_entity(entity, world);
}

t_entity	*spawn_basic_entity(t_world *world, char *objectname, t_vector3 position) //UNUSED
{
	t_entity	*ent;

	ent = spawn_entity(world);
	ent->obj = get_object_by_name(*world->sdl, objectname);
	ent->transform.position = position;
	ent->transform.scale = vector3_one();
	return (ent);
}

void	for_all_entities(t_world	*world, void	(*func)(t_entity *ent, t_world *world))
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
			func(&cache->entities[i], world);
			found++;
		}
		i++;
	}
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

void	load_cache_from_list(t_world *world, t_list *l)
{
	t_entity	*list_entity;
	t_entity	*world_entity;

	while (l != NULL)
	{
		list_entity = l->content;
		world_entity = spawn_entity(world);
		ft_memcpy(world_entity, list_entity, sizeof(t_entity));
		l = l->next;
	}
	
}

t_world	load_world(char *filename, t_sdlcontext *sdl)
{
	t_world	world;

	ft_bzero(&world, sizeof(t_world));
	world.sdl = sdl;
	t_vector2 v;
	world.guns = load_chunk(filename, "GUNS", sizeof(t_gun));
	world.debugconsole = init_debugconsole();
	world.entitycache = init_entitycache(1024);
	
	t_list	*entitylist = load_chunk(filename, "ENT_", sizeof(t_entity));
	load_cache_from_list(&world, entitylist);
	for_all_entities(&world, init_entity);

	world.debug_gui = ft_memalloc(sizeof(t_autogui));

	world.roomlist = load_chunk(filename, "RMNM", sizeof(t_room));
	load_rooms(&world, sdl);
	
	init_guns(&world, sdl);
	load_walltextures(&world, *sdl);
	ft_bzero(&world.skybox, sizeof(t_entity));
	world.skybox.obj = get_object_by_name(*sdl, "cube");
	world.skybox.obj->materials[0].img = get_image_by_name(*sdl, "grid_d.cng");
	//scale_skybox_uvs(world.skybox.obj);
	world.skybox.transform.scale = vector3_mul(vector3_one(), 300.0f);
	world.skybox.transform.position = (t_vector3){1500.0f, 1500.0f, 1497.5f};
	//spawn_npc(&world, "cyborg", (t_vector3){500.0f, 500.0f, 0.0f}, &sdl);
	//world.npcpool[0].destination = (t_vector3){200.0f, 200.0f, 0.0f};
	ft_bzero(&world.lighting, sizeof(t_lighting));
	world.lighting.ambient_light = 20;
	for_all_entities(&world, create_lightmap_for_entity);
	for_all_entities(&world, create_map_for_entity);
	return (world);
}


/*
TODO: 
loadrooms function that gets filename strings for each room
and loads them into world->roomlist from the files
*/

/*save_entity_id(t_entity *entity, uint16_t *id)
{
	*id = entity.id
}*/

void	prepare_saving(t_room *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		room->walls[i].saved_entityid = room->walls[i].entity->id;
		ft_strcpy(room->walls[i].texname, room->walls[i].entity->obj->materials[0].img->name);
		i++;
	}
	i = 0;
	while (i < room->floorcount)
	{
		room->floors[i].saved_entityid = room->floors[i].entity->id;
		ft_strcpy(room->floors[i].texname, room->floors[i].entity->obj->materials[0].img->name);
		i++;
	}
}

void	save_room(t_room room)
{
	int		fd;

	printf("saving room '%s' \n", room.name);
	fd = fileopen(room.name, O_RDWR | O_CREAT | O_TRUNC); //Empty the file or create a new one if it doesn't exist
	prepare_saving(&room);
	t_list *walls_list = ptr_to_list(room.walls, room.wallcount, sizeof(t_wall));
	save_chunk(room.name, "WALL", walls_list);
	t_list *floorlist = ptr_to_list(room.floors, room.floorcount, sizeof(t_meshtri));
	save_chunk(room.name, "FLOR", floorlist);
	t_list *arealist = ptr_to_list(room.floor_areas, room.floor_areacount, sizeof(t_floor_area));
	save_chunk(room.name, "AREA", arealist);
	t_list *edgelist = ptr_to_list(room.edges, room.edgecount, sizeof(t_vector2));
	save_chunk(room.name, "EDGE", edgelist);
	close(fd);
}

t_list	*entitycache_to_list(t_entitycache *cache)
{
	int				i;
	int				found;
	t_list			*entitylist;

	i = 0;
	found = 0;
	entitylist = NULL;
	//printf("cache has %i existing entities\n", cache->existing_entitycount);
	while (found < cache->existing_entitycount)
	{
		if (cache->entities[i].status != es_free)
		{
			//printf("found %i \n", found);
			cache->entities[i].id = found;
			//printf("entitylist null status before push %i \n", entitylist == NULL);
			list_push(&entitylist, &cache->entities[i], sizeof(t_entity));
			found++;
		}
		i++;
	}
	return (entitylist);
}

void	save_world(char *filename, t_world world)
{
	int	fd;

	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC); //Empty the file or create a new one if it doesn't exist
	close(fd);
	printf("Saving world\n");
	t_list	*entitylist = entitycache_to_list(&world.entitycache);
	printf("Entitycache converted to list\n");
	save_chunk(filename, "ENT_", entitylist);
	//TODO: free the dumb list

	printf("Saving rooms\n");
	t_list	*r;
	r = world.roomlist;
	//TODO: 
	save_chunk(filename, "RMNM", world.roomlist);
	while (r != NULL)
	{
		save_room(*(t_room *)r->content);
		r = r->next;
	}
	
	//close(fd);
}
