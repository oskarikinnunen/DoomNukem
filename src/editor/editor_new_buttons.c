/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_new_buttons.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:34:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/06 16:30:55 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

static void changetool(t_editor *ed, t_sdlcontext *sdl, t_tool *newtool)
{
	if (ed->tool->cleanup != NULL)
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

void	update_editor_toolbar(t_editor *ed, t_autogui *toolbar)
{
	toolbar->rect.size.y = 60;
	toolbar->rect.size.x = 270;
	gui_start(toolbar);
	gui_starthorizontal(toolbar);
	if (gui_button("[1]Rooms", toolbar))
		changetool(ed, toolbar->sdl, get_room_tool());
	if (gui_button("[2]Entities", toolbar))
		changetool(ed, toolbar->sdl, get_entity_tool());
	if (gui_button("[3]Guns", toolbar))
		changetool(ed, toolbar->sdl, get_gun_tool());
	if (gui_button("[4]NPCs", toolbar))
		changetool(ed, toolbar->sdl, get_npc_tool());
	gui_endhorizontal(toolbar);
	gui_end(toolbar);
}

