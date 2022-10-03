/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/03 14:17:58 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	drawrect(uint32_t *pxls, int crd[2], int clr)
{
	int	i;

	i = 0;
	while (i < TILESIZE)
	{
		pxls[crd[X] + i + crd[Y] * WINDOW_W] = clr;
		pxls[crd[X] + (crd[Y] + i) * WINDOW_W] = clr;
		pxls[crd[X] + TILESIZE + (crd[Y] + i) * WINDOW_W] = clr;
		pxls[crd[X] + i + (crd[Y] + TILESIZE) * WINDOW_W] = clr;
		i++;
	}
}

void	drawgrid(uint32_t *pxls)
{
	int	crd[2];

	bzero(crd, sizeof(int [2]));	//TODO: replace with ft?
	while (crd[Y] < TILESIZE * GRIDSIZE)
	{
		crd[X] = 0;
		while (crd[X] < TILESIZE * GRIDSIZE)
		{
			drawrect(pxls, crd, INT_MAX);
			crd[X] += TILESIZE;
		}
		crd[Y] += TILESIZE;
	}
}

int		handle_events(t_sdlcontext *sdl)
{
	static SDL_Event	e;
	int		m[2];

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			printf("clicked mouse \n");
			SDL_GetMouseState(&m[X], &m[Y]);
			printf("%i %i \n", m[X], m[Y]);
			//Divide mx and my by TILESIZE, gets coord? draw debug thingy on position
		}
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
			return (1);
	}
	return (0);
}

void	editorloop(t_sdlcontext sdl)
{
	while (1)
	{
		drawgrid((uint32_t *)sdl.surface->pixels);
		if (handle_events(&sdl))
			return ; //error returned from event handling, exit gracefully
		SDL_UpdateWindowSurface(sdl.window);
	}
}