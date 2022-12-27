/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_new_buttons.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:34:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/27 19:11:51 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

static void changetool(t_editor *ed, t_sdlcontext *sdl, t_tool *newtool)
{
	if (ed->tool != NULL && ed->tool->cleanup != NULL)
		ed->tool->cleanup(ed, sdl);
	ed->tool = newtool;
	if (ed->tool->init != NULL)
		ed->tool->init(ed, sdl);
}

int		get_tool_index(t_editor *ed)
{
	if (ed->tool == get_room_tool())
		return (1);
	if (ed->tool == get_entity_tool())
		return (2);
	if (ed->tool == get_gun_tool())
		return (3);
	if (ed->tool == get_npc_tool())
		return (4);
	return (0);
}

void    update_editor_toolbar(t_editor *ed, t_autogui *toolbar)
{
    SDL_Event    e;
    toolbar->rect.size.y = 60;
    toolbar->rect.size.x = 420;
    gui_start(toolbar);
    gui_starthorizontal(toolbar);
    if (gui_button("[1]Rooms", toolbar) || ed->hid.keystate >> KEYS_1MASK & 1)
        changetool(ed, toolbar->sdl, get_room_tool());
    if (gui_button("[2]Entities", toolbar) || ed->hid.keystate >> KEYS_2MASK & 1)
        changetool(ed, toolbar->sdl, get_entity_tool());
    if (gui_button("[3]Guns", toolbar) || ed->hid.keystate >> KEYS_3MASK & 1)
        changetool(ed, toolbar->sdl, get_gun_tool());
    if (gui_button("[4]NPCs", toolbar) || ed->hid.keystate >> KEYS_4MASK & 1)
        changetool(ed, toolbar->sdl, get_npc_tool());
    if (gui_shortcut_button("No tool", 't', toolbar))
        ed->tool = NULL;
    gui_endhorizontal(toolbar);
    gui_end(toolbar);
}
