/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/28 19:01:36 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
 
#include "objects.h"

int	editorloop(t_sdlcontext sdl)
{
	t_editor		ed;

	bzero(&ed, sizeof(t_editor));
	ed.world = load_world("world1", sdl);
	
	ed.gamereturn = game_continue;
	ed.render = init_render(sdl);
	player_init(&ed.player, &sdl);
	ed.player.gun->disabled = true;
	/*ed.angle = (t_vector2){-20.0f, -RAD90 * 0.99f};
	ed.position = (t_vector3){500.0f, 500.0f, 200.0f};*/
	ed.tool = get_npc_tool();
	set_font_size(&sdl, 0);
	while (ed.gamereturn == game_continue)
	{
		update_deltatime(&ed.clock);
		update_deltatime(&ed.world.clock);
		ed.gamereturn = editor_events(&ed);
		//move_editor(&ed);
		moveplayer(&ed.player, &ed.hid.input, ed.clock);
		update_render(&ed.render, &ed.player);
		screen_blank(sdl);
		render_start(&ed.render);
		update_world3d(sdl, &ed.world, &ed.render);
		draw_buttons(&ed, &sdl);
		if (ed.tool != NULL)
		{
			ed.tool->draw_update(&ed, sdl); //Instant buttons here can toggle mouse.click unhandled, so draw first
			if (ed.tool->icon != NULL) //Indicates which tool is selected
				draw_image(sdl, (t_point){ 8, sdl.window_h - 40 }, *ed.tool->icon, (t_point){32, 32});
			else if (ed.tool->icon_name[0] != '\0')
				ed.tool->icon = get_image_by_name(sdl, ed.tool->icon_name);
		}
		ed.hid.mouse.click_unhandled = false;
		draw_text_boxed(&sdl, "tab to unlock/lock mouse, shift + enter to go to playmode", (t_point){sdl.window_w / 2, 10}, (t_point){sdl.window_w, sdl.window_h});
		
		char *fps = ft_itoa(ed.clock.fps);
		draw_text_boxed(&sdl, fps, (t_point){sdl.window_w - 80, 10}, (t_point){sdl.window_w, sdl.window_h});
		drawcircle(sdl, point_div(sdl.screensize, 2), 4, CLR_BLUE);
		free(fps);
		if (!ed.player.gun->disabled)
			render_entity(sdl, ed.render, &ed.player.gun->entity);
		update_debugconsole(&ed.world.debugconsole, &sdl, ed.clock.delta);
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