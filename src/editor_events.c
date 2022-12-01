/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/01 14:26:17 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
 
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

t_gamereturn	editor_events(t_editor *ed)
{
	static SDL_Event	e;
	
	ed->hid.mouse.scroll_delta = 0; //Needs to be reset
	if (ed->hid.mouse.relative)
	{
		SDL_GetRelativeMouseState(&ed->hid.mouse.delta.x, &ed->hid.mouse.delta.y);
		if (ed->hid.mouse.safe_delta && ((ed->hid.mouse.delta.x) > 30 || ft_abs(ed->hid.mouse.delta.y) > 30))
			ed->hid.mouse.delta = point_zero();
	}
		
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
				ed->player.locked = !ed->hid.mouse.relative;
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
	updateinput(&ed->hid.input, ed->hid.keystate, ed->hid.mouse, ed->hid.controller);
	return (game_continue);
}
