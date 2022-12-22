/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/22 15:52:40 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "png.h"
#include "game_lua.h"
#include "objects.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

typedef enum e_screenmode
{
	screenmode_windowed,
	screenmode_borderless,
	screenmode_fullscreen
}	t_screenmode;


static void	create_sdl_context(t_sdlcontext *sdl, t_screenmode	screenmode)
{
	const char	*platform;

	ft_bzero(sdl, sizeof(t_sdlcontext));
	load_lua_conf(sdl);
	SDL_DisplayMode	mode;
	sdl->resolution_scaling = 1.0f;
	if (SDL_Init(SDL_INIT_VIDEO) < 0 \
		|| SDL_Init(SDL_INIT_EVENTS) < 0 \
		|| SDL_Init(SDL_INIT_GAMECONTROLLER) < 0 \
		|| TTF_Init() < 0)
		error_log(EC_SDL_INIT);
	if (screenmode == screenmode_borderless && SDL_GetCurrentDisplayMode(0, &mode) == 0)
	{
		sdl->window_w = mode.w;
		sdl->window_h = mode.h;
		sdl->screensize = (t_point) {sdl->window_w, sdl->window_h};
		sdl->resolution_scaling = 0.5f;
		float hdpi;
		float vdpi;
		SDL_GetDisplayDPI(0, NULL, &hdpi, &vdpi);
		printf("dpi %f %f \n", hdpi, vdpi);
		printf("mode res %i %i \n", mode.w, mode.h);
	}
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
		sdl->window_w, sdl->window_h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	if (sdl->window == NULL)
		error_log(EC_SDL_CREATEWINDOW);
	if (screenmode == screenmode_borderless)
	{
		SDL_SetWindowBordered(sdl->window, SDL_FALSE);
		SDL_SetWindowPosition(sdl->window, 0, 0);
	}
		
	sdl->window_surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->window_surface == NULL)
		error_log(EC_SDL_GETWINDOW_SURFACE);

	sdl->surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, sdl->window_w, sdl->window_h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (sdl->surface == NULL)
		error_log(EC_SDL_CREATERGBSURFACE);
	sdl->ui_surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, sdl->window_w, sdl->window_h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (sdl->ui_surface == NULL)
		error_log(EC_SDL_CREATERGBSURFACE);
	load_fonts(&sdl->font);
	load_audio(&sdl->audio);

	sdl->zbuffer = malloc(sdl->window_w * sdl->window_h * sizeof(float));
	objects_init(sdl);
	t_object *o = get_object_by_name(*sdl, "cyborg");
	parseanim(o, "walk");
	/* create context here, call gl clear in render start, glbegin in drawtriangles etc */
	SDL_GLContext glc = SDL_GL_CreateContext(sdl->window);
	t_point	drawablesize;
	SDL_GL_GetDrawableSize(sdl->window, &drawablesize.x, &drawablesize.y);
	printf("gl draw size %i %i \n", drawablesize.x, drawablesize.y);
	//exit(0);
	/*glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	SDL_GL_SwapWindow(sdl->window);
	SDL_Delay(10000);*/
	printf("OPENGL RENDERER: '%s' \n", glGetString(GL_RENDERER));
}

void	quit_game(t_sdlcontext *sdl)
{
	close_audio(&sdl->audio);
	SDL_Quit();
	exit(0);
}

int	main(int argc, char **argv)
{
	t_sdlcontext	sdl;
	t_gamereturn	gr;
	t_screenmode	screenmode; //TODO: make a struct for this that also contains other data, such as screen index

	screenmode = screenmode_windowed;
	printf("args %i \n", argc);
	if (argc > 1 && ft_strcmp(argv[1], "-borderless") == 0)
	{
		screenmode = screenmode_borderless;
	}
	create_sdl_context(&sdl, screenmode);
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

