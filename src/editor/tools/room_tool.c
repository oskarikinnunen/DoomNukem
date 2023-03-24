/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:32:36 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 17:55:36 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

static t_line	edgeline_to_line(t_edgeline edgeline)
{
	return ((t_line){.start = *edgeline.start, .end = *edgeline.end});
}

static void	room_tool_remove_room(t_editor *ed, t_roomtooldata *dat)
{
	t_area	temp;

	ft_bzero(&temp, sizeof(t_area));
	ft_memcpy(temp.edges, dat->room->edges,
		sizeof(t_vector2) * dat->room->edgecount);
	temp.edgecount = dat->room->edgecount;
	world_remove_room(&ed->world, dat->room);
	room_recalculate_joined_rooms(&ed->world, &temp);
	dat->room = NULL;
	dat->rtm = rtm_none;
}

void	update_maingui(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui	*gui;

	gui = &dat->maingui;
	gui_start(gui);
	if (gui_shortcut_button("New room", 'N', gui))
	{
		dat->room = ft_memalloc(sizeof(t_area));
		ft_strncpy_term(dat->room->s_floortex.str, "concrete02.cng", 60);
		ft_strncpy_term(dat->room->s_ceiltex.str, "concrete02.cng", 60);
		dat->rtm = rtm_create;
		room_setpreset_room(dat->room);
		force_mouselock(&ed->hid);
	}
	if (gui_shortcut_button("Paint rooms", 'P', gui))
		dat->rtm = rtm_paint;
	gui_end(gui);
}

void	room_tool_update_nonemode(t_editor *ed,
		t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_area	*hover;

	hover = NULL;
	hover = get_raycast_room(dat->raycastinfo, &ed->world);
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
	t_roomtooldata	*dat;
	t_ray			ray;

	dat = (t_roomtooldata *)ed->tool->tooldata;
	room_tool_raycast(ed, dat);
	if (dat->rtm == rtm_none)
		room_tool_update_nonemode(ed, sdl, dat);
	if (dat->room != NULL && dat->rtm == rtm_create)
		createmode(ed, sdl, dat);
	update_maingui(ed, sdl, dat);
	if (dat->rtm == rtm_modify && dat->room != NULL)
	{
		modifymode(ed, *sdl, dat);
	}
	if (dat->rtm == rtm_paint)
		room_tool_paint(ed, sdl, dat);
}
