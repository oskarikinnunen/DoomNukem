/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/07 16:23:25 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "png.h"
#include "game_lua.h"
#include "objects.h"
#include "file_io.h"
#include <sys/wait.h>
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
		doomlog(LOG_EC_SDL_CREATEWINDOW, NULL);
	sdl->window_surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->window_surface == NULL)
		doomlog(LOG_EC_SDL_GETWINDOW_SURFACE, NULL);
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
	printf("prefs volume was %f \n", prefs.volume);
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

void	quit_game(t_sdlcontext *sdl)
{
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
		doomlog(LOG_EC_SDL_INIT, NULL);
	set_sdl_settings(sdl);
	printf("audio volume %f \n", sdl->audio.sfx_volume);
	load_assets(sdl);
	printf("audio volume2 %f \n", sdl->audio.sfx_volume);
}

void	checkargs(int argc, char **argv)
{
	if (argc == 2 && ft_strcmp(argv[1], "-gfxreset") == 0)
		reset_graphics_prefs();
	if (argc == 2 && ft_strcmp(argv[1], "-mapreset") == 0)
	{
		
	}
}

void	show_error_message(t_sdlcontext	*sdl, char *str)
{
	ft_bzero(sdl, sizeof(t_sdlcontext));
	if (SDL_Init(SDL_INIT_VIDEO) < 0 \
			|| SDL_Init(SDL_INIT_EVENTS) < 0 \
			|| SDL_Init(SDL_INIT_GAMECONTROLLER) < 0 \
			|| TTF_Init() < 0)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't init sdl for the error message", 2);
		exit (1);
	}
	/* does this need to be protected for tiny window sizes? */
	sdl->window = SDL_CreateWindow("Error",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			600, 300, SDL_WINDOW_SHOWN);
	if (sdl->window == NULL)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't create a window for the error message", 2);
		exit (1);
	}
	sdl->window_surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->window_surface == NULL)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't create a surface for the error message window", 2);
		exit (1);
	}
	sdl->surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, 600, 300, 32, SDL_PIXELFORMAT_ARGB8888);
	if (sdl->surface == NULL)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't create a RGB surface for the error message window", 2);
		exit (1);
	}
	load_fonts(&sdl->font);
	t_hid_info	hid;
	SDL_Event	event;
	int			fd;
	char		*line;
	int			message_count;
	char		**messages;
	int			i;
	int			j;
	static	int	scroll;
	int			gnl;

	fd = open("doomlog.txt", O_RDONLY);
	if (fd == -1)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't open the doomlog.txt for the error message", 2);
		exit (1);
	}
	message_count = 0;
	line = NULL;
	gnl = get_next_line(fd, &line);
	if (gnl == -1)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't read the doomlog.txt for the error message", 2);
		exit(1);
	}
	while (gnl)
	{
		if (line)
		{
			free(line);
			line = NULL;
		}
		message_count++;
		gnl = get_next_line(fd, &line);
	}
	if (gnl == -1)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't read the doomlog.txt for the error message", 2);
		exit(1);
	}
	if (close(fd) == -1)
		ft_putendl_fd("couldn't close the doomlog.txt from the parent process", 2);
	messages = (char **)malloc(sizeof(char *) * (message_count + 3));
	if (messages == NULL)
	{
		ft_putendl_fd("game ran into an error and the parent process's malloc for the error message failed", 2);
		exit (1);
	}
	fd = open("doomlog.txt", O_RDONLY);
	if (fd == -1)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't open the doomlog.txt for the error message", 2);
		exit (1);
	}
	messages[0] = ft_strdup("GAME RAN INTO AN ERROR! DISPLAYING LOG ENTRIES FROM DOOMLOG.TXT:");
	i = 1;
	gnl = get_next_line(fd, &line);
	if (gnl == -1)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't read the doomlog.txt for the error message", 2);
		exit(1);
	}
	while (gnl)
	{
		messages[i] = line;
		i++;
		gnl = get_next_line(fd, &line);
	}
	if (gnl == -1)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't read the doomlog.txt for the error message", 2);
		exit(1);
	}
	messages[i++] = str;
	messages[i] = NULL;
	sdl->font.color = color32_to_sdlcolor(CLR_GREEN);
	i = 0;
	while (1)
	{
		hid.mouse.scroll_delta = 0; //Needs to be reset
		hid.alphakey_pressed = 0; //Needs to be reset
		SDL_GetRelativeMouseState(&hid.mouse.delta.x, &hid.mouse.delta.y);
		while (SDL_PollEvent(&event))
		{
			ft_bzero(sdl->window_surface->pixels, sizeof(uint32_t) * 600 * 300);
			ft_bzero(sdl->surface->pixels, sizeof(uint32_t) * 600 * 300);
			mouse_event(event, &hid.mouse);
			if ((event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) || \
					(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
			{
				ft_putendl_fd("game ran into an error and the parent process displayed the error message succesfully", 2);
				exit (0);
			}
			i = ft_clamp((i - hid.mouse.scroll_delta), 0, (message_count - 22 + 2));
			j = i;
			while (j < message_count + 3)
			{
				if (j >= 0 && j < message_count + 3)
					print_text(sdl, messages[j], (t_point){10, (10 + (j - i) * 13)});
				j++;
			}
			memcpy(sdl->window_surface->pixels, sdl->surface->pixels, sizeof(uint32_t) * 600 * 300);
			if (SDL_UpdateWindowSurface(sdl->window) < 0)
			{
				ft_putendl_fd("game ran into an error and the parent process couldn't update the window surface for the error message", 2);
				exit (1);
			}
		}
	}
}

char	*signal_name(int wait_status)
{
	static char	signal[16];

	if (WTERMSIG(wait_status) == SIGSEGV)
		ft_strcpy(signal, "SIGSEGV");
	if (WTERMSIG(wait_status) == SIGBUS)
		ft_strcpy(signal, "SIGSEGV");
	if (WTERMSIG(wait_status) == SIGILL)
		ft_strcpy(signal, "SIGILL");
	if (WTERMSIG(wait_status) == SIGABRT)
		ft_strcpy(signal, "SIGABRT");
	if (WTERMSIG(wait_status) == SIGIOT)
		ft_strcpy(signal, "SIGIOT");
	if (WTERMSIG(wait_status) == SIGTRAP)
		ft_strcpy(signal, "SIGTRAP");
	if (WTERMSIG(wait_status) == SIGEMT)
		ft_strcpy(signal, "SIGEMT");
	if (WTERMSIG(wait_status) == SIGSYS)
		ft_strcpy(signal, "SIGSYS");
	return (signal);
}

char	*combine_strings(char **str)
{
	int		x;
	int		y;
	int		len;
	char	*result;

	len = 0;
	y = 0;
	while (str[y] != NULL)
	{
		x = 0;
		while (str[y][x] != '\0')
			x++;
		len += x;
		if (str[y + 1] != NULL)
			len += 1;
		y++;
	}
	result = ft_strnew(len);
	if (result == NULL)
		return (result);
	y = 0;
	x = 0;
	while (str[y] != NULL)
	{
		ft_strcpy(&result[x], str[y]);
		x += ft_strlen(str[y]);
		result[x] = ' ';
		x++;
		y++;
	}
	result[len] = '\0';
	return (result);
}

int	main(int argc, char **argv)
{
	t_sdlcontext	sdl;
	t_gamereturn	gr;

	int	pid;
	
	pid = fork();
	if (pid == -1)
	{
		ft_putendl_fd("fork failed", 2);
		return (1);
	}
	// child process is always pid 0
	if (pid == 0)
	{
		generate_struct_datas();
		checkargs(argc, argv);
		create_sdlcontext(&sdl);
		gr = game_switchmode;
		while (gr == game_switchmode)
		{
			gr = editorloop(sdl); // quit & exit is handled inside the loop
			gr = playmode(sdl); // quit & exit is handled inside the loop
		}
	}
	// parent process
	else
	{
		int	wait_status;
		
		wait(&wait_status);
		if (WIFSIGNALED(wait_status))
		{
			ft_putendl_fd(combine_strings((char *[5]){"child process raised a signal that caused it to exit:", \
												s_itoa(WTERMSIG(wait_status)), \
												"-", signal_name(wait_status)}), 2);
			show_error_message(&sdl, combine_strings((char *[5]){"child process raised a signal that caused it to exit:", \
												s_itoa(WTERMSIG(wait_status)), \
												"-", signal_name(wait_status)}));
			return (1);
		}
		else if (WIFEXITED(wait_status))
		{
			ft_putendl_fd(combine_strings((char *[3]){"child process exited with status:", \
												s_itoa(WEXITSTATUS(wait_status))}), 2);
			if (WEXITSTATUS(wait_status) != 0)
			{
				show_error_message(&sdl, combine_strings((char *[3]){"child process exited with status:", \
												s_itoa(WEXITSTATUS(wait_status))}));
				return (1);
			}
		}
		else
		{
			ft_putendl_fd("no exit signal or status from the child process", 2);
			show_error_message(&sdl, "no exit signal or status from the child process");
		}
	}
	return (0);
}
