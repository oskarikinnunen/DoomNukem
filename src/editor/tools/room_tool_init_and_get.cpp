/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_init_and_get.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 17:07:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 23:10:03 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

static void	room_tool_init_other_guis(t_editor *ed, t_sdlcontext *sdl,
	t_room_tool_data *dat)
{
	if (dat->modroom_gui.sdl == NULL)
	{
		dat->modroom_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
						(t_point){20, sdl->screensize.y / 2}, "Edit room"});
		dat->modroom_gui.minimum_size = (t_point){240, 250};
		dat->modroom_gui.rect.size = dat->modroom_gui.minimum_size;
	}
	if (dat->paint_gui.sdl == NULL)
		dat->paint_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
						(t_point){20, 80}, "Paint rooms"});
	if (dat->edge_gui.sdl == NULL)
		dat->paint_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
						(t_point){60, 140}, "Edge"});
}

static void	room_tool_init_mainguis(t_editor *ed, t_sdlcontext *sdl,
		t_room_tool_data *dat)
{
	if (dat->maingui.sdl == NULL)
	{
		dat->maingui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
						(t_point){20, 80}, "Room tool"});
		gui_autosize(&dat->maingui);
		dat->maingui.minimum_size.x = 140;
	}
	if (dat->newroom_gui.sdl == NULL)
	{
		dat->newroom_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
						(t_point){20, 80}, "New room"});
		dat->newroom_gui.dock = &dat->maingui;
		gui_autosize(&dat->newroom_gui);
	}
}

void	room_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_room_tool_data	*dat;

	dat = ed->tool->tool_data;
	room_tool_init_mainguis(ed, sdl, dat);
	room_tool_init_other_guis(ed, sdl, dat);
}

t_tool	*get_room_tool(void)
{
	static t_tool		tool
		= {
		.update = room_tool_update,
		.init = room_tool_init
	};
	t_room_tool_data	*dat;

	if (tool.tool_data == NULL)
	{
		tool.tool_data = prot_memalloc(sizeof(t_room_tool_data));
		dat = (t_room_tool_data *)tool.tool_data;
		dat->room = NULL;
	}
	return (&tool);
}
