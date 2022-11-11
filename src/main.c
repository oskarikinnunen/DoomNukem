/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/11 16:18:48 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "png.h"
#include "game_lua.h"
#include "objects.h"

static void	create_sdl_context(t_sdlcontext *sdl)
{
	int			joysticks;
	const char	*platform;

	load_lua_conf(sdl);
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		error_log(EC_SDL_INIT);
	if (SDL_Init(SDL_INIT_EVENTS) < 0)
		error_log(EC_SDL_INIT);
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
		error_log(EC_SDL_INIT);
		
	platform = SDL_GetPlatform();
	printf("platform: %s\n", platform);
	if (ft_strequ(platform, "Mac OS X"))
		sdl->platform = macosx;
	else if (ft_strequ(platform, "Linux"))
		sdl->platform = linux;
	else
	{
		sdl->platform = unsupported;
		printf("platform %s has no controller support\n", platform);
	}
	
	sdl->window = SDL_CreateWindow("DoomNukem",
	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	sdl->window_w, sdl->window_h, SDL_WINDOW_SHOWN);
	if (sdl->window == NULL)
		error_log(EC_SDL_CREATEWINDOW);
	sdl->surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->surface == NULL)
		error_log(EC_SDL_GETWINDOW_SURFACE);
		
	joysticks = SDL_NumJoysticks();
	printf("number of joysticks: %i\n", joysticks);
	if (joysticks > 0)
	{
		SDL_JoystickEventState(SDL_ENABLE);
		sdl->joystick = SDL_JoystickOpen(0);
		if (sdl->joystick == NULL)
			error_log(EC_SDL_JOYSTICKOPEN);
		printf("joystick nbr %i opened\n", 0);
	}
	
	load_fonts(sdl);
	
	sdl->zbuffer = malloc(sdl->window_w * sdl->window_h * sizeof(float));
	objects_init(sdl);
}

void	quit_game(t_sdlcontext *sdl)
{
	SDL_JoystickEventState(SDL_IGNORE); // could be utilized earlier if controller input is not wanted
	SDL_JoystickClose(sdl->joystick); // could be closed after joystick's been unplugged
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
		gr = editorloop(sdl); // quit & exit is handled inside the loop
		gr = playmode(sdl); // quit & exit is handled inside the loop
		//printf("%s\ngamereturn after playmode %i \n", CLEARSCREEN, gr);
	}
	//shouldn't get here?
	return (0);
}
