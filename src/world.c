	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 17:40:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/10 12:53:30 by okinnune         ###   ########.fr       */
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
			//update_entity_bounds(cur->entity);
		}
		i++;
	}
}

t_vector3 pathfind(t_world *world, uint32_t start, uint32_t end)
{
	t_navnode	openlist[1000];
	uint32_t	o_amount;
	uint32_t	lowest_f;
	int	i;

	bzero(openlist, sizeof(t_navnode) * 1000);
	openlist[start] = world->navmesh[start];
	openlist[start].valid = true;
	//printf("start %d, end %d\n\n", start, end);
	o_amount = 1;
	while (o_amount > 0)
	{
		uint32_t found = 0;
		i = 0;
		while (found < o_amount)
		{
			if (openlist[i].valid == true)
			{
				if (found == 0 || openlist[i].f < openlist[lowest_f].f)
					lowest_f = i;
				found++;
			}
			i++;
			if (i > 999)
			{
				printf("buffer overflow pathfind\n");
				exit(0);
			}
		}
		if (lowest_f == end)
		{
			int e;
			e = openlist[end].parent;
			t_vector3 tempe;

			//printf("e %d\n", end);
			while (e != start)
			{
				//printf("e %d\n", e);
				tempe = openlist[e].mid_point;
				e = openlist[e].parent;
			}
			//printf("e %d\n", e);
			//printf("found goal!\n");
			return(tempe);
		}
		openlist[lowest_f].valid = false;
		openlist[lowest_f].visited = true;
		o_amount--;
		i = 0;
		while (i < openlist[lowest_f].neighbors)
		{
			uint32_t id;
			id = openlist[lowest_f].neighbors_id[i];
			if (openlist[id].visited == false)
			{
				openlist[id] = world->navmesh[id];
				openlist[id].g = vector3_dist(openlist[id].mid_point, world->navmesh[start].mid_point); // should be amount of parents to start
				openlist[id].h = vector3_dist(openlist[id].mid_point, world->navmesh[end].mid_point);
				openlist[id].f = openlist[id].g + openlist[id].h;
				openlist[id].valid = true;
				openlist[id].parent = lowest_f;
				o_amount++;
			}
			i++;
		}
		//printf("o amount %d %d\n", o_amount, lowest_f);
	}
	printf("goal not found\n");
	exit(0);
}

void	update_entitycache(t_sdlcontext *sdl, t_world *world, t_render *render)
{
	int			i;
	int			found;
	t_entity	*ent;

	
	i = 0;
	found = 0;
	bool test = false;
	while (found < world->entitycache.existing_entitycount
		/*&& i < world->entitycache.alloc_count*/)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
			{
				if (ent->obj->bounds.type == bt_box && render->occlusion.occlusion == false)
				{
					int e = 1;
					test = true;
					int closest_point = 0;
					float dist;
					dist = vector3_dist(world->navmesh[0].mid_point, ent->transform.position);
					while (e < world->node_amount)
					{
						if (dist > vector3_dist(world->navmesh[e].mid_point, ent->transform.position))
						{
							dist = vector3_dist(world->navmesh[e].mid_point, ent->transform.position);
							closest_point = e;
						}
						e++;
					}
					e = closest_point;
					t_vector3 tempe = pathfind(world, e, world->node_amount - 1);
					ent->transform.position = vector3_lerp(ent->transform.position, tempe, 5.0f / vector3_dist(ent->transform.position, tempe));
				}
				if (is_entity_culled(sdl, render, ent) == false)
					render_entity(sdl, render, ent);
			}
			found++;
		}
		i++;
	}
	sdl->render.occlusion.slow_render = false;
}

