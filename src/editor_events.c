/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/15 13:17:31 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int		editor_events(t_editor *ed)
{
	static SDL_Event	e;
	static int			check_for_free;
	
	ed->mouse.scroll_delta = 0; //Needs to be reseted outside of eventloop
	while (SDL_PollEvent(&e))
	{
		if (ed->transition.active)
			break ;
		mouse_event(e, ed);
		if (e.type == SDL_KEYDOWN) {
			if (iskey(e, SDLK_ESCAPE))
				return (game_exit);
			else if (iskey(e, SDLK_TAB))
				return(game_switchmode);
			else if(iskey(e, SDLK_SPACE))
			{
				/*if (ed->state != display3d)
				{
					if (check_for_free == 1)
					{
						free_fdf(&ed->grid_fdf);
						free_fdf(&ed->walls_fdf);
						free_fdf_obj(&ed->grid_fdf.obj);
						free_fdf_obj(&ed->walls_fdf.obj);
					}
					start_anim(&ed->transition, anim_forwards);
					ed->state = display3d; 
				}
				else
				{
					start_anim(&ed->transition, anim_backwards);
					ed->state = place_start;
				}*/
			}
		}
	}
	return (game_continue);
}
