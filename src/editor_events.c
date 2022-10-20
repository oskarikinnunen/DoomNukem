/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 14:16:17 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	move_editor_offset(t_editor *ed, SDL_Event e)
{
	ed->offset.x -= iskey(e, SDLK_d) * ed->clock.delta * 3.0f;
	ed->offset.x += iskey(e, SDLK_a) * ed->clock.delta * 3.0f;
	ed->offset.y -= iskey(e, SDLK_s) * ed->clock.delta * 3.0f;
	ed->offset.y += iskey(e, SDLK_w) * ed->clock.delta * 3.0f;
}

int		editor_events(t_editor *ed)
{
	static SDL_Event	e;
	
	ed->mouse.scroll_delta = 0; //Needs to be reseted outside of eventloop
	while (SDL_PollEvent(&e))
	{
		if (ed->transition.active)
			break ;
		mouse_event(e, ed);
		if (e.type == SDL_KEYDOWN)
		{
			move_editor_offset(ed, e);
			if (iskey(e, SDLK_ESCAPE))
				return (game_exit);
			else if (iskey(e, SDLK_TAB))
				return(game_switchmode);
		}
	}
	return (game_continue);
}
