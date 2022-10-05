/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/05 13:12:22 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static bool iskey(SDL_Event e, int keycode)
{
	return (e.key.keysym.sym == keycode);
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
		if (e.type == SDL_KEYDOWN) {
			if (iskey(e, SDLK_ESCAPE))
				return (1);
			else if(iskey(e, SDLK_SPACE))
			{
				if (ed->state != display3d)
				{
					start_anim(&ed->transition, anim_forwards);
					gridto_obj(&ed->grid_fdf.obj); //TODO: LEAKS!! doesn't free the previous stuff
					fdf_init(&ed->grid_fdf);
					lines_to_obj(&ed->walls_fdf.obj, ed);
					fdf_init(&ed->walls_fdf);
					ed->state = display3d;
				}
				else
				{
					start_anim(&ed->transition, anim_backwards);
					ed->state = place_start;
				}
			}
		}
	}
	return (0);
}
