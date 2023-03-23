/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/23 19:59:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

void	force_mouseunlock(t_hid_info *hid)
{
	hid->mouse.relative = false;
	SDL_SetRelativeMouseMode(hid->mouse.relative);
	hid->mouse.delta = point_zero();
}

void	force_mouselock(t_hid_info *hid)
{
	hid->mouse.relative = true;
	SDL_SetRelativeMouseMode(hid->mouse.relative);
	hid->mouse.delta = point_zero();
	hid->mouse.pos = point_zero();
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
	if (iskey(e, SDLK_F3))
		ed->graphics_gui.hidden = !ed->graphics_gui.hidden;
	if (iskey(e, SDLK_F5))
	{
		world_save_to_file(ed->world);
		ed->world.lastsavetime = ed->world.clock.time;
	}
}

t_gamereturn	editor_events(t_editor *ed)
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
					exit(0);
				return (game_exit);
			}
			editor_key_down(ed, e);
		}
	}
	update_input(&ed->hid.input, ed->hid);
	return (game_continue);
}