static void sort_entitycache(t_world *world, t_vector3 location)
{
	int			i;
	t_entity	*key;
	int			j;
	int			found;
	t_entity	*ent;
	int e;

	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = world->entitycache.sorted_entities[i];
		key = ent;
		if (ent->status != es_free)
		{
			j = i - 1;
			while (j >= 0 && world->entitycache.sorted_entities[j]->occlusion.z_dist[1] > key->occlusion.z_dist[1])
			{
				if (ent->status != es_free)
					world->entitycache.sorted_entities[j + 1] = world->entitycache.sorted_entities[j];
				j--;
			}
			found++;
			world->entitycache.sorted_entities[j + 1] = key;
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
	if (!world->player->gun->disabled)
		render_entity(world->sdl, &world->sdl->render, &world->player->gun->entity);
	update_frustrum_culling(world, sdl, render);
	clear_occlusion_buffer(sdl);
	sort_entitycache(world, render->camera.position);
	update_entitycache(sdl, world, render);
	if (!sdl->global_wireframe && !world->skybox.hidden)
		render_entity(sdl, render, &world->skybox);
	bitmask_to_pixels(sdl);
	rescale_surface(sdl);
	gui_start(world->debug_gui);
	gui_labeled_int("Tri count:", render->rs.triangle_count, world->debug_gui);
	gui_labeled_int("Render count:", render->rs.render_count, world->debug_gui);
	gui_labeled_int("Entity count:", world->entitycache.existing_entitycount, world->debug_gui);
	if (gui_shortcut_button("Toggle ceilings:", 'J', world->debug_gui))
	{
		world->ceiling_toggle = !world->ceiling_toggle;
		toggle_ceilings(world);
	}
	if (gui_shortcut_button("Toggle grids:", 'G', world->debug_gui))
		world->sdl->render_grid = !world->sdl->render_grid;
	if (gui_labeled_float_slider("Audio max:", &world->sdl->audio.max_volume, 0.01f, world->debug_gui))
		update_maxvolume(&world->sdl->audio);
	if (gui_button("Test audio", world->debug_gui))
	{
		static bool	statement = true;

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
			//play_worldsound(&sdl->audio, "bubbles.wav", &world->entitycache.entities[0].transform.position);
			play_localsound(&sdl->audio, "bubbles.wav");
			statement = true;
		}
		
	}
	world->sdl->audio.max_volume = ft_clampf(world->sdl->audio.max_volume, 0.25f, 1.0f);
	if (gui_shortcut_button("Toggle rendering:", 'R', world->debug_gui))
		world->sdl->global_wireframe = !world->sdl->global_wireframe;
	if (gui_shortcut_button("Toggle Lighting", 'L', world->debug_gui))
		sdl->lighting_toggled = !sdl->lighting_toggled;
	//gui_labeled_bool_edit("Lighting:", &world->sdl->lighting_toggled, world->debug_gui);
	gui_labeled_int_slider("PS1 tri div:", &sdl->ps1_tri_div, 2.0f, world->debug_gui);
	if (gui_shortcut_button("Toggle Skybox", 'H', world->debug_gui))
		world->skybox.hidden = !world->skybox.hidden;
	if (gui_shortcut_button("Draw Occlusion Buffer", 'Y', world->debug_gui))
		sdl->render.occlusion.draw_occlusion = !sdl->render.occlusion.draw_occlusion;
	if (gui_shortcut_button("Toggle Occlusion", 'O', world->debug_gui))
		render->occlusion.occlusion = !render->occlusion.occlusion;
	if (gui_shortcut_button("Toggle Occlusion boxes", 'P', world->debug_gui))
		render->occlusion.occluder_box = !render->occlusion.occluder_box;
	if (gui_shortcut_button("Render Next Frame Slow", 'U', world->debug_gui))
		sdl->render.occlusion.slow_render = true;
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
	entity->lightmap = NULL;
	entity->map = NULL; //TODO: load maps here
	default_entity_occlusion_settings(entity, NULL);
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
	temp = load_chunk(filename, "WALL", sizeof(t_wall));
	result.walls = list_to_ptr(temp, &result.wallcount);
	listdel(&temp);
	temp = load_chunk(filename, "FLOR", sizeof(t_meshtri));
	result.floors = list_to_ptr(temp, &result.floorcount);
	listdel(&temp);

	temp = load_chunk(filename, "EDGE", sizeof(t_vector2));
	result.edges = list_to_ptr(temp, &result.edgecount);
	listdel(&temp);

	temp = load_chunk(filename, "EDGE", sizeof(t_vector2));
	result.edges = list_to_ptr(temp, &result.edgecount);
	listdel(&temp);
	return (result);
}

