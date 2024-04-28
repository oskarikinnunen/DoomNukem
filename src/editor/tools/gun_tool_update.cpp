/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_tool_update.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 21:16:21 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:15:41 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/guntool.h"
#include "doomnukem.h"
#include "objects.h"
#include "file_io.h"

static void	update_gun_offset_gui(t_gun_tool_data *dat,
									t_editor *ed, t_sdlcontext *sdl)
{
	t_autogui	*gui;
	t_vector3	prev;

	gui = &dat->offset_gui;
	gui_start(gui);
	print_text_boxed(sdl, "\xD1Player locked\xD1", \
			point_add(point_div(sdl->screensize, 2), (t_point){.x = 20}));
	prev = ed->player.gun->stats.holster_pos;
	gui_labeled_vector3_slider("Hip:", \
			&ed->player.gun->stats.holster_pos, 0.1f, gui);
	if (vector3_cmp(ed->player.gun->stats.holster_pos, prev) == false)
		dat->gun_aim = false;
	prev = ed->player.gun->stats.aim_pos;
	gui_labeled_vector3_slider("Aim:", \
			&ed->player.gun->stats.aim_pos, 0.1f, gui);
	if (vector3_cmp(ed->player.gun->stats.aim_pos, prev) == false)
		dat->gun_aim = true;
	ed->hid.input.shoot = false;
	ed->hid.input.aim = dat->gun_aim;
	gui_end(gui);
	ed->player.locked = true;
	moveplayer(&ed->player, &ed->hid.input, &ed->world);
}

static void	update_gun_recoil_gui(t_gun_tool_data *dat, t_editor *ed)
{
	t_autogui	*gui;

	gui = &dat->recoil_gui;
	gui_start(gui);
	gui_labeled_float_slider("Jump y", \
			&ed->player.gun->stats.recoil_jump.y, 0.1f, gui);
	gui_labeled_float_slider("Viewjump y", \
			&ed->player.gun->stats.view_recoil.y, 0.0005f, gui);
	gui_labeled_float_slider("Angle y", \
			&ed->player.gun->stats.recoil_angle.y, 0.5f, gui);
	gui_labeled_float_slider("Angle recover", \
			&ed->player.gun->stats.angle_recovery, 0.0001f, gui);
	gui_labeled_bool_edit("Full auto", \
			&ed->player.gun->stats.full_auto, gui);
	gui_labeled_int_slider("Fire delay", \
			(int32_t *)&ed->player.gun->stats.fire_delay, 1.0f, gui);
	gui_end(gui);
}

void	gun_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_gun_tool_data	*dat;
	t_autogui		*gui;

	dat = (t_gun_tool_data *)ed->tool->tool_data;
	gui = &dat->main_gui;
	gui_start(gui);
	gui_empty_vertical(10, gui);
	if (gui_button("Offset\xF1", gui))
	{
		dat->offset_gui.hidden = false;
		dat->recoil_gui.hidden = true;
		dat->preset_gui.hidden = true;
	}
	if (gui_button("Recoil\xF1", gui))
	{
		dat->recoil_gui.hidden = false;
		dat->offset_gui.hidden = true;
		dat->preset_gui.hidden = true;
	}
	gui_end(gui);
	if (!dat->offset_gui.hidden)
		update_gun_offset_gui(dat, ed, sdl);
	if (!dat->recoil_gui.hidden)
		update_gun_recoil_gui(dat, ed);
}
