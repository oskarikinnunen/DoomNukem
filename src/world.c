/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 17:40:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/08 19:25:37 by okinnune         ###   ########.fr       */
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

	//doomlog(LOG_FATAL, "shit something went wrong");
	sdl = world->sdl;
	ft_bzero(&render->rs, sizeof(t_render_statistics));
	if (world->player != NULL && world->player->gun != NULL && !world->player->gun->disabled)
		render_entity(world->sdl, &world->sdl->render, &world->player->gun->entity);
	update_frustrum_culling(world, sdl, render);
	clear_occlusion_buffer(sdl);
	sort_entitycache(world, render->camera.position);
	update_entitycache(sdl, world, render);
	if (!sdl->global_wireframe && !world->skybox.hidden)
		render_entity(sdl, render, &world->skybox);
	bitmask_to_pixels(sdl);
	rescale_surface(sdl);
	show_navmesh(world);
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
			//world->player->velocity.x += 15.0f;
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

	entity->obj = get_object_by_name(*world->sdl, entity->object_name.str);
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




void	init_guns(t_world *world, t_sdlcontext *sdl)
{
	t_list	*l;
	t_gun	*gun;

	l = world->guns;
	while (l != NULL)
	{
		gun = l->content;
		gun->entity.obj = get_object_by_name(*sdl, gun->entity.object_name.str);
		l = l->next;
	}
}

void		destroy_entity(t_world *world, t_entity *ent)
{
	t_entitycache	*cache;

	cache = &world->entitycache;
	//protect id here? if greater than alloccount
	cache->entities[ent->id].status = es_free;
	cache->entities[ent->id].obj = NULL;
	if (cache->existing_entitycount == 0)
		doomlog(LOG_EC_MALLOC, "Tried to remove entity -1\n");
	cache->existing_entitycount--;
	//printf("%i entities exist after remove \n", cache->existing_entitycount);
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
			cache->entities[i].hidden = false;
			ft_bzero(&cache->entities[i].component, sizeof(t_component));
			//cache->entities[i].transform.scale = vector3_zero();
			cache->existing_entitycount++;
			if (cache->existing_entitycount >= cache->alloc_count)
				doomlog(LOG_EC_MALLOC, NULL);
			return (&cache->entities[i]);
		}
		i++;
	}
	printf("ENTITYCACHE TOO SMALL, ABORT!");
	doomlog(LOG_EC_MALLOC, NULL);
	return (NULL); //never gets here
}

void		entity_assign_object(t_world *world, t_entity *entity, t_object *obj)
{
	ft_strcpy(entity->object_name.str, obj->name);
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


audiosource_preset1(t_audiosource source)
{
	source.range = 100.0f;
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

char	*worldname(char	*filename)
{
	char	*occurence;

	occurence = ft_strstr(filename, "worlds/");
	if (occurence != NULL)
		return(occurence + sizeof("worlds"));
	return (filename);
}
