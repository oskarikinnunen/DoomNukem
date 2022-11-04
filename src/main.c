/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/04 15:31:40 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "png.h"
#include "game_lua.h"
#include "objects.h"

static void	create_sdl_context(t_sdlcontext *sdl)
{
	load_lua_conf(sdl);
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		error_log(EC_SDL_INIT);
	if (SDL_Init(SDL_INIT_EVENTS) < 0)
		error_log(EC_SDL_INIT);
	sdl->window = SDL_CreateWindow("DoomNukem",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		sdl->window_w, sdl->window_h, SDL_WINDOW_SHOWN);
	if (sdl->window == NULL)
		error_log(EC_SDL_CREATEWINDOW);
	sdl->surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->surface == NULL)
		error_log(EC_SDL_GETWINDOW_SURFACE);
	load_fonts(sdl);
	sdl->zbuffer = malloc(sdl->window_w * sdl->window_h * sizeof(float));
	objects_init(sdl);
}

void	quit_game(t_sdlcontext *sdl)
{
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
	exit(0);
}

int	main(int argc, char **argv)
{
	t_sdlcontext	sdl;
	t_gamereturn	gr;

	create_sdl_context(&sdl);
	gr = game_switchmode;
	while (gr == game_switchmode)
	{
		
		//printf("%s\ngamereturn after editor %i \n", CLEARSCREEN, gr);
		gr = playmode(sdl); // quit & exit is handled inside the loop
		gr = editorloop(sdl); // quit & exit is handled inside the loop
		//printf("%s\ngamereturn after playmode %i \n", CLEARSCREEN, gr);
	}
	//shouldn't get here?
	return (0);
}
