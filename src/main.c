/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 19:09:18 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "assets.h"
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
	if (sdl->scaling_buffer != NULL)
		free(sdl->scaling_buffer);
}

void	quit_game(t_sdlcontext *sdl)
{
	SDL_Quit();
	exit(0);
}

void	alloc_occlusion(t_sdlcontext *sdl)
{
	sdl->bitmask.tile = prot_memalloc(sizeof(t_tile) * ((sdl->window_h * sdl->window_w) / 64)); //TODO: free
	sdl->bitmask.bitmask_chunks.x = sdl->window_w / 16;
	sdl->bitmask.bitmask_chunks.y = sdl->window_h / 8;
	sdl->bitmask.tile_chunks.x = sdl->window_w / 8;
	sdl->bitmask.tile_chunks.y = sdl->window_h / 8;
}

void	set_sdl_settings(t_sdlcontext *sdl)
{
	t_graphicprefs	prefs;

	free_sdl_stuff(sdl);
	ft_bzero(sdl, sizeof(sdl)); //WHY DOES THIS WORK... this resets asset pointers aswell??
	prefs = load_graphicsprefs(); //TODO: load before this and just pass prefs set_sdl_settings
	sdl->window_w = prefs.resolution_x;
	sdl->window_h = prefs.resolution_y;
	sdl->resolution_scaling = ft_clampf(prefs.resolutionscale, 0.25f, 1.0f);
	sdl->audio.sfx_volume = prefs.volume;
	create_sdl_window(sdl, prefs.screenmode);
	sdl->surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, sdl->window_w, sdl->window_h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (sdl->surface == NULL)
		doomlog(LOG_EC_SDL_CREATERGBSURFACE, NULL);
	sdl->ui_surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, sdl->window_w, sdl->window_h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (sdl->ui_surface == NULL)
		doomlog(LOG_EC_SDL_CREATERGBSURFACE, NULL);
	sdl->zbuffer = prot_memalloc(sdl->window_w * sdl->window_h * sizeof(float));
	sdl->scaling_buffer = prot_memalloc(sdl->window_w * sdl->window_w * sizeof(uint32_t));
	alloc_occlusion(sdl);
	sdl->render = init_render(*sdl);
}

void	create_sdlcontext(char *level, t_sdlcontext	*sdl, t_app_mode app_mode)
{
	ft_bzero(sdl, sizeof(t_sdlcontext));
	if (SDL_Init(SDL_INIT_VIDEO) < 0 \
		|| SDL_Init(SDL_INIT_EVENTS) < 0 \
		|| SDL_Init(SDL_INIT_GAMECONTROLLER) < 0 \
		|| TTF_Init() < 0)
		doomlog(LOG_EC_SDL_INIT, NULL);
	set_sdl_settings(sdl);
	if (app_mode == APPMODE_EDIT)
		editor_load_assets(sdl);
	else if (app_mode == APPMODE_PLAY)
		playmode_load_assets(level, sdl);
}

int	main(int argc, char **argv)
{
	t_sdlcontext	sdl;
	t_app_argument	app_argument;

	app_argument = get_app_argument(argc, argv);
	if (app_argument.app_mode == APPMODE_GFX_RESET)
	{
		reset_graphics_prefs();
		ft_putendl("graphics reset succesfully");
		return (0);
	}
	create_sdlcontext(app_argument.level_name, &sdl, app_argument.app_mode);
	if (app_argument.app_mode == APPMODE_EDIT)
		editorloop(app_argument.level_name, sdl);
	else if (app_argument.app_mode == APPMODE_PLAY)
		playmode(app_argument.level_name, sdl);
	else if (app_argument.app_mode == APPMODE_INVALID)
	{
		ft_putendl_fd("usage: ./DoomNukem [[-edit | -play] level_file] | -gfxreset", 2);
		return (1);
	}
	return (0);
}

/*
t_img *ptr;
	t_thread test;
	test.structs = malloc(sizeof(t_img) * THREAD);
	test.struct_size = sizeof(t_img);
	for (int i = 0; i < THREAD; i++)
	{
		ptr = &(((t_img *)test.structs)[i]);
		ptr->size.x = i;
	}
	test.func = temp_remove;
	test.init = init_remove;
	thread_set(&test);
	exit(0);
	*/
