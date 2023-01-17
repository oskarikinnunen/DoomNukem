/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/17 15:24:18 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "render.h"
#include "objects.h"
#include "entity.h"
#include "navigation.h"


char	*world_fullpath(char	*filename)
{
	static char	fullname[128];

	ft_bzero(fullname, sizeof(fullname));
	ft_strcat(fullname, "worlds/");
	ft_strcat(fullname, filename);
	//ft_strcat(fullname, ".world");
	return	(fullname);
}

void	editor_load_and_init_world(t_editor *ed, char	*worldname, t_sdlcontext *sdl)
{
	ed->world = load_world(world_fullpath(worldname), sdl);
	*(ed->world.debug_gui) = init_gui(sdl, &ed->hid, &ed->player, sdl->screensize, "Debugging menu (F2)");
	ed->world.debug_gui->minimum_size.y = 135;
	ed->world.debug_gui->rect.position = sdl->screensize;
	ed->toolbar_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point){5, 5}, "Toolbar (F1)");
	ed->toolbar_gui.minimum_size = (t_point){165, 20};
	ed->toolbar_gui.locked = true;
	//ed->world.debug_gui
	
	
	ed->graphics_gui = init_gui(sdl, &ed->hid, &ed->player, sdl->screensize, "Graphics (F3)");
	ed->graphics_gui.minimum_size = (t_point){200, 200};
	ed->graphics_gui.rect.position = point_div(sdl->screensize, 2);
	//ed->graphics_gui.locked = true;

	player_init(&ed->player, sdl, &ed->world);
	ed->player.gun->disabled = true;
	ed->world.debug_gui->hidden = true;
	ed->graphics_gui.hidden = true;
	ed->world.player = &ed->player;
	//update_debugconsole(&ed->world.debugconsole, sdl, ed->world.clock.delta);
	//debugconsole_addmessage(&ed->world.debugconsole, "Loaded world");
	//ed->world.debugconsole.
}

/*
	Fullscreen
	borderless
	windowed

*/

t_point	*get_resolutions()
{
	static t_point resolutions[32] =
	{
		{320, 240},
		{640, 480},
		{800, 600},
		{1024, 768},
		{960, 540},
		{1280, 720},
		{1366, 768},
		{1600, 900},
		{1920, 1080},
		{2560, 1440}
	};

	return (resolutions);
}

char	*resolution_str(t_point res)
{
	char	*str1;
	char	*str2;
	char	*final;

	str1 = ft_itoa(res.x);
	str2 = ft_itoa(res.y);

	final = ft_strnew(ft_strlen(str1) + ft_strlen(str2) + 1);
	ft_strcat(final, str1);
	ft_strcat(final, "x");
	ft_strcat(final, str2);
	free(str1);
	free(str2);
	return (final);
}

void	screenmode_toggles(t_autogui *gui, t_graphicprefs *prefs)
{
	if (prefs->screenmode == screenmode_fullscreen)
	{
		if (gui_highlighted_button("Fullscreen", gui))
			prefs->screenmode = screenmode_fullscreen;
		if (gui_button("Windowed", gui))
			prefs->screenmode = screenmode_windowed;
	}
	else
	{
		if (gui_button("Fullscreen", gui))
			prefs->screenmode = screenmode_fullscreen;
		if (gui_highlighted_button("Windowed", gui))
			prefs->screenmode = screenmode_windowed;
	}
}

void	update_editor_lateguis(t_editor *ed) //TODO: make this graphics_settings_gui
{
	t_autogui				*gui;
	static	t_graphicprefs	prefs;
	t_point					*resolutions;
	int						i;
	char					*str;
	
	gui = &ed->graphics_gui;
	resolutions = get_resolutions();

	gui_start(gui);
	gui_label("Select resolution:", gui);
	prefs = get_prefs_from_sdl(gui->sdl);
	i = 0;
	while (i < 10)
	{
		str = resolution_str(resolutions[i]);
		if ((point_cmp(ed->world.sdl->screensize, resolutions[i]) && gui_highlighted_button(str, gui))
			|| (!point_cmp(ed->world.sdl->screensize, resolutions[i]) && gui_button(str, gui)))
		{
			prefs.resolution_x = resolutions[i].x;
			prefs.resolution_y = resolutions[i].y;
			apply_graphics_prefs(prefs);
			set_sdl_settings(ed->world.sdl);
		}
		free(str);
		i++;
	}
	gui_labeled_float_slider("Audio volume:", &gui->sdl->audio.sfx_volume, 0.01f, gui);
	gui->sdl->audio.sfx_volume = ft_clampf(gui->sdl->audio.sfx_volume, 1.0f, 10.0f);
	if (gui_labeled_float_slider("Music volume:", &gui->sdl->audio.music_volume, 0.01f, gui))
	{
		gui->sdl->audio.music_volume = ft_clampf(gui->sdl->audio.music_volume, 0.0f, 1.0f);
		FMOD_Channel_SetVolume(gui->sdl->audio.music_channel, gui->sdl->audio.music_volume);
	}
	
	gui_labeled_float_slider("Resolution scale:", &gui->sdl->resolution_scaling, 0.01f, gui);
	gui->sdl->resolution_scaling = ft_clampf(gui->sdl->resolution_scaling, 0.25f, 1.0f);
	prefs.resolutionscale = gui->sdl->resolution_scaling;
	t_point	res;
	res = point_fmul(gui->sdl->screensize, gui->sdl->resolution_scaling);
	gui_labeled_point("3D Resolution:", res, gui);
	gui_labeled_bool_edit("Blend:", &gui->sdl->blend, gui);
	if (gui_button("Apply settings", gui))
	{
		apply_graphics_prefs(prefs);
		set_sdl_settings(ed->world.sdl);
	}
	screenmode_toggles(gui, &prefs);
	gui_end(gui);
}

