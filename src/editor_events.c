/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/26 17:32:53 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"

void	editor_toggle_keystates(t_editor *ed, SDL_Event e)
{
	if (e.type == SDL_KEYDOWN)
	{
		ed->keystate |= keyismoveleft(e) << KEYS_LEFTMASK;
		ed->keystate |= keyismoveright(e) << KEYS_RIGHTMASK;
		ed->keystate |= keyismoveup(e) << KEYS_UPMASK;
		ed->keystate |= keyismovedown(e) << KEYS_DOWNMASK;
	}
	if (e.type == SDL_KEYUP)
	{
		ed->keystate &= ~(keyismoveleft(e)) << KEYS_LEFTMASK;
		ed->keystate &= ~(keyismoveright(e) << KEYS_RIGHTMASK);
		ed->keystate &= ~(keyismoveup(e) << KEYS_UPMASK);
		ed->keystate &= ~(keyismovedown(e) << KEYS_DOWNMASK);
	}
}

void	move_editor_offset(t_editor *ed)
{
	if ((ed->keystate >> KEYS_UPMASK) & 1) 
		ed->offset.y += EDITOR_MOVESPEED * ed->clock.delta;
	if ((ed->keystate >> KEYS_DOWNMASK) & 1)
		ed->offset.y -= EDITOR_MOVESPEED * ed->clock.delta;
	if ((ed->keystate >> KEYS_LEFTMASK) & 1)
		ed->offset.x += EDITOR_MOVESPEED * ed->clock.delta;
	if ((ed->keystate >> KEYS_RIGHTMASK) & 1)
		ed->offset.x -= EDITOR_MOVESPEED * ed->clock.delta;
}

int		editor_events(t_editor *ed)
{
	static SDL_Event	e;
	
	ed->mouse.scroll_delta = 0; //Needs to be reseted outside of eventloop
	while (SDL_PollEvent(&e))
	{
		mouse_event(e, ed);
		editor_toggle_keystates(ed, e);
		if (e.type == SDL_KEYDOWN)
		{
			if (iskey(e, SDLK_ESCAPE))
				return (game_exit);
			else if (iskey(e, SDLK_TAB))
				return(game_switchmode);
		}
	}
	move_editor_offset(ed);
	return (game_continue);
}
