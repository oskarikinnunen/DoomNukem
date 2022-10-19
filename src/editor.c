/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/19 18:34:01 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	drawrect(uint32_t *pxls, t_point crd, int clr)
{
	int	i;

	i = 0;
	while (i < TILESIZE)
	{
		draw(pxls, point_add(crd, (t_point){i, 0}), clr);
		draw(pxls, point_add(crd, (t_point){0, i}), clr);
		draw(pxls, point_add(crd, (t_point){TILESIZE, i}), clr);
		draw(pxls, point_add(crd, (t_point){TILESIZE, i}), clr);
		i++;
	}
}

void	drawgrid(uint32_t *pxls, t_point origin)
{
	t_point	crd;

	crd.y = origin.y;
	while (crd.y < (TILESIZE * GRIDSIZE) + origin.y)
	{
		crd.x = origin.x;
		while (crd.x < (TILESIZE * GRIDSIZE) + origin.x)
		{
			drawrect(pxls, crd, CLR_GRAY);
			crd.x += TILESIZE;
		}
		crd.y += TILESIZE;
	}
}

int	editorloop(t_sdlcontext sdl)
{
	t_editor		ed;
	t_gamereturn	gr;

	bzero(&ed, sizeof(t_editor));
	ed.linelist = loadmap("mapfile1");
	//FDF
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
		else
		{
			draw_image(sdl.surface->pixels, (t_point){30, 50}, *sdl.images, 120);
			drawgrid((uint32_t *)sdl.surface->pixels, ed.mousedrag->pos);
			renderlines(&sdl, &ed);
		}
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	return(game_error); //should never get here
}