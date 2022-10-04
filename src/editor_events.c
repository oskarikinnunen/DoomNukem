/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 07:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 16:39:21 by okinnune         ###   ########.fr       */
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
	
	while (SDL_PollEvent(&e))
	{
		mouse_event(e, ed);
		if (e.type == SDL_KEYDOWN) {
			if (iskey(e, SDLK_ESCAPE))
				return (1);
			else if(iskey(e, SDLK_SPACE))
			{
				if (ed->state != display3d)
				{
					gridto_obj(&ed->grid_fdf.obj); //TODO: LEAKS!! free this pls
					fdf_init(&ed->grid_fdf);
					ft_bzero(&ed->walls_fdf, sizeof(t_fdf));
					lines_to_obj(&ed->walls_fdf.obj, ed);
					fdf_init(&ed->walls_fdf);
					ed->state = display3d;
				}
					
				else
					ed->state = place_start;
			}
		}
			
	}
	return (0);
}
