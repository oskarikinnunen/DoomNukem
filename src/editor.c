/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/21 17:03:15 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "render.h"
#include "objects.h"

int	editorloop(t_sdlcontext sdl)
{
	t_editor		ed;

	bzero(&ed, sizeof(t_editor));
	ed.world = load_world("world1", &sdl);
	
	*(ed.world.debug_gui) = init_gui(&sdl, &ed.hid, &ed.player, sdl.screensize, "Debugging menu (F2)");
	ed.world.debug_gui->minimum_size.y = 135;
	ed.world.debug_gui->rect.position = sdl.screensize;
	ed.toolbar_gui = init_gui(&sdl, &ed.hid, &ed.player, (t_point){5, 5}, "Toolbar");
	ed.toolbar_gui.minimum_size = (t_point){165, 20};
	ed.toolbar_gui.locked = true;

	ed.gamereturn = game_continue;
	ed.render = init_render(sdl, &ed.world);
	player_init(&ed.player, &sdl);
	ed.player.transform.position = (t_vector3){1000, 1000, 250};
	ed.player.gun->disabled = true;
	/*ed.angle = (t_vector2){-20.0f, -RAD90 * 0.99f};
	ed.position = (t_vector3){500.0f, 500.0f, 200.0f};*/
	ed.tool = NULL;
	//set_font_size(&sdl, 0);
	while (ed.gamereturn == game_continue)
	{
		bzero((uint32_t *)sdl.surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
		bzero((uint32_t *)sdl.ui_surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
		update_deltatime(&ed.clock);
		update_deltatime(&ed.world.clock);
		ed.gamereturn = editor_events(&ed);
		if (!ed.player.locked)
			moveplayer(&ed.player, &ed.hid.input, ed.clock);
		update_render(&ed.render, &ed.player);
		screen_blank(sdl);
		
		render_start(&ed.render);
		update_world3d(&ed.world, &ed.render);
		update_editor_toolbar(&ed, &ed.toolbar_gui);
		if (ed.tool != NULL)
		{
			ed.tool->update(&ed, &sdl);
		}
		ed.hid.mouse.click_unhandled = false;
		//print_text_boxed(&sdl, "tab to unlock/lock mouse, shift + enter to go to playmode", (t_point){sdl.window_w / 2, 10}, (t_point){sdl.window_w, sdl.window_h});
		char *fps = ft_itoa(ed.clock.fps);
		print_text(&sdl, fps, (t_point){sdl.window_w - 80, 10});
		drawcircle(sdl, point_div(sdl.screensize, 2), 4, CLR_BLUE);
		free(fps);
		if (!ed.player.gun->disabled)
			render_entity(&sdl, &ed.render, &ed.player.gun->entity);
		//update_debugconsole(&ed.world.debugconsole, &sdl, ed.clock.delta);
		rescale_surface(&sdl);
		join_surfaces(sdl.window_surface, sdl.surface);
		join_surfaces(sdl.window_surface, sdl.ui_surface);
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