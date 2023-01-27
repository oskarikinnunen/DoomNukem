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

void	lateupdate_entitycache(t_sdlcontext *sdl, t_world *world)
{
	int			i;
	int			found;
	t_entity	*ent;

	
	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount
		/*&& i < world->entitycache.alloc_count*/)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if(ent->component.func_ui_update != NULL)
				ent->component.func_ui_update(ent, world);
			found++;
		}
		i++;
	}
}

void	update_entitycache(t_sdlcontext *sdl, t_world *world, t_render *render)
{
	int			i;
	int			found;
	t_entity	*ent;
	int		test1;
	
	i = 0;
	found = 0;
	bool test = false;
	while (found < world->entitycache.existing_entitycount
		/*&& i < world->entitycache.alloc_count*/)
	{
		ent = world->entitycache.sorted_entities[i];
	//	ent = &world->entitycache.entities[i];
		if (ent->status != es_free)
		{
			if(ent->component.func_update != NULL)
				ent->component.func_update(ent, world);
			if (ent->status == es_active && !ent->hidden)
			{
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
	if (world->player != NULL && world->player->gun != NULL && !world->player->gun->disabled)
		render_entity(world->sdl, &world->sdl->render, &world->player->gun->entity);
	show_navmesh(world);
	update_frustrum_culling(world, sdl, render);
	clear_occlusion_buffer(sdl);
	sort_entitycache(world, render->camera.position);
	update_entitycache(sdl, world, render);
	if (!sdl->global_wireframe && !world->skybox.hidden)
		render_entity(sdl, render, &world->skybox);
	bitmask_to_pixels(sdl);
	rescale_surface(sdl);

	lateupdate_entitycache(sdl, world);
	if (!world->debug_gui->hidden)
	{
		gui_start(world->debug_gui);
		/*if (gui_button("music action", world->debug_gui))
			change_music(sdl, "music_arp1_action.wav");
		if (gui_button("music calm", world->debug_gui))
			change_music(sdl, "music_arp1_ambient.wav");*/
		gui_labeled_int("Tri count:", render->rs.triangle_count, world->debug_gui);
		gui_labeled_int("Render count:", render->rs.render_count, world->debug_gui);
		gui_labeled_int("Entity count:", world->entitycache.existing_entitycount, world->debug_gui);
		if (gui_shortcut_button("Toggle ceilings:", 'J', world->debug_gui))
		{
			world->ceiling_toggle = !world->ceiling_toggle;
			toggle_ceilings(world);
		}
		if (gui_shortcut_button("Toggle grids:", 'G', world->debug_gui))
		{
			world->player->velocity.x += 15.0f;
			world->sdl->render_grid = !world->sdl->render_grid;
		}
			
		if (gui_shortcut_button("Toggle noclip", 'F', world->debug_gui))
			world->player->noclip = !world->player->noclip;
		//gui_labeled_bool_edit("Noclip:", &world->player->noclip, world->debug_gui);
		if (gui_shortcut_button("Show navmesh:", 'N', world->debug_gui))
				world->nav.show_navmesh = !world->nav.show_navmesh;
		gui_labeled_float_slider("Navigation node size: ", &world->nav.clip_size, 10.0f, world->debug_gui);
		if (gui_shortcut_button("Create navmesh:", 'C', world->debug_gui))
			create_navmesh(world);
		if (gui_shortcut_button("Toggle rendering:", 'R', world->debug_gui))
			world->sdl->global_wireframe = !world->sdl->global_wireframe;
		if (gui_shortcut_button("Toggle Lighting", 'L', world->debug_gui))
			sdl->lighting_toggled = !sdl->lighting_toggled;
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
		if (gui_shortcut_button("Bake lighting (new)", 'b', world->debug_gui))
			recalculate_lighting(world);
		gui_labeled_int_slider("Ps1 tri div: ", &sdl->ps1_tri_div, 0.2f, world->debug_gui);
		sdl->ps1_tri_div = ft_clamp(sdl->ps1_tri_div, 1, 4);
		gui_end(world->debug_gui);
	}
	
}

/*static void load_componentdata(t_world *world)
{

}*/

void	init_entity(t_entity *entity, t_world *world)
{
	t_componentdefinition	*defs;
	int						i;

	entity->obj = get_object_by_name(*world->sdl, entity->object_name);
	entity->lightmap = NULL;
	entity->map = NULL; //TODO: load maps here
	defs = get_component_definitions();
	i = 0;
	while (ft_strlen(defs[i].name) > 0)
	{
		if (entity->component.type == defs[i].type)
		{
			//assign_component_none(&entity->component);
			if (defs[i].func_assign_component != NULL)
				defs[i].func_assign_component(&entity->component);
			else
			{
				printf("Component definition is missing func_assign_component"); //TODO: move this protection to get_component_defs
				exit(0);
			}
			//printf("loaded component %i/%s\n", entity->id, defs[i].name);
			if (entity->component.func_loadassets != NULL)
			{
				//printf("	loading asset for %i/%s\n", entity->id, defs[i].name);
				entity->component.func_loadassets(entity, world);
			}
		}
		i++;
	}
	/*entity->component.type = COMP_NONE;
	entity->component.data = NULL;
	entity_set_component(entity, COMP_NONE, world);*/
	
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

	/*temp = load_chunk(filename, "EDGE", sizeof(t_vector2));
	result.edges = list_to_ptr(temp, &result.edgecount);
	listdel(&temp);*/
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
		//startup_init_room(world, r);
		l = l->next;
	}
	l = world->roomlist;
	while (l != NULL)
	{
		r = (t_room *)l->content;
		room_init(r, world);
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
	//start_anim(&console.show_anim, anim_forwards);
	//debugconsole_addmessage(&console, "Initialized debugconsole!");
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
	{
		printf("Tried to remove entity -1\n");
		error_log(EC_MALLOC);
	}
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
			ft_bzero(&cache->entities[i].component, sizeof(t_component));
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

void	load_component(t_entity	*entity, char	*filename)
{
	t_list	*temp;
	int		fd;
	char	*str;
	char	comp_filename[64];

	printf("trying to load component\n");
	str = ft_itoa(entity->id);
	ft_strcpy(comp_filename, str);
	free(str);
	ft_strcat(comp_filename, ".comp");
	fd = load_filecontent_fd(filename, comp_filename);
	if (fd != -1)
	{
		close(fd);
		temp = load_chunk(comp_filename, "COMP", entity->component.data_size);
		if (temp != NULL && temp->content != NULL)
		{
			printf("found component \n");
			entity->component.data = temp->content;
		}
		remove(comp_filename);
	}
}

void	load_cache_from_list(char *filename, t_world *world, t_list *l)
{
	t_entity	*list_entity;
	t_entity	*world_entity;
	char		comp_filename[64];
	char		*str;

	while (l != NULL)
	{
		list_entity = l->content;
		if (list_entity->component.type != COMP_NONE)
		{
			load_component(list_entity, filename);
		}
		//list_entity
		//load_filecontent(filename, "")
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
	world->skybox.transform.rotation = vector3_zero();
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
	printf("loaded %i entities from worldfile. \n", ft_listlen(entitylist));
	load_cache_from_list(filename, &world, entitylist);
	for_all_entities(&world, init_entity);
	world.debug_gui = ft_memalloc(sizeof(t_autogui)); //Y tho?
	world.roomlist = load_chunk(filename, "RMNM", sizeof(t_room));
	load_rooms(&world, filename, sdl);
	init_guns(&world, sdl);
	init_skybox(&world);
	ft_bzero(&world.lighting, sizeof(t_lighting));
	world.lighting.ambient_light = 20;
	world.nav.clip_size = 250.0f;
	//create_navmesh(&world);
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

void	save_room(t_room room)
{
	int		fd;

	printf("saving room '%s' \n", room.name);
	fd = fileopen(room.name, O_RDWR | O_CREAT | O_TRUNC); //Empty the file or create a new one if it doesn't exist
	t_list *walls_list = ptr_to_list(room.walls, room.wallcount, sizeof(t_wall));
	save_chunk(room.name, "WALL", walls_list);
	t_list *floorlist = ptr_to_list(room.floors, room.floorcount, sizeof(t_meshtri));
	save_chunk(room.name, "FLOR", floorlist);
	t_list *edgelist = ptr_to_list(room.edges, room.edgecount, sizeof(t_vector2));
	save_chunk(room.name, "EDGE", edgelist);
	close(fd);
}

/*
	cache
		list

			saveentity


*/

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
			//cache->entities[i].component.data = NULL;
			//cache->entities[i].component.type = pft_none; //TODO: save component to file with entity id
			//printf("entitylist null status before push %i \n", entitylist == NULL);
			list_push(&entitylist, &cache->entities[i], sizeof(t_entity));
			found++;
		}
		i++;
	}
	return (entitylist);
}



void	save_entities(char *filename, t_list	*entitylist)
{
	t_list		*l;
	t_list		temp;
	t_entity	*ent;
	char		componentfilename[64];

	l = entitylist;
	temp.next = NULL;
	while (l != NULL)
	{
		ent = l->content;
		if (ent->component.type != COMP_NONE)
		{
			temp.content = ent->component.data;
			temp.content_size = ent->component.data_size;
			char	*str;
			int		fd;
			str = ft_itoa(ent->id);
			ft_strcpy(componentfilename, str);
			free(str);
			ft_strcat(componentfilename, ".comp");
			fd = fileopen(componentfilename, O_RDWR | O_CREAT | O_TRUNC);
			close(fd);
			printf("saving component to file %s \n", componentfilename);
			save_chunk(componentfilename, "COMP", &temp);
			pack_file(filename, componentfilename);
			remove(componentfilename);
			//TODO: remove .comp
			ent->component.data = NULL;
			//ent->component.type = pft_none;
		}
		l = l->next;
	}
	save_chunk(filename, "ENT_", entitylist);
}

static void _world_remove_all_room_entities(t_world *world)
{
	t_list	*l;
	t_room	*room;

	l = world->roomlist;
	while (l != NULL)
	{
		room = (t_room *)l->content;
		room_remove_entities(room, world);
		l = l->next;
	}
}

void	save_world(char *namename, t_world world)
{
	int		fd;
	char	*filename;

	filename = full_worldpath(namename);
	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC); //Empty the file or create a new one if it doesn't exist
	close(fd);
	printf("Saving world\n");
	_world_remove_all_room_entities(&world);
	t_list	*entitylist = entitycache_to_list(&world.entitycache);
	save_entities(filename, entitylist);
	//TODO: free the dumb list

	printf("Saving rooms\n");
	t_list	*l;
	t_room	*room;
	l = world.roomlist;
	//TODO: 
	save_chunk(filename, "RMNM", world.roomlist);
	pack_file(filename, "assets/images/something.png");
	//load_filecontent()
	//load_filecontent_fd()
	while (l != NULL)
	{
		room = (t_room *)l->content;
		save_room(*room);
		pack_file(filename, room->name);
		remove(room->name);
		l = l->next;
	}
	
}
