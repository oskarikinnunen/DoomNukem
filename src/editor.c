/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/28 21:14:23 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "inputhelp.h"
#include "objects.h"

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


static void	update_render_editor(t_render *render, t_editor ed) //TODO: move game3d matrix stuff 
{
	render->position = ed.position;
	render->lookdir = ed.forward;
	//render->matcamera = matrix_lookat(render->position, render->vtarget, (t_vector3){0, 0, 1});
	//render->matview = matrix_quickinverse(render->matcamera);
}

int	editorloop(t_sdlcontext sdl)
{
	t_editor		ed;
	int				i;

	bzero(&ed, sizeof(t_editor));
	ed.buttonlist = load_chunk("buttons", "BUTN", sizeof(t_guibutton));
	initialize_buttons(ed.buttonlist, sdl);
	ed.world = load_world("world1", sdl);
	
	ed.gamereturn = game_continue;
	ed.render = init_render(sdl);
	//ed.angle = (t_vector2){-RAD90, -RAD90 * 0.99f};
	ed.angle = (t_vector2){-20.0f, -RAD90 * 0.99f};
	ed.position = (t_vector3){500.0f, 500.0f, 200.0f};
	i = 0;
	ed.tool = get_npc_tool();
	while (ed.gamereturn == game_continue)
	{
		update_deltatime(&ed.clock);
		update_deltatime(&ed.world.clock);
		ed.gamereturn = editor_events(&ed);
		move_editor(&ed);
		update_render_editor(&ed.render, ed);
		screen_blank(sdl);
		render_start(&ed.render);
		update_world3d(sdl, &ed.world, &ed.render);
		if (ed.tool != NULL)
		{
			ed.tool->draw_update(&ed, sdl); //Instant buttons here can toggle mouse.click unhandled, so draw first
			ed.tool->update(&ed);
			if (ed.tool->icon != NULL) //Indicates which tool is selected
				draw_image(sdl, (t_point){ 8, sdl.window_h - 40 }, *ed.tool->icon, (t_point){32, 32});
			else if (ed.tool->icon_name[0] != '\0')
				ed.tool->icon = get_image_by_name(sdl, ed.tool->icon_name);
		}
		draw_buttons(ed, sdl);
		ed.mouse.click_unhandled = false;
		print_text(&sdl, "tab to unlock/lock mouse", (t_point){sdl.window_w / 2, 10});
		print_text(&sdl, "shift + enter to go to playmode", (t_point){sdl.window_w / 2, 45});
		char *fps = ft_itoa(ed.clock.fps);
		print_text(&sdl, fps, (t_point){sdl.window_w - 80, 10});
		
		// showing functionality of print text boxed:
		SDL_Color	temp1;
		uint32_t	temp2;
		temp1 = sdl.font.color;
		temp2 = sdl.font.box_color;
		sdl.font.color = sdl.font.font_colors.green;
		sdl.font.box_color = sdl.font.background_colors.brown;
		print_text_boxed(&sdl, "coming in hot", (t_point){200, 300});
		sdl.font.color = sdl.font.font_colors.orange;
		print_text_boxed(&sdl, "guns blazing", (t_point){250, 350});
		sdl.font.color = temp1;
		sdl.font.box_color = temp2;

		drawcircle(sdl, point_div(sdl.screensize, 2), 4, CLR_BLUE);
		free(fps);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	save_world("world1", ed.world);
	save_editordata(&ed);
	free_render(ed.render);
	if (ed.gamereturn == game_exit)
		quit_game(&sdl);
	return (ed.gamereturn);
}