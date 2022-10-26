/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/26 16:00:48 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"

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
	ed.linelist = load_chunk("map_test1", "WALL");
	ed.entitylist = load_chunk("map_test1", "ENT_");
	ed.tool = get_point_tool();
	ed.tool_selected = 1;
	gr = game_continue;
	while (gr == game_continue)
	{
		update_deltatime(&ed.clock);
		update_anim(&ed.transition, ed.clock.delta);
		ft_bzero(sdl.surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
		gr = editor_events(&ed); 
		drawgrid(sdl, ed.offset);
		renderlines(&sdl, &ed);
		draw_editor_buttons(sdl, ed.tool_selected);
		if (ed.tool != NULL)
		{
			ed.tool->update(&ed);
			ed.tool->draw_update(&ed, sdl);
		}
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	save_lists_to_file(&ed);
	if (gr == game_exit)
		quit_game(&sdl);
	return (gr);
}