/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/01 15:31:27 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "inputhelp.h"

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

void	buttoncreator(t_editor *ed, t_sdlcontext sdl);

int	editorloop(t_sdlcontext sdl)
{
	t_editor		ed;
	t_gamereturn	gr;

	save_text(sdl.font, "1234567890 draw_saved_text");
	save_text(sdl.font, "| | | | |");
	bzero(&ed, sizeof(t_editor));
	ed.linelist = load_chunk("map_test1", "WALL", sizeof(t_line));
	ed.entitylist = load_chunk("map_test1", "ENT_", sizeof(t_entity));
	ed.buttonlist = load_chunk("buttons", "BUTN", sizeof(t_guibutton));
	initialize_buttons(ed.buttonlist, sdl);
	ed.tool = get_point_tool();
	gr = game_continue;
	while (gr == game_continue)
	{
		update_deltatime(&ed.clock);
		ft_bzero(sdl.surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
		gr = editor_events(&ed);
		drawgrid(sdl, ed.offset);
		renderlines(&sdl, &ed);
		draw_buttons(ed, sdl);
		if (ed.tool != NULL)
		{
			ed.tool->update(&ed);
			ed.tool->draw_update(&ed, sdl);
			if (ed.tool->icon != NULL) //Indicates which tool is selected
				draw_image(sdl, (t_point){ 8, sdl.window_h - 40 }, *ed.tool->icon, (t_point){32, 32});
			else if (ed.tool->icon_name[0] != '\0')
				ed.tool->icon = get_image_by_name(sdl, ed.tool->icon_name);
		}
		draw_saved_text(&sdl, sdl.font->texts[0], (t_point){400, 100});
		draw_text(&sdl, sdl.font, "1234567890 draw_text", (t_point){400, 150});
		draw_saved_text(&sdl, sdl.font->texts[1], (t_point){400, 200});
		draw_text(&sdl, sdl.font, "| | | | |", (t_point){400, 250});
		ed.mouse.click_unhandled = false;
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	save_lists_to_file(&ed);
	if (gr == game_exit)
		quit_game(&sdl);
	return (gr);
}