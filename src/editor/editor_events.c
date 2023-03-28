/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfum <kfum@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 10:41:18 by kfum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

void	editor_save_quit(t_editor *ed)
{
	world_save_to_file(ed->world);
	SDL_Quit();
	exit (0);
}

void	editor_quit(void)
{
	SDL_Quit();
	exit (0);
}

static void	editor_key_down(t_editor *ed, SDL_Event e)
{
	if (iskey(e, SDLK_TAB))
	{
		ed->hid.mouse.relative = !ed->hid.mouse.relative;
		SDL_SetRelativeMouseMode(ed->hid.mouse.relative);
		ed->player.locked = !ed->hid.mouse.relative;
		ed->hid.mouse.delta = point_zero();
	}
	if (iskey(e, SDLK_F1))
		ed->toolbar_gui.hidden = !ed->toolbar_gui.hidden;
	if (iskey(e, SDLK_F2))
		ed->world.debug_gui->hidden = !ed->world.debug_gui->hidden;
	if (iskey(e, SDLK_F5))
	{
		world_save_to_file(ed->world);
		ed->world.last_save_time = ed->world.clock.time;
	}
}

void	editor_events(t_editor *ed)
{
	static SDL_Event	e;

	ed->hid.mouse.scroll_delta = 0;
	ed->hid.alphakey_pressed = 0;
	SDL_GetRelativeMouseState(&ed->hid.mouse.delta.x, &ed->hid.mouse.delta.y);
	if (!ed->hid.mouse.dragging_ui && ed->player.locked)
		SDL_GetMouseState(&ed->hid.mouse.pos.x, &ed->hid.mouse.pos.y);
	while (SDL_PollEvent(&e))
	{
		toggle_keystates(&ed->hid, e);
		mouse_event(e, &ed->hid.mouse);
		if (e.type == SDL_KEYDOWN)
		{
			if (iskey(e, SDLK_ESCAPE))
			{
				if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
					editor_quit();
				editor_save_quit(ed);
			}
			editor_key_down(ed, e);
		}
		if (e.type == SDL_QUIT)
			editor_save_quit(ed);
	}
	update_input(&ed->hid.input, ed->hid);
}
