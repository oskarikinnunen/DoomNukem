/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 20:29:33 by raho             ###   ########.fr       */
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
			drawrect(pxls, crd, CLR_GRAY);
			crd[X] += TILESIZE;
		}
		crd[Y] += TILESIZE;
	}
}

static void debug_draw_object(uint32_t *pxls, t_obj obj) //remove this and use proper fdf rendering
{
	int			i;

	i = 0;
	while (i < obj.f_count)
	{
		drawline(pxls, obj.verts[obj.faces[i][0]], obj.verts[obj.faces[i][1]], CLR_TURQ);
		drawline(pxls, obj.verts[obj.faces[i][1]], obj.verts[obj.faces[i][2]], CLR_TURQ);
		drawline(pxls, obj.verts[obj.faces[i][2]], obj.verts[obj.faces[i][0]], CLR_TURQ);
		i++;
	}
}
	

int	editorloop(t_sdlcontext sdl)
{
	t_editor		ed;
	t_gamereturn	gr;

	bzero(&ed, sizeof(t_editor));
	loadmap(&ed.linelist, "mapfile1");

	//FDF
	alloc_image(&ed.grid_fdf.img, WINDOW_W, WINDOW_H);
	alloc_image(&ed.walls_fdf.img, WINDOW_W, WINDOW_H);
	ed.transition.framerate = 60;
	ed.transition.lastframe = 15;
	ed.transition.active = false;
	ed.threedee_zoom = 1.0f;
	while (1)
	{
		update_deltatime(&ed.clock);
		update_anim(&ed.transition, ed.clock.delta);
		ft_bzero(sdl.surface->pixels, sizeof(uint32_t) * WINDOW_H * WINDOW_W);
		gr = editor_events(&ed); 
		if (gr != game_continue)
		{
			savemap(&ed, "mapfile1");
			if (gr == game_exit)
			{
				quit_sdl(&sdl);
				exit (0);
			}
			return(gr) ; //error returned from event handling, exit gracefully
		}
		if (ed.state == display3d || ed.transition.active)
			editor3d(sdl, &ed);
		else
		{
			drawgrid((uint32_t *)sdl.surface->pixels, ed.mousedrag->pos);
			renderlines(&sdl, &ed);
		}
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	return(game_error); //should never get here
}