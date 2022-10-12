/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/12 15:40:10 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	create_sdl_context(t_sdlcontext *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		errors(1);
	if (SDL_Init(SDL_INIT_EVENTS) < 0)
		errors(1);
	sdl->window = SDL_CreateWindow("DoomNukem",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	if (sdl->window == NULL)
		errors(2);
	sdl->surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->surface == NULL)
		errors(3);
}

static void	quit_sdl(t_sdlcontext *sdl)
{
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}

int	main(int argc, char **argv)
{
	t_sdlcontext	sdl;
	t_gamereturn	gr;

	create_sdl_context(&sdl);
	while (1)
	{
		gr = editorloop(sdl);
		if (gr == game_exit)
			break ;
		gr = playmode(sdl);
		if (gr == game_exit)
			break ;
	}
	quit_sdl(&sdl);
	return (0);
}