void	update_audio(t_world *world)
{
	t_vector3		nf;
	t_sdlcontext	*sdl;
	
	sdl = world->sdl;
	nf = world->player->lookdir;
	nf = (t_vector3){-nf.x, -nf.y, 0.0f};
	nf = vector3_normalise(nf);
	//sdl->audio
	if (sdl->audio.music_control.active)
	{
		t_music_control	*mc;

		mc = &sdl->audio.music_control;
		if (!mc->lastfade)
		{
			mc->fade = ft_fmovetowards(mc->fade, 0.0f, world->clock.delta * 0.0005f * sdl->audio.music_volume);
			if (mc->fade == 0.0f)
			{
				FMOD_Channel_SetVolume(sdl->audio.music_channel, 0.0f);
				FMOD_Channel_SetPaused(sdl->audio.music_channel, true);
				FMOD_System_PlaySound(sdl->audio.system, mc->nextsong.sound, NULL, true, &sdl->audio.music_channel);
				FMOD_Channel_SetPaused(sdl->audio.music_channel, false);
				mc->lastfade = true;
			}
		}
		else
		{
			mc->fade = ft_fmovetowards(mc->fade, sdl->audio.music_volume, world->clock.delta * 0.00025f * sdl->audio.music_volume);
			if (mc->fade == sdl->audio.music_volume)
				mc->active = false;
		}
		
		FMOD_Channel_SetVolume(sdl->audio.music_channel, sdl->audio.music_control.fade);
		
	}
	FMOD_System_Set3DListenerAttributes(sdl->audio.system, 0, (FMOD_VECTOR *)&world->player->transform.position,
															&((FMOD_VECTOR){0}), (FMOD_VECTOR *)&nf, &((FMOD_VECTOR){.z = 1.0f}));
	FMOD_System_Update(sdl->audio.system);
}

