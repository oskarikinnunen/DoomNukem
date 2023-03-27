/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 19:21:39 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "render.h"
#include "objects.h"
#include "entity.h"
#include "navigation.h"

static void	editorloop(t_sdlcontext *sdl, t_editor *ed)
{
	while (1)
	{
		update_clock(&ed->world.clock);
		editor_events(ed);
		if (!ed->player.locked)
			moveplayer(&ed->player, &ed->hid.input, &ed->world);
		update_world3d(&ed->world, &sdl->render);
		update_editor_toolbar(ed, &ed->toolbar_gui);
		if (ed->tool != NULL)
			ed->tool->update(ed, sdl);
		draw_crosshair(sdl);
		draw_fps_counter(sdl, ed->world.clock.fps);
		ed->hid.mouse.click_unhandled = false;
		update_window_surface(sdl);
		update_audio(&ed->world);
	}
}

void	editor(t_app_argument app_argument, t_sdlcontext sdl)
{
	t_editor	ed;

	ft_bzero(&ed, sizeof(t_editor));
	editor_loading_screen("LOADING WORLD", &sdl);
	ed.world = load_world(app_argument, &sdl);
	ed.world.sdl->render.world = &ed.world;
	editor_loading_screen("SETTING UP WORLD", &sdl);
	set_up_editor(&sdl, &ed);
	if (SDL_SetRelativeMouseMode(ed.hid.mouse.relative) < 0)
		doomlog(LOG_EC_SDL_SETRELATIVEMOUSEMODE, NULL);
	editorloop(&sdl, &ed);
}
