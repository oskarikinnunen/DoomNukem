/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_set_up.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfum <kfum@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 22:13:31 by raho              #+#    #+#             */
/*   Updated: 2023/03/28 10:41:46 by kfum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

void	editor_player_init(t_editor *ed)
{
	player_init(&ed->player, ed->world.sdl, &ed->world);
	ed->player.no_clip = true;
	ed->player.gun->disabled = true;
	ed->world.debug_gui->hidden = true;
	ed->graphics_gui.hidden = true;
	ed->world.player = &ed->player;
}

void	set_up_editor(t_sdlcontext *sdl, t_editor *ed)
{
	editor_player_init(ed);
	*(ed->world.debug_gui) = init_gui((t_gui_init){sdl, &ed->hid, \
			&ed->player, sdl->screensize, "Debugging menu (F2)"});
	ed->toolbar_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
			(t_point){5, 5}, "Toolbar (F1)"});
	ed->toolbar_gui.minimum_size = (t_point){165, 20};
	ed->toolbar_gui.locked = true;
	ed->graphics_gui.minimum_size = (t_point){200, 200};
	ed->graphics_gui.rect.position = point_div(sdl->screensize, 2);
	ed->player.locked = !ed->hid.mouse.relative;
	sdl->lighting_toggled = false;
	ed->player.gun->entity->hidden = true;
	sdl->render.occlusion.occlusion = false;
	sdl->fog = false;
}
