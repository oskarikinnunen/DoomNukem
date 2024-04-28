/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sdl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 12:23:26 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"

void	create_sdl_window(t_sdlcontext *sdl, t_screen_mode mode)
{
	SDL_DisplayMode	dmode;

	SDL_GetCurrentDisplayMode(0, &dmode);
	if (dmode.w != sdl->window_w || dmode.h != sdl->window_h)
		mode = screen_mode_windowed;
	sdl->window_w = ft_clamp(sdl->window_w, 0, dmode.w);
	sdl->window_h = ft_clamp(sdl->window_h, 0, dmode.h);
	sdl->screensize = (t_point){sdl->window_w, sdl->window_h};
	if (mode != screen_mode_fullscreen)
	{
		sdl->window = SDL_CreateWindow("DoomNukem", \
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, \
			sdl->window_w, sdl->window_h, SDL_WINDOW_SHOWN);
	}
	if (mode == screen_mode_fullscreen)
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

void	free_sdl_stuff(t_sdlcontext *sdl)
{
	if (sdl->zbuffer != NULL)
		free(sdl->zbuffer);
	if (sdl->surface != NULL)
		SDL_FreeSurface(sdl->surface);
	if (sdl->window_surface != NULL)
		SDL_FreeSurface(sdl->window_surface);
	if (sdl->window != NULL)
		SDL_DestroyWindow(sdl->window);
	free_render(sdl->render);
	if (sdl->bitmask.tile != NULL)
		free(sdl->bitmask.tile);
	if (sdl->scaling_buffer != NULL)
		free(sdl->scaling_buffer);
}

//TODO: free bitmask.tile
void	alloc_occlusion(t_sdlcontext *sdl)
{
	sdl->bitmask.tile = prot_memalloc(sizeof(t_tile) * \
			((sdl->window_h * sdl->window_w) / 64));
	sdl->bitmask.bitmask_chunks.x = sdl->window_w / 16;
	sdl->bitmask.bitmask_chunks.y = sdl->window_h / 8;
	sdl->bitmask.tile_chunks.x = sdl->window_w / 8;
	sdl->bitmask.tile_chunks.y = sdl->window_h / 8;
}

void	set_sdl_settings(t_sdlcontext *sdl)
{
	t_graphic_prefs	prefs;

	free_sdl_stuff(sdl);
	prefs.screenmode = screen_mode_windowed;
	prefs.resolution_x = DEFAULT_RES_X;
	prefs.resolution_y = DEFAULT_RES_Y;
	prefs.resolutionscale = DEFAULT_RES_SCALING;
	prefs.volume = 5.0f;
	sdl->window_w = prefs.resolution_x;
	sdl->window_h = prefs.resolution_y;
	sdl->resolution_scaling = ft_clampf(prefs.resolutionscale, 0.25f, 1.0f);
	sdl->audio.sfx_volume = prefs.volume;
	create_sdl_window(sdl, prefs.screenmode);
	sdl->surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, \
				sdl->window_w, sdl->window_h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (sdl->surface == NULL)
		doomlog(LOG_EC_SDL_CREATERGBSURFACE, NULL);
	sdl->zbuffer = prot_memalloc(sdl->window_w * sdl->window_h * sizeof(float));
	sdl->scaling_buffer = prot_memalloc(sdl->window_w * sdl->window_w * \
											sizeof(uint32_t));
	alloc_occlusion(sdl);
	sdl->render = init_render(*sdl);
}

void	create_sdlcontext(t_app_argument app_argument, t_sdlcontext	*sdl)
{
	ft_bzero(sdl, sizeof(t_sdlcontext));
	if (SDL_Init(SDL_INIT_VIDEO) < 0 \
		|| SDL_Init(SDL_INIT_EVENTS) < 0 \
		|| SDL_Init(SDL_INIT_GAMECONTROLLER) < 0 \
		|| TTF_Init() < 0)
		doomlog(LOG_EC_SDL_INIT, NULL);
	set_sdl_settings(sdl);
	if (app_argument.app_mode == APPMODE_EDIT)
		editor_load_assets(sdl);
	else if (app_argument.app_mode == APPMODE_PLAY)
		playmode_load_assets(app_argument.level_name, sdl);
}
