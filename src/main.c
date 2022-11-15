/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/15 16:18:47 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "png.h"
#include "game_lua.h"
#include "objects.h"

static void	tryout_ttf(void)
{
	TTF_Font	*newfont;
	SDL_Surface	*surfacetext;

	newfont = TTF_OpenFont("./assets/fonts/grixel_acme_9/Acme_9_Regular.ttf", 32); // what is 32?
	if (newfont == NULL)
	{
		printf("couldn't load the ttf font file\n");
		exit(0);
	}
	TTF_RenderText_Blended();
}

static void	create_sdl_context(t_sdlcontext *sdl)
{
	const char	*platform;

	load_lua_conf(sdl);
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		error_log(EC_SDL_INIT);
	if (SDL_Init(SDL_INIT_EVENTS) < 0)
		error_log(EC_SDL_INIT);
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
		error_log(EC_SDL_INIT);
	if (TTF_Init() == -1)
		printf("%s\n", TTF_GetError());
	else
		printf("TTF_Init succesful\n");

	tryout_ttf();
	
	platform = SDL_GetPlatform();
	printf("platform: %s\n", platform);
	if (ft_strequ(platform, "Mac OS X"))
		sdl->platform = os_mac;
	else if (ft_strequ(platform, "Linux"))
		sdl->platform = os_linux;
	else
	{
		sdl->platform = os_unsupported;
		printf("platform %s not supported\n", platform);
	}
	
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
		gr = editorloop(sdl); // quit & exit is handled inside the loop
		gr = playmode(sdl); // quit & exit is handled inside the loop
		//printf("%s\ngamereturn after playmode %i \n", CLEARSCREEN, gr);
	}
	//shouldn't get here?
	return (0);
}
