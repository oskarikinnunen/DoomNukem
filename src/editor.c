/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/11 10:39:15 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "render.h"
#include "objects.h"
#include "entity.h"


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
	ed->toolbar_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point){5, 5}, "Toolbar");
	ed->toolbar_gui.minimum_size = (t_point){165, 20};
	ed->toolbar_gui.locked = true;
	player_init(&ed->player, sdl, &ed->world);
	ed->player.gun->disabled = true;
	ed->world.player = &ed->player;
}

typedef struct s_editorprefs
{
	char		worldname[32];
	t_vector3	playerpos;
}	t_editorprefs;

typedef struct s_graphicprefs
{
	uint32_t	resolution_x;
	uint32_t	resolution_y;
	float		resolutionscale;
}	t_graphicprefs;

void	editor_load_prefs(t_editor *ed, t_sdlcontext *sdl)
{
	char			*prefpath;
	char			*pref_filename;
	t_editorprefs	prefs;
	int				fd;
	prefpath = SDL_GetPrefPath("temp", "stark");
	pref_filename = ft_strnew(256);
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "editor.pref");
	fd = open(pref_filename, O_RDONLY);
	if (fd != -1)
	{
		read(fd, &prefs, sizeof(t_editorprefs));
		ed->player.transform.position = prefs.playerpos;
		editor_load_and_init_world(ed, prefs.worldname, sdl);
		printf("loading prefs from '%s' , worldname %s \n", prefpath, prefs.worldname);
	}
	else
		editor_load_and_init_world(ed, "worlds/test123.world", sdl);
	SDL_free(prefpath);
	free(pref_filename);
}

void	editor_save_prefs(t_editor *ed)
{
	char			*prefpath;
	char			*pref_filename;
	t_editorprefs	prefs;
	int				fd;
	prefpath = SDL_GetPrefPath("temp", "stark");
	
	pref_filename = ft_strnew(256);
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "editor.pref");
	fd = open(pref_filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd != -1)
	{
		ft_strcpy(prefs.worldname, ed->world.name);
		prefs.playerpos = ed->player.transform.position;
		write(fd, &prefs, sizeof(prefs));
		//editor_load_and_init_world(ed, prefs.worldname, sdl);
		close(fd);
		printf("saved prefs to '%s', worldname was %s \n", pref_filename, prefs.worldname);
	}
	SDL_free(prefpath);
	free(pref_filename);
}

int	editorloop(t_sdlcontext sdl)
{
	t_editor	ed;
	bool		audio = 0;

	bzero(&ed, sizeof(t_editor));
	editor_load_prefs(&ed, &sdl);
	//
	//ed.player.transform.position = (t_vector3){100, 100, 100.0f};
	sdl.lighting_toggled = true;
	ed.gamereturn = game_continue;
	sdl.render = init_render(sdl, &ed.world);
	
	ed.tool = NULL;
	sdl.lighting_toggled = false;
	
	sdl.bitmask.tile = malloc(sizeof(t_tile) * ((sdl.window_h * sdl.window_w) / 64));
	sdl.bitmask.bitmask_chunks.x = sdl.window_w / 16;
	sdl.bitmask.bitmask_chunks.y = sdl.window_h / 8;
	sdl.bitmask.tile_chunks.x = sdl.window_w / 8;
	sdl.bitmask.tile_chunks.y = sdl.window_h / 8;
	//spawn_basic_entity()

	//ed.world.sdl->render = 
	while (ed.gamereturn == game_continue)
	{
		update_deltatime(&ed.world.clock);
		bzero((uint32_t *)sdl.surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
		bzero((uint32_t *)sdl.ui_surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
		ed.gamereturn = editor_events(&ed);
		bake_lights(&sdl.render, &ed.world);
		if (!ed.player.locked)
			moveplayer(&ed.player, &ed.hid.input, ed.world.clock);
		update_render(&sdl.render, &ed.player);
		screen_blank(sdl);
		
		render_start(&sdl.render);
		update_world3d(&ed.world, &sdl.render);
		update_editor_toolbar(&ed, &ed.toolbar_gui);
		if (ed.tool != NULL)
		{
			ed.tool->update(&ed, &sdl);
		}
		ed.hid.mouse.click_unhandled = false;
		//print_text_boxed(&sdl, "tab to unlock/lock mouse, shift + enter to go to playmode", (t_point){sdl.window_w / 2, 10}, (t_point){sdl.window_w, sdl.window_h});
		char *fps = ft_itoa(ed.world.clock.fps);
		print_text(&sdl, fps, (t_point){sdl.window_w - 80, 10});
		drawcircle(sdl, point_div(sdl.screensize, 2), 4, CLR_BLUE);
		free(fps);
		if (!ed.player.gun->disabled)
			render_entity(&sdl, &sdl.render, &ed.player.gun->entity);
		update_debugconsole(&ed.world.debugconsole, &sdl, ed.world.clock.delta);
		
		//rescale_surface(&sdl);
		//join_surfaces(sdl.window_surface, sdl.surface);
		memcpy(sdl.window_surface->pixels, sdl.surface->pixels, sizeof(uint32_t) * sdl.window_w * sdl.window_h);
		//join_surfaces(sdl.window_surface, sdl.ui_surface);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);

		if (audio == 0)
		{
			t_vector3	nf = ed.player.lookdir;

			nf = (t_vector3){-nf.x, -nf.y, 0.0f};
			nf = vector3_normalise(nf);
			FMOD_System_Set3DListenerAttributes(sdl.audio.system, 0, &ed.player.transform.position, &((t_vector3){0}), &nf, &((t_vector3){.z = 1.0f}));
			//play_worldsound(&sdl.audio, "bubbles.wav", &ed.world.entitycache.entities[0].transform.position);
			FMOD_System_Update(sdl.audio.system);
		}
	}
	editor_save_prefs(&ed);
	save_world(ed.world.name, ed.world);
	free_render(sdl.render);
	if (ed.gamereturn == game_exit)
		quit_game(&sdl);
	return (ed.gamereturn);
}