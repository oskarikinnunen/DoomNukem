/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_tool_update.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 21:16:21 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 22:31:05 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/guntool.h"
#include "doomnukem.h"
#include "objects.h"
#include "file_io.h"

static void	update_gun_offset_gui(t_guntooldata *dat,
									t_editor *ed, t_sdlcontext *sdl)
{
	t_autogui	*gui;
	t_vector3	prev;

	gui = &dat->offsetgui;
	gui_start(gui);
	print_text_boxed(sdl, "\xD1Player locked\xD1", \
			point_add(point_div(sdl->screensize, 2), (t_point){.x = 20}));
	prev = ed->player.gun->stats.holsterpos;
	gui_labeled_vector3_slider("Hip:", \
			&ed->player.gun->stats.holsterpos, 0.1f, gui);
	if (vector3_cmp(ed->player.gun->stats.holsterpos, prev) == false)
		dat->gun_aim = false;
	prev = ed->player.gun->stats.aimpos;
	gui_labeled_vector3_slider("Aim:", \
			&ed->player.gun->stats.aimpos, 0.1f, gui);
	if (vector3_cmp(ed->player.gun->stats.aimpos, prev) == false)
		dat->gun_aim = true;
	ed->hid.input.shoot = false;
	ed->hid.input.aim = dat->gun_aim;
	gui_end(gui);
	ed->player.locked = true;
	moveplayer(&ed->player, &ed->hid.input, &ed->world);
}

static void	update_gun_recoil_gui(t_guntooldata *dat, t_editor *ed)
{
	t_autogui	*gui;

	gui = &dat->recoilgui;
	gui_start(gui);
	gui_labeled_float_slider("Jump y", \
			&ed->player.gun->stats.recoiljump.y, 0.1f, gui);
	gui_labeled_float_slider("Viewjump y", \
			&ed->player.gun->stats.viewrecoil.y, 0.0005f, gui);
	gui_labeled_float_slider("Angle y", \
			&ed->player.gun->stats.recoilangle.y, 0.5f, gui);
	gui_labeled_float_slider("Angle recover", \
			&ed->player.gun->stats.anglerecovery, 0.0001f, gui);
	gui_labeled_bool_edit("Full auto", \
			&ed->player.gun->stats.fullauto, gui);
	gui_labeled_int_slider("Fire delay", \
			(int32_t *)&ed->player.gun->stats.firedelay, 1.0f, gui);
	gui_end(gui);
}

void	gun_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_guntooldata	*dat;
	t_autogui		*gui;

	dat = (t_guntooldata *)ed->tool->tooldata;
	gui = &dat->maingui;
	gui_start(gui);
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
		update_gun_offset_gui(dat, ed, sdl);
	if (!dat->recoilgui.hidden)
		update_gun_recoil_gui(dat, ed);
}
