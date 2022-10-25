/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/25 13:37:59 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

static void	drawsquare(t_sdlcontext sdl, t_point crd, int clr)
{
	int	i;

	i = 0;
	while (i < TILESIZE)
	{
		draw(sdl, point_add(crd, (t_point){i, 0}), clr);
		draw(sdl, point_add(crd, (t_point){0, i}), clr);
		draw(sdl, point_add(crd, (t_point){TILESIZE, i}), clr);
		draw(sdl, point_add(crd, (t_point){TILESIZE, i}), clr);
		i++;
	}
}

static void	drawgrid(t_sdlcontext sdl, t_point origin)
{
	t_point	crd;

	crd.y = origin.y;
	while (crd.y < (TILESIZE * GRIDSIZE) + origin.y)
	{
		crd.x = origin.x;
		while (crd.x < (TILESIZE * GRIDSIZE) + origin.x)
		{
			drawsquare(sdl, crd, CLR_GRAY);
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
	//get_tools();
	while (1)
	{
		update_deltatime(&ed.clock);
		update_anim(&ed.transition, ed.clock.delta);
		//ft_bzero(sdl.surface->pixels, sizeof(uint32_t) * WINDOW_H * WINDOW_W);
		ft_bzero(sdl.surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
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
			drawgrid(sdl, ed.offset);
			renderlines(&sdl, &ed);
			if (ed.tool != NULL)
			{
				ed.tool->update(&ed);
				ed.tool->draw_update(&ed, sdl);
			}
		}
		draw_editor_buttons(sdl, ed.tool_selected);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	return(game_error); //should never get here
}