void	load_room_new(char	*worldname, char *filename, t_room *room)
{
	t_list			*temp;
	int				fd;

	fd = load_filecontent_fd(worldname, filename);
	//load_filecontent()
	temp = load_chunk(filename, "WALL", sizeof(t_wall));
	room->walls = list_to_ptr(temp, &room->wallcount);
	listdel(&temp);
	temp = load_chunk(filename, "FLOR", sizeof(t_meshtri));
	room->floors = list_to_ptr(temp, &room->floorcount);
	listdel(&temp);
	temp = load_chunk(filename, "EDGE", sizeof(t_vector2));
	room->edges = list_to_ptr(temp, &room->edgecount);
	listdel(&temp);
	close(fd);
	remove(filename);
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
		w->entity->obj = object_plane(world->sdl);
		entity_assign_object(world, w->entity, object_plane(world->sdl));
		w->entity->obj->materials[0].img = get_image_by_name(*world->sdl, w->texname);
		i++;
	}
	i = 0;
	//r->floors = NULL;
	//r->floorcount == 0;
	//make_areas(world, r);
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

void	load_rooms(t_world *world, char *worldname, t_sdlcontext *sdl)
{
	t_list	*l;
	t_room	*r;
	t_room	temp;
	int		i;

	l = world->roomlist;
	while (l != NULL)
	{
		r = (t_room *)l->content;
		load_room_new(worldname, r->name, r);
		startup_init_room(world, r);
		l = l->next;
	}
	l = world->roomlist;
	while (l != NULL)
	{
		r = (t_room *)l->content;
		init_roomwalls(world, r);
		//make_areas(world, r);
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
	printf("%i entities exist after remove \n", cache->existing_entitycount);
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
			ft_bzero(&cache->entities[i], sizeof(t_entity));
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
	//create_lightmap_for_entity(entity, world);
	//create_map_for_entity(entity, world);
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

void	init_skybox(t_world *world)
{
	ft_bzero(&world->skybox, sizeof(t_entity));
	world->skybox.obj = get_object_by_name(*world->sdl, "cube");
	world->skybox.obj->materials[0].img = get_image_by_name(*world->sdl, "grid_d.cng");
	world->skybox.transform.scale = vector3_mul(vector3_one(), 300.0f);
	world->skybox.transform.position = (t_vector3){1500.0f, 1500.0f, 1497.5f};
}

char	*full_worldpath(char *name)
{
	static char	full[128];

	ft_bzero(full, sizeof(full));
	ft_strcat(full, "worlds/");
	ft_strcat(full, name);
	return (full);
}

char	*worldname(char	*filename)
{
	char	*occurence;

	occurence = ft_strstr(filename, "worlds/");
	if (occurence != NULL)
		return(occurence + sizeof("worlds"));
	return (filename);
}

t_world	load_world(char *filename, t_sdlcontext *sdl)
{
	t_world	world;

	ft_bzero(&world, sizeof(t_world));
	ft_strcpy(world.name, worldname(filename));
	world.sdl = sdl;
	t_vector2 v;
	world.guns = load_chunk(filename, "GUNS", sizeof(t_gun));
	world.debugconsole = init_debugconsole();
	world.entitycache = init_entitycache(1024);
	t_list	*entitylist = load_chunk(filename, "ENT_", sizeof(t_entity));
	load_cache_from_list(&world, entitylist);
	for_all_entities(&world, init_entity);
	world.debug_gui = ft_memalloc(sizeof(t_autogui)); //Y tho?
	world.roomlist = load_chunk(filename, "RMNM", sizeof(t_room));
	load_rooms(&world, filename, sdl);
	init_guns(&world, sdl);
	init_skybox(&world);
	ft_bzero(&world.lighting, sizeof(t_lighting));
	world.lighting.ambient_light = 20;
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

void	save_world(char *namename, t_world world)
{
	int		fd;
	char	*filename;

	filename = full_worldpath(namename);
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
	save_filecontent(filename, "assets/images/something.png");
	//load_filecontent()
	//load_filecontent_fd()
	while (r != NULL)
	{
		save_room(*(t_room *)r->content);
		save_filecontent(filename, (*(t_room *)r->content).name);
		remove((*(t_room *)r->content).name);
		r = r->next;
	}
	
}
