/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/24 14:49:08 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "editor_tools.h"

void		editor_toggle_keystates(t_editor *ed, SDL_Event e)
{
	if (e.type == SDL_KEYDOWN)
	{
		ed->keystate |= keyismoveleft(e) << KEYS_LEFTMASK;
		ed->keystate |= keyismoveright(e) << KEYS_RIGHTMASK;
		ed->keystate |= keyismoveup(e) << KEYS_UPMASK;
		ed->keystate |= keyismovedown(e) << KEYS_DOWNMASK;
		ed->keystate |= iskey(e, SDLK_LCTRL) << KEYS_CTRLMASK;
		ed->keystate |= iskey(e, SDLK_SPACE) << KEYS_SPACEMASK;
		ed->keystate |= iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK;
		ed->keystate |= iskey(e, SDLK_LALT) << KEYS_LALTMASK;
	}
	if (e.type == SDL_KEYUP)
	{
		ed->keystate &= ~(keyismoveleft(e)) << KEYS_LEFTMASK;
		ed->keystate &= ~(keyismoveright(e) << KEYS_RIGHTMASK);
		ed->keystate &= ~(keyismoveup(e) << KEYS_UPMASK);
		ed->keystate &= ~(keyismovedown(e) << KEYS_DOWNMASK);
		ed->keystate &= ~(iskey(e, SDLK_LCTRL) << KEYS_CTRLMASK);
		ed->keystate &= ~(iskey(e, SDLK_SPACE) << KEYS_SPACEMASK);
		ed->keystate &= ~(iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK);
		ed->keystate &= ~(iskey(e, SDLK_LALT) << KEYS_LALTMASK);
	}
}

void	force_mouseunlock(t_editor *ed)
{
	ed->mouse.relative = false;
	SDL_SetRelativeMouseMode(ed->mouse.relative);
	ed->mouse.delta = point_zero();
}

void	force_mouselock(t_editor *ed)
{
	ed->mouse.relative = true;
	SDL_SetRelativeMouseMode(ed->mouse.relative);
	ed->mouse.delta = point_zero();
	ed->mouse.pos = point_zero();
}

static void	buttons_click(t_editor *ed)
{
	t_list		*l;
	t_guibutton	button;

	l = ed->buttonlist;

	while (l != NULL)
	{
		button = *(t_guibutton *)l->content;
		if (pointrectanglecollision(ed->mouse.pos, button.rect) && mouse_clicked(ed->mouse, MOUSE_LEFT))
		{
			if (button.onclick != NULL)
			{
				button.onclick(ed);
				ed->mouse.click_unhandled = false;
			}
		}
		l = l->next;
	}
}


t_gamereturn	editor_events(t_editor *ed)
{
	static SDL_Event	e;
	
	ed->mouse.scroll_delta = 0; //Needs to be reseted outside of eventloop
	/*if (!point_cmp(ed->mouse.delta, point_zero()))
		printf("%i %i \n", ed->mouse.delta.x, ed->mouse.delta.y);*/
	if (ed->mouse.relative)
		SDL_GetRelativeMouseState(&ed->mouse.delta.x, &ed->mouse.delta.y);
	while (SDL_PollEvent(&e))
	{
		editor_toggle_keystates(ed, e);
		mouse_event(e, &ed->mouse);
		if (e.type == SDL_KEYDOWN)
		{
			if (iskey(e, SDLK_ESCAPE))
				return (game_exit);
			if (iskey(e, SDLK_TAB))
			{
				ed->mouse.relative = !ed->mouse.relative;
				SDL_SetRelativeMouseMode(ed->mouse.relative);
				ed->mouse.delta = point_zero();
			}
			else if (((ed->keystate >> KEYS_SHIFTMASK) & 1)
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
	//move_editor_offset(ed);
	return (game_continue);
}