int	editorloop(t_sdlcontext sdl)
{
	t_editor	ed;
	bool		audio = 0;

	bzero(&ed, sizeof(t_editor));
	editor_load_prefs(&ed, &sdl);
	ed.gamereturn = game_continue;
	sdl.lighting_toggled = false;
	t_world *world = &ed.world;
	int			i;
	int			found;
	t_entity	*ent;

	
	i = 0;
	found = 0;
	world->node_amount = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
			{
				if (ent->obj->bounds.type == bt_ignore)
				{
					memcpy(world->navmesh[world->node_amount].vertex, ent->obj->vertices, sizeof(t_vector3) * 3);
					world->navmesh[world->node_amount].mid_point = vector3_div(vector3_add(vector3_add(world->navmesh[world->node_amount].vertex[0], world->navmesh[world->node_amount].vertex[1]), world->navmesh[world->node_amount].vertex[2]), 3.0f);
					world->node_amount++;
				}
			}
			found++;
		}
		i++;
	}
	for (int i = 0; i < world->node_amount; i++)
	{
		//setup cell
		world->navmesh[i].neighbors = 0;
		world->navmesh[i].valid = true;
		for (int j = 0; j < 3; j++)
		{
			t_vector3 start = world->navmesh[i].vertex[j];
			t_vector3 end = world->navmesh[i].vertex[(j + 1) % 3];
			for (int i1 = 0; i1 < world->node_amount; i1++)
			{
				if (i == i1)
					continue;
				int count = 0;
				for (int j1 = 0; j1 < 3; j1++)
				{
					if (vector3_cmp(start, world->navmesh[i1].vertex[j1]))
						count++;
					if (vector3_cmp(end, world->navmesh[i1].vertex[j1]))
						count++;
				}
				if (count == 2)
				{
					world->navmesh[i].line_point[world->navmesh[i].neighbors] = vector3_lerp(start, end, 0.5f);
					world->navmesh[i].neighbors_id[world->navmesh[i].neighbors++] = i1;
				}
			}
		}
		printf("neighbors amount %d self %d\n", world->navmesh[i].neighbors, i);
		for (int e = 0; e < world->navmesh[i].neighbors; e++)
		{
			printf("%d ", world->navmesh[i].neighbors_id[e]);
		}
		printf("\n");
	}
	int node_count = world->node_amount;
	for (int i = 0; i < world->node_amount; i++)
	{
		t_navnode temp;
		t_triangle test;
		temp = world->navmesh[i];
		float dist;
		dist = vector3_dist(world->navmesh[i].vertex[0], world->navmesh[i].vertex[1]);
		int index = 0;
		for (int j = 1; j < 3; j++)
		{
			if (dist < vector3_dist(world->navmesh[i].vertex[j], world->navmesh[i].vertex[(j + 1) % 3]))
			{
				dist = vector3_dist(world->navmesh[i].vertex[j], world->navmesh[i].vertex[(j + 1) % 3]);
				index = j;
			}
		}
		for (int j = 0; j < 3; j++)
		{
			test.p[j].v = world->navmesh[i].vertex[j];
		}
		t_vector3 plane_p = vector3_lerp(world->navmesh[i].vertex[index], world->navmesh[i].vertex[(index + 1) % 3], 0.5f);
		t_vector3 plane_n = vector3_normalise(vector3_sub(world->navmesh[i].vertex[index], world->navmesh[i].vertex[(index + 1) % 3]));
	
		t_triangle out[4];
		int count = 0;
		count = clip_triangle_against_plane(plane_p, plane_n, test, out);
		plane_n = vector3_negative(plane_n);
		count += clip_triangle_against_plane(plane_p, plane_n, test, &out[count]);
		for (int i1 = 0; i1 < count; i1++)
		{
			world->navmesh[node_count].neighbors = 0;
			for (int i2 = 0; i2 < count; i2++)
			{
				if (i2 == i1)
					continue;
				world->navmesh[node_count].neighbors_id[world->navmesh[node_count].neighbors++] = node_count + i2;
			}
			for (int j = 0; j < temp.neighbors; j++)
			{
				world->navmesh[node_count].neighbors_id[world->navmesh[node_count].neighbors++] = temp.neighbors_id[j];
			}
			for (int j = 0; j < 3; j++)
			{
				world->navmesh[node_count].vertex[j] = out[i1].p[j].v;
			}
			world->navmesh[i].neighbors_id[world->navmesh[i].neighbors++] = node_count;
			world->navmesh[node_count].mid_point = vector3_div(vector3_add(vector3_add(world->navmesh[node_count].vertex[0], world->navmesh[node_count].vertex[1]), world->navmesh[node_count].vertex[2]), 3.0f);
			node_count++;
		}
		//triangle_clipagainstplane(plane_p, plane_n, test, out);
	}
	world->node_amount = node_count;
//	printf("node amount %d\n", world->node_amount);
//	exit(0);
	ed.world.lighting.calculated = false;
	play_music(&sdl, "music_arp1_ambient.wav");
	while (ed.gamereturn == game_continue)
	{
		update_deltatime(&ed.world.clock);
		ed.gamereturn = editor_events(&ed);
		bake_lights(&sdl.render, &ed.world);
		if (!ed.player.locked)
			moveplayer(&ed.player, &ed.hid.input, &ed.world);
		update_render(&sdl.render, &ed.player);
		//print_vector3(sdl.render.camera.lookdir);
		//print_vector3(sdl.render.camera.position);
		if (0)
		{
			sdl.render.camera.lookdir = (t_vector3){-0.604028, 0.055242, -0.795046};
			sdl.render.camera.position = (t_vector3){1559.231812, 1055.679810, 294.330811};
		}
		screen_blank(sdl);
		render_start(&sdl.render);
		update_frustrum_culling(&ed.world, &sdl, &sdl.render);
		clear_occlusion_buffer(&sdl);
		if (!ed.player.gun->disabled)
			render_entity(&sdl, &sdl.render, &ed.player.gun->entity);
		update_world3d(&ed.world, &sdl.render);
		update_editor_toolbar(&ed, &ed.toolbar_gui);
		if (ed.tool != NULL)
			ed.tool->update(&ed, &sdl);
		
		char *fps = ft_itoa(ed.world.clock.fps);
		print_text(&sdl, fps, (t_point){sdl.window_w - 80, 10});
		drawcircle(sdl, point_div(sdl.screensize, 2), 4, CLR_BLUE);
		free(fps);
		update_editor_lateguis(&ed);
		update_debugconsole(&ed.world.debugconsole, &sdl, ed.world.clock.delta);
		ed.hid.mouse.click_unhandled = false;
		memcpy(sdl.window_surface->pixels, sdl.surface->pixels, sizeof(uint32_t) * sdl.window_w * sdl.window_h);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
		update_audio(&ed.world);
		//play_localsound()
	}
	editor_save_prefs(&ed);
	save_graphics_prefs(&sdl);
	save_world(ed.world.name, ed.world);
	free_render(sdl.render);
	if (ed.gamereturn == game_exit)
		quit_game(&sdl);
	return (ed.gamereturn);
}