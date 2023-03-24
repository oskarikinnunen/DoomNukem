/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sdl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 20:45:55 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	create_sdl_window(t_sdlcontext *sdl, t_screenmode mode)
{
	SDL_DisplayMode	dmode;

	SDL_GetCurrentDisplayMode(0, &dmode);
	if (dmode.w != sdl->window_w || dmode.h != sdl->window_h)
		mode = screenmode_windowed;
	sdl->window_w = ft_clamp(sdl->window_w, 0, dmode.w);
	sdl->window_h = ft_clamp(sdl->window_h, 0, dmode.h);
	sdl->screensize = (t_point){sdl->window_w, sdl->window_h};
	if (mode != screenmode_fullscreen)
	{
		sdl->window = SDL_CreateWindow("DoomNukem", \
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, \
			sdl->window_w, sdl->window_h, SDL_WINDOW_SHOWN);
	}
	if (mode == screenmode_fullscreen)
	{
		sdl->window = SDL_CreateWindow("DoomNukem", 0, 0, \
			sdl->window_w, sdl->window_h, SDL_WINDOW_SHOWN | \
										SDL_WINDOW_FULLSCREEN);
	}
	if (sdl->window == NULL)
		doomlog(LOG_EC_SDL_CREATEWINDOW, NULL);
	sdl->window_surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->window_surface == NULL)
		doomlog(LOG_EC_SDL_GETWINDOW_SURFACE, NULL);
}
