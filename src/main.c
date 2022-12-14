/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/12 11:23:34 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "png.h"
#include "game_lua.h"
#include "objects.h"
#include "file_io.h"
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void	free_sdl_stuff(t_sdlcontext *sdl)
{
	if (sdl->zbuffer != NULL)
		free(sdl->zbuffer);
	if (sdl->surface != NULL)
		SDL_FreeSurface(sdl->surface);
	if (sdl->ui_surface != NULL)
		SDL_FreeSurface(sdl->ui_surface);
	if (sdl->window_surface != NULL)
		SDL_FreeSurface(sdl->window_surface);
	if (sdl->window != NULL)
		SDL_DestroyWindow(sdl->window);
	free_render(sdl->render);
	if (sdl->bitmask.tile != NULL)
		free(sdl->bitmask.tile);
}

void	alloc_occlusion(t_sdlcontext *sdl)
{
	sdl->bitmask.tile = ft_memalloc(sizeof(t_tile) * ((sdl->window_h * sdl->window_w) / 64)); //TODO: free
	sdl->bitmask.bitmask_chunks.x = sdl->window_w / 16;
	sdl->bitmask.bitmask_chunks.y = sdl->window_h / 8;
	sdl->bitmask.tile_chunks.x = sdl->window_w / 8;
	sdl->bitmask.tile_chunks.y = sdl->window_h / 8;
}

void	create_sdl_window(t_sdlcontext *sdl, t_screenmode mode)
{
	SDL_DisplayMode DMode;

	SDL_GetCurrentDisplayMode(0, &DMode);
	if (DMode.w != sdl->window_w || DMode.h != sdl->window_h)
		mode = screenmode_windowed;
	sdl->window_w =	ft_clamp(sdl->window_w, 0, DMode.w);
	sdl->window_h =	ft_clamp(sdl->window_h, 0, DMode.h);
	sdl->screensize = (t_point) {sdl->window_w, sdl->window_h};
	if (mode != screenmode_fullscreen)
	{
		sdl->window = SDL_CreateWindow("DoomNukem",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			sdl->window_w, sdl->window_h, SDL_WINDOW_SHOWN);
	}
	if (mode == screenmode_fullscreen)
	{
		sdl->window = SDL_CreateWindow("DoomNukem",
			0, 0,
			sdl->window_w, sdl->window_h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	}
	if (sdl->window == NULL)
		error_log(EC_SDL_CREATEWINDOW);
	sdl->window_surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->window_surface == NULL)
		error_log(EC_SDL_GETWINDOW_SURFACE);
}

void	set_sdl_settings(t_sdlcontext *sdl)
{
	t_graphicprefs	prefs;

	free_sdl_stuff(sdl);
	ft_bzero(sdl, sizeof(sdl));
	prefs = load_graphicsprefs(); //TODO: load before this and just pass prefs set_sdl_settings
	sdl->window_w = prefs.resolution_x;
	sdl->window_h = prefs.resolution_y;
	sdl->resolution_scaling = ft_clampf(prefs.resolutionscale, 0.25f, 1.0f);
	create_sdl_window(sdl, prefs.screenmode);
	sdl->surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, sdl->window_w, sdl->window_h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (sdl->surface == NULL)
		error_log(EC_SDL_CREATERGBSURFACE);
	sdl->ui_surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, sdl->window_w, sdl->window_h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (sdl->ui_surface == NULL)
		error_log(EC_SDL_CREATERGBSURFACE);
	sdl->zbuffer = ft_memalloc(sdl->window_w * sdl->window_h * sizeof(float));
	alloc_occlusion(sdl);
	sdl->render = init_render(*sdl);
}

void	quit_game(t_sdlcontext *sdl)
{
	//close_audio(&sdl->audio);
	SDL_Quit();
	exit(0);
}

void	create_sdlcontext(t_sdlcontext	*sdl)
{
	ft_bzero(sdl, sizeof(t_sdlcontext));
	if (SDL_Init(SDL_INIT_VIDEO) < 0 \
		|| SDL_Init(SDL_INIT_EVENTS) < 0 \
		|| SDL_Init(SDL_INIT_GAMECONTROLLER) < 0 \
		|| TTF_Init() < 0)
		error_log(EC_SDL_INIT);
	set_sdl_settings(sdl);
	load_assets(sdl);
}

void	checkargs(int argc, char **argv)
{
	if (argc == 2 && ft_strcmp(argv[1], "-gfxreset") == 0)
		reset_graphics_prefs();
	if (argc == 2 && ft_strcmp(argv[1], "-mapreset") == 0)
	{
		
	}
}

int	main(int argc, char **argv)
{
	t_sdlcontext	sdl;
	t_gamereturn	gr;

	checkargs(argc, argv);
	create_sdlcontext(&sdl);
	gr = game_switchmode;
	while (gr == game_switchmode)
	{
		gr = editorloop(sdl); // quit & exit is handled inside the loop
		gr = playmode(sdl); // quit & exit is handled inside the loop
	}
	//shouldn't get here?
	return (0);
}

