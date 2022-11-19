/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/08 05:41:17 by okinnune         ###   ########.fr       */
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
	}
}

void		move_editor_offset(t_editor *ed)
{
	/*float	speed = EDITOR_MOVESPEED * ed->clock.delta;
	if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		speed *= 2.5f;
	if ((ed->keystate >> KEYS_CTRLMASK) & 1)
		speed *= 0.45f;
	if ((ed->keystate >> KEYS_DOWNMASK) & 1)
		ed->position.y += speed;
	if ((ed->keystate >> KEYS_UPMASK) & 1)
		ed->position.y -= speed;
	if ((ed->keystate >> KEYS_LEFTMASK) & 1)
		ed->position.x += speed;
	if ((ed->keystate >> KEYS_RIGHTMASK) & 1)
		ed->position.x -= speed;
	if (((ed->keystate >> KEYS_SHIFTMASK) & 1) == 0)
		ed->position.z += ed->mouse.scroll_delta * 30.0f;
	ed->position.x = ft_clampf(ed->position.x, 0.0f, 1000.0f);
	ed->position.y = ft_clampf(ed->position.y, 0.0f, 1000.0f);*/
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
		mouse_event(e, ed);
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
	}
	//move_editor_offset(ed);
	return (game_continue);
}
