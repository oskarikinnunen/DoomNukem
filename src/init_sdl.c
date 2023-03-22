/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sdl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/02/07 15:54:56 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	init_sdl_error_window2(t_sdlcontext *sdl)
{
	sdl->window_surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->window_surface == NULL)
	{
		ft_putstr_fd("can't create a surface for error window - ", 2);
		ft_putendl_fd(SDL_GetError(), 2);
		exit (1);
	}
	sdl->surface = SDL_CreateRGBSurfaceWithFormat(\
			SDL_SWSURFACE, 600, 300, 32, SDL_PIXELFORMAT_ARGB8888);
	if (sdl->surface == NULL)
	{
		ft_putstr_fd("can't create a RGB surface for error window - ", 2);
		ft_putendl_fd(SDL_GetError(), 2);
		exit (1);
	}
}

void	init_sdl_error_window(t_sdlcontext *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0 \
			|| SDL_Init(SDL_INIT_EVENTS) < 0 \
			|| SDL_Init(SDL_INIT_GAMECONTROLLER) < 0 \
			|| TTF_Init() < 0)
	{
		ft_putstr_fd("can't init sdl for error message - ", 2);
		ft_putendl_fd(SDL_GetError(), 2);
		exit (1);
	}
	// TODO: does this need to be protected for tiny window sizes?
	sdl->window = SDL_CreateWindow("Error",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			600, 300, SDL_WINDOW_SHOWN);
	if (sdl->window == NULL)
	{
		ft_putstr_fd("can't create a window for error message", 2);
		ft_putendl_fd(SDL_GetError(), 2);
		exit (1);
	}
	init_sdl_error_window2(sdl);
}

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
