/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfum <kfum@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 17:40:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 15:30:36 by kfum             ###   ########.fr       */
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
		&& i < world->entitycache.alloc_count)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->component.func_ui_update != NULL)
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

	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->component.func_update != NULL)
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

static void	sort_entitycache(t_world *world, t_vector3 location)
{
	int			i;
	t_entity	*key;
	int			j;
	int			found;

	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		key = world->entitycache.sorted_entities[i];
		if (key->status != es_free)
		{
			j = i - 1;
			while (j >= 0 && world->entitycache.sorted_entities[j]
				->occlusion.z_dist[1] > key->occlusion.z_dist[1])
			{
				if (key->status != es_free)
					world->entitycache.sorted_entities[j + 1]
						= world->entitycache.sorted_entities[j];
				j--;
			}
			world->entitycache.sorted_entities[j + 1] = (found++, key);
		}
		i++;
	}
}

static void	world_update_debug_gui(t_world *world,
								t_sdlcontext *sdl, t_render *render)
{
	gui_start(world->debug_gui);
	if (gui_shortcut_button("Toggle noclip", 'F', world->debug_gui))
		world->player->noclip = !world->player->noclip;
	if (gui_shortcut_button("Show navmesh:", 'N', world->debug_gui))
		world->nav.show_navmesh = !world->nav.show_navmesh;
	gui_labeled_float_slider("Navigation node size: ",
		&world->nav.clip_size, 10.0f, world->debug_gui);
	if (gui_shortcut_button("Create navmesh:", 'C', world->debug_gui))
		create_navmesh(world);
	if (gui_shortcut_button("Toggle Lighting", 'L', world->debug_gui))
		sdl->lighting_toggled = !sdl->lighting_toggled;
	if (gui_shortcut_button("Draw Occlusion Buffer", 'Y', world->debug_gui))
		sdl->render.occlusion.draw_occlusion
			= !sdl->render.occlusion.draw_occlusion;
	if (gui_shortcut_button("Toggle Occlusion", 'O', world->debug_gui))
		render->occlusion.occlusion = !render->occlusion.occlusion;
	if (gui_shortcut_button("Toggle Occlusion boxes", 'P', world->debug_gui))
		render->occlusion.occluder_box = !render->occlusion.occluder_box;
	if (gui_shortcut_button("Render Next Frame Slow", 'U', world->debug_gui))
		sdl->render.occlusion.slow_render = true;
	if (gui_shortcut_button("Bake lighting (new)", 'b', world->debug_gui))
		recalculate_lighting(world);
	gui_end(world->debug_gui);
}


static void	draw_skybox_image(t_world *world, t_img *skybox)
{
	t_point pointX1;
	t_point pointX2;
	float rotX;
	float rotY;

	rotX = fmodf(world->player->transform.rotation.x / (2 * PI), 1.0f);
	rotY = fmodf(world->player->transform.rotation.y / PI, 1.0f);	
	skybox = get_image_by_name(*world->sdl, "skybox.tga");	
	if (rotX < 0)
	{
		pointX1 = point_add(point_zero(), (t_point){(rotX * skybox->size.x) + world->sdl->screensize.x, (rotY * skybox->size.y)});
		pointX2 = point_add(point_zero(), (t_point){(rotX * skybox->size.x) - (skybox->size.x - world->sdl->screensize.x), (rotY * skybox->size.y)});
	}
	else
	{
		pointX1 = point_add(point_zero(), (t_point){rotX * skybox->size.x, (rotY * skybox->size.y)});
		pointX2 = point_add(point_zero(), (t_point){(rotX * skybox->size.x) - skybox->size.x, (rotY * skybox->size.y)});
	}	
	draw_image(*world->sdl, pointX1, *skybox, skybox->size);
	draw_image(*world->sdl, pointX2, *skybox, skybox->size);
}

void	update_world3d(t_world *world, t_render *render)
{
	t_sdlcontext	*sdl;
	t_img			*skybox;

	sdl = world->sdl;
	for_all_active_entities(world, render_entity_worldtriangles);
	render_start_new(sdl, world->player);
	draw_skybox_image(world, skybox);
	clear_occlusion_buffer(sdl);
	update_frustrum_culling(world, sdl, render);
	sort_entitycache(world, render->camera.position);
	update_entitycache(sdl, world, render);
	bitmask_to_pixels(sdl);
	rescale_surface(sdl);
	lateupdate_entitycache(sdl, world);
	if (world->app_mode == APPMODE_EDIT && !world->debug_gui->hidden)
		world_update_debug_gui(world, sdl, render);
}
