/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 20:03:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "png.h"

static void	create_sdl_context(t_sdlcontext *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		error_log(EC_SDL_INIT);
	if (SDL_Init(SDL_INIT_EVENTS) < 0)
		error_log(EC_SDL_INIT);
	sdl->window = SDL_CreateWindow("DoomNukem",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	if (sdl->window == NULL)
		error_log(EC_SDL_CREATEWINDOW);
	sdl->surface = SDL_GetWindowSurface(sdl->window);
	if (sdl->surface == NULL)
		error_log(EC_SDL_GETWINDOW_SURFACE);
	sdl->images = ft_memalloc(sizeof(t_img) * 2);
	if (sdl->images == NULL)
		error_log(EC_MALLOC);
	sdl->images[0] = pngparse("images/spritefont2.png");
	sdl->images[1] = pngparse("images/wall2.png");
	sdl->imagecount = 2; //TODO: Restructure the code so this is incremented automatically when loading a new image to sdl->images
}

void	quit_sdl(t_sdlcontext *sdl)
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
		if (gr == game_exit) // quit & exit is handled inside of editorloop atm
			break ;
		gr = playmode(sdl);
		if (gr == game_exit) // quit & exit is handled inside of playmode atm
			break ;
	}
	//quit_sdl(&sdl);
	return (0);
}
