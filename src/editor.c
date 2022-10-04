/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 11:23:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	drawrect(uint32_t *pxls, int crd[2], int clr)
{
	int	i;

	i = 0;
	while (i < TILESIZE)
	{
		draw(pxls, (int [2]){crd[X] + i, crd[Y]}, clr);
		draw(pxls, (int [2]){crd[X], crd[Y] + i}, clr);
		draw(pxls, (int [2]){crd[X] + TILESIZE, crd[Y] + i}, clr);
		draw(pxls, (int [2]){crd[X] + i, crd[Y] + TILESIZE}, clr);
		/*pxls[crd[X] + i + crd[Y] * WINDOW_W] = clr;
		pxls[crd[X] + (crd[Y] + i) * WINDOW_W] = clr;
		pxls[crd[X] + TILESIZE + (crd[Y] + i) * WINDOW_W] = clr;
		pxls[crd[X] + i + (crd[Y] + TILESIZE) * WINDOW_W] = clr;*/
		i++;
	}
}

void	drawgrid(uint32_t *pxls, int origin[2])
{
	int	crd[2];

	crd[Y] = origin[Y];
	while (crd[Y] < (TILESIZE * GRIDSIZE) + origin[Y])
	{
		crd[X] = origin[X];
		while (crd[X] < (TILESIZE * GRIDSIZE) + origin[X])
		{
			drawrect(pxls, crd, INT_MAX);
			crd[X] += TILESIZE;
		}
		crd[Y] += TILESIZE;
	}
}

void	editorloop(t_sdlcontext sdl)
{
	t_editor	ed;

	bzero(&ed, sizeof(t_editor));
	loadmap(&ed, "mapfile1");
	while (1)
	{
		bzero(sdl.surface->pixels, sizeof(uint32_t) * WINDOW_H * WINDOW_W);
		if (editor_events(&ed))
			break ; //error returned from event handling, exit gracefully
		drawgrid((uint32_t *)sdl.surface->pixels, ed.mousedrag.pos);
		renderlines(&sdl, &ed);
		SDL_UpdateWindowSurface(sdl.window);
	}
	savemap(&ed, "mapfile1");
}