/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/14 13:52:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	createsdlcontext(t_sdlcontext *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || SDL_Init(SDL_INIT_EVENTS) < 0)
		printf("SDL_Init error");
	sdl->window = SDL_CreateWindow("DoomNukem",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	if (sdl->window == NULL)
		printf("Couldn't create SDL2 window :(");
	printf("sdl context and init \n");
	sdl->surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->surface == NULL)
		(printf("WTH SURFACE NULL"), exit(0));
	printf("%s \n", SDL_GetError());
}

int	main(int argc, char **argv)
{
	t_sdlcontext	sdl;
	t_gamereturn		gr;

	createsdlcontext(&sdl);
	while (1)
	{
		gr = editorloop(sdl);
		if (gr == game_exit)
			break ;
		gr = playmode(sdl);
		if (gr == game_error || gr == game_exit)
			break ;
	}
	//SDL_DestroyWindow()
	//TODO: sdl exit
	return 0;
}