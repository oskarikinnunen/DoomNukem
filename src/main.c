/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfum <kfum@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/23 14:49:23 by kfum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "png.h"
#include "game_lua.h"
#include "objects.h"
#include <OpenGL/gl.h>

static void	create_sdl_context(t_sdlcontext *sdl)
{
	const char	*platform;

	load_lua_conf(sdl);
	if (SDL_Init(SDL_INIT_VIDEO) < 0
		|| SDL_Init(SDL_INIT_EVENTS) < 0)
		error_log(EC_SDL_INIT);
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
		error_log(EC_SDL_INIT);
		
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
		sdl->window_w, sdl->window_h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	//SDL_CreateWindowAndRenderer(0, 0, )
	if (sdl->window == NULL)
		error_log(EC_SDL_CREATEWINDOW);
	sdl->surface = SDL_GetWindowSurface(sdl->window);
	//sdl->surface->format = SDL_pixeSDL_PIXELFORMAT_BGR24;
	sdl->surface->format->format = SDL_PIXELFORMAT_ABGR1555;
	//SDL_setw
	//sdl->surface
	if (sdl->surface == NULL)
		error_log(EC_SDL_GETWINDOW_SURFACE);
	
	load_fonts(sdl);
	
	sdl->zbuffer = malloc(sdl->window_w * sdl->window_h * sizeof(float));
	objects_init(sdl);


	/* create context here, call gl clear in render start, glbegin in drawtriangles etc */
	/*SDL_GLContext glc = SDL_GL_CreateContext(sdl->window);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	//glBegin( GL_QUADS );
	
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f( -0.5f, -0.5f, 0.2f);
	glVertex3f( 0.5f, -0.5f, 0.5f);
	glVertex3f( 0.5f, 0.5f, 1.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f( -0.6f, -0.6f, 2.0f);
	glVertex3f( 0.5f, -0.6f, 2.0f);
	glVertex3f( 0.1f, 0.3f, -2.0f);
	//glVertex2f( -0.5f, 0.5f );
	glEnd();
	SDL_GL_SwapWindow(sdl->window);*/
	SDL_GLContext glc = SDL_GL_CreateContext(sdl->window);
	printf("OPENGL RENDERER: '%s' \n", glGetString(GL_RENDERER));
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
