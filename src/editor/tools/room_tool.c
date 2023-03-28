/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:32:36 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 09:17:03 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

void	update_maingui(t_editor *ed, t_sdlcontext *sdl, t_room_tool_data *dat)
{
	t_autogui	*gui;

	gui = &dat->maingui;
	gui_start(gui);
	if (gui_shortcut_button("New room", 'N', gui) && dat->rtm != rtm_create \
		&& dat->room == NULL)
	{
		dat->room = prot_memalloc(sizeof(t_area));
		ft_strncpy_term(dat->room->s_floor_tex.str, "concrete02", 60);
		ft_strncpy_term(dat->room->s_ceil_tex.str, "concrete02", 60);
		dat->rtm = rtm_create;
		room_set_preset_room(dat->room);
		force_mouselock(&ed->hid);
	}
	if (dat->room == NULL)
	{
		if (gui_shortcut_button("Paint rooms", 'P', gui))
			dat->rtm = rtm_paint;
	}
	else
		gui_label("Exit createmode to [P]Paint rooms", gui);
	gui_end(gui);
}

void	room_tool_update_nonemode(t_editor *ed,
		t_sdlcontext *sdl, t_room_tool_data *dat)
{
	t_area	*hover;

	hover = NULL;
	hover = get_raycast_room(dat->raycast_info, &ed->world);
	if (hover != dat->room && hover != NULL)
	{
		highlight_room(ed, sdl, hover, AMBER_2);
		print_text_boxed(sdl, hover->name, point_div(sdl->screensize, 2));
		if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
		{
			dat->held_edge = NULL;
			dat->room = hover;
			dat->rtm = rtm_modify;
			ed->hid.mouse.click_unhandled = false;
		}
	}
}

void	room_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_room_tool_data	*dat;
	t_ray				ray;

	dat = (t_room_tool_data *)ed->tool->tool_data;
	room_tool_raycast(ed, dat);
	if (dat->rtm == rtm_none)
		room_tool_update_nonemode(ed, sdl, dat);
	if (dat->room != NULL && dat->rtm == rtm_create)
		create_mode(ed, sdl, dat);
	if (dat->rtm == rtm_create || dat->rtm == rtm_none)
		update_maingui(ed, sdl, dat);
	if (dat->rtm == rtm_modify && dat->room != NULL)
	{
		modifymode(ed, *sdl, dat);
	}
	if (dat->rtm == rtm_paint)
		room_tool_paint(ed, sdl, dat);
}
