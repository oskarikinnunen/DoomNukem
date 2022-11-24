/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/24 17:19:37 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "editor_tools.h"

void		toggle_keystates(t_hid_info *hid, SDL_Event e)
{
	if (e.type == SDL_KEYDOWN)
	{
		hid->keystate |= keyismoveleft(e) << KEYS_LEFTMASK;
		hid->keystate |= keyismoveright(e) << KEYS_RIGHTMASK;
		hid->keystate |= keyismoveup(e) << KEYS_UPMASK;
		hid->keystate |= keyismovedown(e) << KEYS_DOWNMASK;
		hid->keystate |= iskey(e, SDLK_LCTRL) << KEYS_CTRLMASK;
		hid->keystate |= iskey(e, SDLK_SPACE) << KEYS_SPACEMASK;
		hid->keystate |= iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK;
		hid->keystate |= iskey(e, SDLK_LALT) << KEYS_LALTMASK;
	}
	if (e.type == SDL_KEYUP)
	{
		hid->keystate &= ~(keyismoveleft(e)) << KEYS_LEFTMASK;
		hid->keystate &= ~(keyismoveright(e) << KEYS_RIGHTMASK);
		hid->keystate &= ~(keyismoveup(e) << KEYS_UPMASK);
		hid->keystate &= ~(keyismovedown(e) << KEYS_DOWNMASK);
		hid->keystate &= ~(iskey(e, SDLK_LCTRL) << KEYS_CTRLMASK);
		hid->keystate &= ~(iskey(e, SDLK_SPACE) << KEYS_SPACEMASK);
		hid->keystate &= ~(iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK);
		hid->keystate &= ~(iskey(e, SDLK_LALT) << KEYS_LALTMASK);
	}
}

void	force_mouseunlock(t_editor *ed)
{
	ed->hid.mouse.relative = false;
	SDL_SetRelativeMouseMode(ed->hid.mouse.relative);
	ed->hid.mouse.delta = point_zero();
}

void	force_mouselock(t_editor *ed)
{
	ed->hid.mouse.relative = true;
	SDL_SetRelativeMouseMode(ed->hid.mouse.relative);
	ed->hid.mouse.delta = point_zero();
	ed->hid.mouse.pos = point_zero();
}

static void	buttons_click(t_editor *ed)
{
	t_list		*l;
	t_guibutton	button;

	l = ed->buttonlist;

	while (l != NULL)
	{
		button = *(t_guibutton *)l->content;
		if (pointrectanglecollision(ed->hid.mouse.pos, button.rect) && mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
		{
			if (button.onclick != NULL)
			{
				button.onclick(ed);
				ed->hid.mouse.click_unhandled = false;
			}
		}
		l = l->next;
	}
}


t_gamereturn	editor_events(t_editor *ed)
{
	static SDL_Event	e;
	
	ed->hid.mouse.scroll_delta = 0; //Needs to be reset
	if (ed->hid.mouse.relative)
		SDL_GetRelativeMouseState(&ed->hid.mouse.delta.x, &ed->hid.mouse.delta.y);
	while (SDL_PollEvent(&e))
	{
		toggle_keystates(&ed->hid, e);
		mouse_event(e, &ed->hid.mouse);
		if (e.type == SDL_KEYDOWN)
		{
			if (iskey(e, SDLK_ESCAPE))
				return (game_exit);
			if (iskey(e, SDLK_TAB))
			{
				ed->hid.mouse.relative = !ed->hid.mouse.relative;
				SDL_SetRelativeMouseMode(ed->hid.mouse.relative);
				ed->hid.mouse.delta = point_zero();
			}
			else if (((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
				&& iskey(e, SDLK_RETURN))
				return(game_switchmode);
		}
		if (e.type == SDL_CONTROLLERBUTTONDOWN)
		{
			if (e.cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
				return (game_switchmode);
			if (e.cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
				return (game_exit);
		}
	}
	buttons_click(ed);
	//updateinput(&hid->input, hid->keystate, hid->mouse, &hid->controller[0]);
	return (game_continue);
}
