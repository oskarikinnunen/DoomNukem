/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 15:49:59 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/01 13:33:58 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/guntool.h"
#include "doomnukem.h"
#include "objects.h"
#include "file_io.h"

void	gun_tool_draw_offsetgui(t_editor *ed, t_guntooldata *dat)
{
	t_autogui		*gui;

	
}

void	gun_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_guntooldata	*dat;
	t_autogui		*gui;

	dat = (t_guntooldata *)ed->tool->tooldata;

	gui = &dat->maingui;
	gui_start(gui);
	//gui_label("Preset:", gui);
	//gui_string_edit(ed->player.gun->preset_name, gui);
	//gui_label(ed->player.gun->preset_name, gui);
	gui_emptyvertical(10, gui);
	if (gui_button("Offset\xF1", gui))
	{
		dat->offsetgui.hidden = false;
		dat->recoilgui.hidden = true;
		dat->presetgui.hidden = true;
	}
	if (gui_button("Recoil\xF1", gui))
	{
		dat->recoilgui.hidden = false;
		dat->offsetgui.hidden = true;
		dat->presetgui.hidden = true;
	}
	gui_end(gui);

	if (!dat->offsetgui.hidden)
	{
		gui = &dat->offsetgui;
		gui_start(gui);

		print_text_boxed(sdl, "\xD1Player locked\xD1", point_add(point_div(sdl->screensize, 2), (t_point){.x = 20}));
		t_vector3	prev;

		prev = ed->player.gun->stats.holsterpos;
		gui_labeled_vector3_slider("Hip:", &ed->player.gun->stats.holsterpos, 0.1f, gui);
		if (vector3_cmp(ed->player.gun->stats.holsterpos, prev) == false)
			dat->gun_aim = false;

		prev = ed->player.gun->stats.aimpos;
		gui_labeled_vector3_slider("Aim:", &ed->player.gun->stats.aimpos, 0.1f, gui);
		if (vector3_cmp(ed->player.gun->stats.aimpos, prev) == false)
			dat->gun_aim = true;
		ed->hid.input.shoot = false;
		ed->hid.input.aim = dat->gun_aim;
		gui_end(gui);
		ed->player.locked = true;
		moveplayer(&ed->player, &ed->hid.input, &ed->world);
	}
	if (!dat->recoilgui.hidden)
	{
		gui = &dat->recoilgui;
		gui_start(gui);
		gui_labeled_float_slider("Jump y", &ed->player.gun->stats.recoiljump.y, 0.1f, gui);
		gui_labeled_float_slider("Viewjump y", &ed->player.gun->stats.viewrecoil.y, 0.0005f, gui);
		gui_labeled_float_slider("Angle y", &ed->player.gun->stats.recoilangle.y, 0.5f, gui);
		gui_labeled_float_slider("Angle recover", &ed->player.gun->stats.anglerecovery, 0.0001f, gui);
		gui_labeled_bool_edit("Full auto", &ed->player.gun->stats.fullauto, gui);
		gui_labeled_int_slider("Fire delay", &ed->player.gun->stats.firedelay, 1.0f, gui);
		gui_end(gui);
	}
}

void	gun_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_guntooldata	*dat;

	dat = ed->tool->tooldata;
	if (dat->maingui.sdl == NULL)
	{
		dat->maingui = init_gui(sdl, &ed->hid, &ed->player, (t_point){20, 100}, "Gun tool");
		gui_autosize(&dat->maingui);
	}
	if (dat->offsetgui.sdl == NULL)
	{
		dat->offsetgui = init_gui(sdl, &ed->hid, &ed->player, (t_point){20, 20}, "Offset");
		dat->offsetgui.dock = &dat->maingui;
		dat->offsetgui.hidden = true;
		dat->offsetgui.allow_user_hide = true;
		gui_autosize(&dat->offsetgui);
	}
	if (dat->presetgui.sdl == NULL)
	{
		dat->presetgui = init_gui(sdl, &ed->hid, &ed->player, (t_point){20, 20}, "Select preset");
		dat->presetgui.dock = &dat->maingui;
		dat->presetgui.hidden = true;
		dat->presetgui.allow_user_hide = true;
		gui_autosize(&dat->presetgui);
	}
	if (dat->recoilgui.sdl == NULL)
	{
		dat->recoilgui = init_gui(sdl, &ed->hid, &ed->player, (t_point){20, 20}, "Recoil");
		dat->recoilgui.dock = &dat->maingui;
		dat->recoilgui.hidden = true;
		dat->recoilgui.allow_user_hide = true;
		gui_autosize(&dat->recoilgui);
	}
	ed->player.gun->disabled = false;
	ed->player.locked = true;
}

void	gun_tool_cleanup(t_editor *ed, t_sdlcontext *sdl)
{
	ed->player.gun->disabled = true;
}

t_tool	*get_gun_tool()
{
	static t_tool	tool
	= {
		.init = gun_tool_init, .update = gun_tool_update, .cleanup = gun_tool_cleanup
	};
	t_guntooldata	*dat;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_guntooldata));
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
