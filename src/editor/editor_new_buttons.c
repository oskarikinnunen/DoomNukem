/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_new_buttons.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:34:40 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 12:16:29 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

static void	change_tool(t_editor *ed, t_sdlcontext *sdl, t_tool *newtool)
{
	if (ed->tool != NULL && ed->tool->cleanup != NULL)
		ed->tool->cleanup(ed, sdl);
	ed->tool = newtool;
	if (ed->tool->init != NULL)
		ed->tool->init(ed, sdl);
}

void	update_editor_toolbar(t_editor *ed, t_autogui *toolbar)
{
	SDL_Event	e;

	toolbar->rect.size.y = 60;
	toolbar->rect.size.x = 540;
	gui_start(toolbar);
	gui_start_horizontal(toolbar);
	if (gui_button("[1]Rooms", toolbar) || ed->hid.keystate >> KEYS_1MASK & 1)
		change_tool(ed, toolbar->sdl, get_room_tool());
	if (gui_button("[2]Entities", toolbar) || \
			ed->hid.keystate >> KEYS_2MASK & 1)
		change_tool(ed, toolbar->sdl, get_entity_tool());
	if (gui_shortcut_button("No tool", 't', toolbar))
		ed->tool = NULL;
	gui_end_horizontal(toolbar);
	gui_end(toolbar);
}
