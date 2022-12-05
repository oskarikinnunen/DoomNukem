/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_new_buttons.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:34:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/05 17:19:31 by okinnune         ###   ########.fr       */
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

void update_editor_buttons(t_editor *ed, t_sdlcontext *sdl)
{
	set_font_size(sdl, 3);
	if (instant_text_button(*sdl, &ed->hid.mouse, "Rooms", (t_point) {20, 20}))
		changetool(ed, sdl, get_room_tool());
	if (instant_text_button(*sdl, &ed->hid.mouse, "Entities", (t_point) {95, 20}))
		changetool(ed, sdl, get_entity_tool());
	if (instant_text_button(*sdl, &ed->hid.mouse, "Guns", (t_point) {180, 20}))
		changetool(ed, sdl, get_gun_tool());
	if (instant_text_button(*sdl, &ed->hid.mouse, "NPCs", (t_point) {245, 20}))
		changetool(ed, sdl, get_npc_tool());
	set_font_size(sdl, 0);
}

