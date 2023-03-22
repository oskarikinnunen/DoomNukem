/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 17:55:50 by okinnune         ###   ########.fr       */
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
	sdl->bitmask.tile = ft_memalloc(sizeof(t_tile) * ((sdl->window_h * sdl->window_w) / 64)); //TODO: free
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
	sdl->zbuffer = ft_memalloc(sdl->window_w * sdl->window_h * sizeof(float));
	sdl->scaling_buffer = ft_memalloc(sdl->window_w * sdl->window_w * sizeof(uint32_t));
	alloc_occlusion(sdl);
	sdl->render = init_render(*sdl);
}

void	create_sdlcontext(t_sdlcontext	*sdl, t_gamemode mode)
{
	ft_bzero(sdl, sizeof(t_sdlcontext));
	if (SDL_Init(SDL_INIT_VIDEO) < 0 \
		|| SDL_Init(SDL_INIT_EVENTS) < 0 \
		|| SDL_Init(SDL_INIT_GAMECONTROLLER) < 0 \
		|| TTF_Init() < 0)
		doomlog(LOG_EC_SDL_INIT, NULL);
	set_sdl_settings(sdl);
	if (mode == MODE_EDITOR)
		editor_load_assets(sdl);
	else
		playmode_load_assets(sdl);
}

void	checkargs(int argc, char **argv)
{
	if (argc == 2 && ft_strcmp(argv[1], "-gfxreset") == 0)
		reset_graphics_prefs();
	if (argc == 2 && ft_strcmp(argv[1], "-mapreset") == 0)
	{
		
	}
}

void	doomnukem(int argc, char **argv)
{
	t_sdlcontext	sdl;
	t_gamereturn	gr;

	checkargs(argc, argv);
	gr = game_switchmode;
	if (argc == 2 && ft_strcmp(argv[1], "-editor") == 0)
	{
		create_sdlcontext(&sdl, MODE_EDITOR);
		gr = editorloop(sdl);
	}
	else
	{
		create_sdlcontext(&sdl, MODE_PLAY);
		gr = playmode(sdl);
	}
	/*while (gr == game_switchmode)
	{
		//gr = editorloop(sdl); // quit & exit is handled inside the loop
		gr = playmode(sdl); // quit & exit is handled inside the loop
	}*/
}

/*

int	main(int argc, char **argv)
{
	/* pid_t	pid;
	int		wait_status;

	pid = fork();
	if (pid == -1)
	{
		doomlog(LOG_EC_FORK, "couldn't create a process for the game");
		error_window("couldn't launch the game due to a process fork fail");
	}
	if (pid == 0) // child process is always pid 0*//*
		doomnukem(argc, argv);
	else
	{
		wait(&wait_status);
		handle_exit(wait_status);
	} 
	doomnukem(argc, argv);
	return (0);
}
*/
int	main(int argc, char **argv)
{
	pid_t	pid;
	int		wait_status;

	doomnukem(argc, argv);
	return (0);
}
