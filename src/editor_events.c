/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/22 12:11:23 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
 
#include "editor_tools.h"

bool		check_alpha_key(uint32_t alphakeystate, char c)
{
	c = ft_tolower(c);
	if (!ft_isalpha(c))
		return (false); //And log something?
	return ((alphakeystate >> (c - 'a')) & 1);
}

void		toggle_keystates(t_hid_info *hid, SDL_Event e)
{
	char	c;
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
		hid->keystate |= iskey(e, SDLK_DELETE) << KEYS_DELETEMASK;
		hid->keystate |= iskey(e, SDLK_1) << KEYS_1MASK;
		hid->keystate |= iskey(e, SDLK_2) << KEYS_2MASK;
		hid->keystate |= iskey(e, SDLK_3) << KEYS_3MASK;
		hid->keystate |= iskey(e, SDLK_4) << KEYS_4MASK;
		//hid->keystate |= iskey(e, SDLK_v) << KEYS_VMASK;
		c = 'a';
		while (c <= 'z')
		{
			hid->alphakeystate |= iskey(e, c) << (c - 'a');
			hid->alphakey_pressed |= iskey(e, c) << (c - 'a');
			c++; //Not the language
		}
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
		hid->keystate &= ~(iskey(e, SDLK_DELETE) << KEYS_DELETEMASK);
		hid->keystate &= ~(iskey(e, SDLK_1) << KEYS_1MASK);
		hid->keystate &= ~(iskey(e, SDLK_2) << KEYS_2MASK);
		hid->keystate &= ~(iskey(e, SDLK_3) << KEYS_3MASK);
		hid->keystate &= ~(iskey(e, SDLK_4) << KEYS_4MASK);
		//hid->keystate &= ~(iskey(e, SDLK_v) << KEYS_VMASK);
		c = 'a';
		while (c <= 'z')
		{
			hid->alphakeystate &= ~(iskey(e, c) << (c - 'a'));
			c++; //Not the language
		}
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
	ed->hid.alphakey_pressed = 0; //Needs to be reset
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
			t_pointlight t;
			/*if (iskey(e, SDLK_p))//TODO: temp shortcut for baking lighting
				bake_lighting_shadows(&ed->render, &ed->world);*/
			if (iskey(e, SDLK_ESCAPE))
			{
				if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
					exit(0);
				return (game_exit);
			}
				
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
			if (iskey(e, SDLK_F2))
				ed->world.debug_gui->hidden = !ed->world.debug_gui->hidden;
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
