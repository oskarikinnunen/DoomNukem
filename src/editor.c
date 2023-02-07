/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/07 16:26:26 by raho             ###   ########.fr       */
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

void	editor_load_world_args(t_editor *ed, char	*worldname, t_sdlcontext *sdl, t_load_arg args)
{
	ed->world = load_world_args(worldname, sdl, args);
	*(ed->world.debug_gui) = init_gui(sdl, &ed->hid, &ed->player, sdl->screensize, "Debugging menu (F2)");
	
	ed->toolbar_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point){5, 5}, "Toolbar (F1)");
	ed->toolbar_gui.minimum_size = (t_point){165, 20};
	ed->toolbar_gui.locked = true;
	
	ed->graphics_gui = init_gui(sdl, &ed->hid, &ed->player, sdl->screensize, "Graphics (F3)");
	ed->graphics_gui.minimum_size = (t_point){200, 200};
	ed->graphics_gui.rect.position = point_div(sdl->screensize, 2);
	//ed->graphics_gui.locked = true;
	ed->player.noclip = true;
	player_init(&ed->player, sdl, &ed->world);
	ed->player.gun->disabled = true;
	ed->world.debug_gui->hidden = true;
	ed->graphics_gui.hidden = true;
	ed->world.player = &ed->player;
}

void	editor_load_world(t_editor *ed, char	*worldname, t_sdlcontext *sdl)
{
	ed->world = load_world(world_fullpath(worldname), sdl);
	*(ed->world.debug_gui) = init_gui(sdl, &ed->hid, &ed->player, sdl->screensize, "Debugging menu (F2)");
	
	ed->toolbar_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point){5, 5}, "Toolbar (F1)");
	ed->toolbar_gui.minimum_size = (t_point){165, 20};
	ed->toolbar_gui.locked = true;
	
	ed->graphics_gui = init_gui(sdl, &ed->hid, &ed->player, sdl->screensize, "Graphics (F3)");
	ed->graphics_gui.minimum_size = (t_point){200, 200};
	ed->graphics_gui.rect.position = point_div(sdl->screensize, 2);
	//ed->graphics_gui.locked = true;
	ed->player.noclip = true;
	player_init(&ed->player, sdl, &ed->world);
	ed->player.gun->disabled = true;
	ed->world.debug_gui->hidden = true;
	ed->graphics_gui.hidden = true;
	ed->world.player = &ed->player;
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
	FMOD_System_Set3DListenerAttributes(sdl->audio.system, 0,(FMOD_VECTOR *)&world->player->headposition,
															&((FMOD_VECTOR){0}), (FMOD_VECTOR *)&nf, &((FMOD_VECTOR){.z = 1.0f}));
	FMOD_System_Update(sdl->audio.system);
}

char	*seconds_since_last_save_str(t_world *world)
{
	static char str[128];
	uint32_t	time_m;
	uint32_t	time_s;
	char		*temp;

	time_s = world->clock.prev_time - world->lastsavetime;
	time_s = time_s / 1000;
	time_m = 0;
	if (world->lastsavetime == 0)
		ft_strcpy(str, "Loaded ");
	else
		ft_strcpy(str, "Saved  ");
	if (time_s > 60)
	{
		time_m = floorf((float)time_s / 60.0f);
		temp = ft_itoa(time_m);
		ft_strcat(str, temp);
		free(temp);
		ft_strcat(str, "m ");
	}
	temp = ft_itoa(time_s - (time_m * 60));
	ft_strcat(str, temp);
	free(temp);
	ft_strcat(str, "s ago");
	return (str);
}

void	draw_level_info(t_sdlcontext *sdl, t_world *world)
{
	static t_point real_c;
	char	*time_str;

	if (real_c.x == 0)
	{
		real_c = print_text_boxed(sdl, world->name, (t_point){30, 0}).size;
		real_c = point_sub(point_div(sdl->screensize, 2), real_c);
		real_c.y = sdl->screensize.y - 30;
	}
	print_text_boxed(sdl, world->name, real_c);
	time_str = seconds_since_last_save_str(world);
	print_text_boxed(sdl, time_str, point_add(real_c, (t_point){0, 15}));
}

int	editorloop(t_sdlcontext sdl)
{
	t_editor	ed;
	bool		audio = 0;

	bzero(&ed, sizeof(t_editor));
	editor_load_prefs(&ed, &sdl);
	ed.gamereturn = game_continue;
	sdl.lighting_toggled = false;
	ed.world.lighting.calculated = false;
	//play_music(&sdl, "music_arp1_ambient.wav");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeees");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeeesxxx");
	doomlog(LOG_NORMAL, "messageeees");
	while (ed.gamereturn == game_continue)
	{
		update_deltatime(&ed.world.clock);
		ed.gamereturn = editor_events(&ed);
		bake_lights(&sdl.render, &ed.world);
		if (!ed.player.locked)
			moveplayer(&ed.player, &ed.hid.input, &ed.world);
		update_render(&sdl.render, &ed.player);
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
		draw_level_info(&sdl, &ed.world);
		update_editor_lateguis(&ed);
		update_debugconsole(&ed.world.debugconsole, &sdl, ed.world.clock.delta);
		ed.hid.mouse.click_unhandled = false;
		memcpy(sdl.window_surface->pixels, sdl.surface->pixels, sizeof(uint32_t) * sdl.window_w * sdl.window_h);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			doomlog(LOG_EC_SDL_UPDATEWINDOWSURFACE, NULL);
		update_audio(&ed.world);
		//play_localsound()
		//doomlog(LOG_FATAL, "fatal error");
		ft_bzero(NULL, 10); // forces a crash (for error test purposes only) remove this
	}
	editor_save_prefs(&ed);
	save_graphics_prefs(&sdl);
	//save_world(ed.world.name, ed.world);
	world_save_to_file(ed.world);
	free_render(sdl.render);
	if (ed.gamereturn == game_exit)
		quit_game(&sdl);
	return (ed.gamereturn);
}