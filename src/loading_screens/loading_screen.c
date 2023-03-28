/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_screen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 13:20:32 by raho              #+#    #+#             */
/*   Updated: 2023/03/28 11:34:07 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tga.h"

static void	close_window(SDL_Event	e)
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
		{
			if (iskey(e, SDLK_ESCAPE))
				exit(0);
		}
		if (e.type == SDL_QUIT)
			exit(0);
	}
}

void	print_loading_message(char *loading_message, t_sdlcontext *sdl)
{
	int			len;
	TTF_Font	*temp;

	len = ft_strlen(loading_message);
	temp = sdl->font_default->size_default;
	sdl->font_default->size_default = sdl->font_default->sizes[1];
	print_text_boxed(sdl, loading_message, (t_point){\
			((sdl->window_w / 2) - (len / 2 * (2 * FONT_SIZE_DEFAULT))), \
			(sdl->window_h - (sdl->window_h / 5))});
	sdl->font_default->size_default = temp;
}

void	editor_loading_screen(char *loading_message, t_sdlcontext *sdl)
{
	static int		first_time = 1;
	static t_img	loading_image;
	SDL_Event		e;

	if (first_time)
	{
		loading_image = tga_parse(LOADINGSCREENIMG);
		first_time = 0;
	}
	draw_image(*sdl, point_zero(), loading_image, \
			(t_point){sdl->window_w, sdl->window_h});
	if (loading_message != NULL)
		print_loading_message(loading_message, sdl);
	ft_memcpy(sdl->window_surface->pixels, sdl->surface->pixels, \
			sizeof(uint32_t) * sdl->window_w * sdl->window_h);
	close_window(e);
	if (SDL_UpdateWindowSurface(sdl->window) < 0)
		doomlog(LOG_EC_SDL_UPDATEWINDOWSURFACE, NULL);
}

void	playmode_loading_screen_loop(char *loading_message, t_sdlcontext *sdl)
{
	SDL_Event	e;

	playmode_loading_screen(loading_message, sdl);
	play_music(sdl, "music_arp1_ambient.wav");
	while (1)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (iskey(e, SDLK_ESCAPE))
					exit(0);
				else
					return ;
			}
			if (e.type == SDL_QUIT)
				exit(0);
		}
	}
}

void	playmode_loading_screen(char *loading_message, t_sdlcontext *sdl)
{
	static int		first_time = 1;
	static t_img	*loading_image;
	TTF_Font		*temp;
	int				len;
	SDL_Event		e;

	if (first_time)
	{
		loading_image = get_image_by_name(*sdl, "loading_screen");
		first_time = 0;
	}
	draw_image(*sdl, point_zero(), *loading_image, \
			(t_point){sdl->window_w, sdl->window_h});
	if (loading_message != NULL)
		print_loading_message(loading_message, sdl);
	ft_memcpy(sdl->window_surface->pixels, sdl->surface->pixels, \
			sizeof(uint32_t) * sdl->window_w * sdl->window_h);
	close_window(e);
	if (SDL_UpdateWindowSurface(sdl->window) < 0)
		doomlog(LOG_EC_SDL_UPDATEWINDOWSURFACE, NULL);
}
