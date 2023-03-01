#include "doomnukem.h"

static void	init_sdl_error_window2(t_sdlcontext *sdl)
{
	sdl->window_surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->window_surface == NULL)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't create a surface for the error message window", 2);
		ft_putendl_fd(SDL_GetError(), 2);
        exit (1);
	}
	sdl->surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, 600, 300, 32, SDL_PIXELFORMAT_ARGB8888);
	if (sdl->surface == NULL)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't create a RGB surface for the error message window", 2);
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
		ft_putendl_fd("game ran into an error and the parent process couldn't init sdl for the error message", 2);
		ft_putendl_fd(SDL_GetError(), 2);
        exit (1);
	}
	/* does this need to be protected for tiny window sizes? */
	sdl->window = SDL_CreateWindow("Error",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			600, 300, SDL_WINDOW_SHOWN);
	if (sdl->window == NULL)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't create a window for the error message", 2);
		ft_putendl_fd(SDL_GetError(), 2);
        exit (1);
	}
    init_sdl_error_window2(sdl);
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
		doomlog(LOG_EC_SDL_CREATEWINDOW, NULL);
	sdl->window_surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->window_surface == NULL)
		doomlog(LOG_EC_SDL_GETWINDOW_SURFACE, NULL);
}

