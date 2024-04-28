/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_tool_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 21:15:46 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:12:47 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/guntool.h"
#include "doomnukem.h"

static void	init_gun_main_gui(t_gun_tool_data *dat,
								t_editor *ed, t_sdlcontext *sdl)
{
	dat->main_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
							(t_point){20, 100}, "Gun tool"});
	gui_autosize(&dat->main_gui);
}

static void	init_gun_offset_gui(t_gun_tool_data *dat,
								t_editor *ed, t_sdlcontext *sdl)
{
	dat->offset_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
								(t_point){20, 20}, "Offset"});
	dat->offset_gui.dock = &dat->main_gui;
	dat->offset_gui.hidden = true;
	dat->offset_gui.allow_user_hide = true;
	gui_autosize(&dat->offset_gui);
}

static void	init_gun_preset_gui(t_gun_tool_data *dat,
								t_editor *ed, t_sdlcontext *sdl)
{
	dat->preset_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
								(t_point){20, 20}, "Select preset"});
	dat->preset_gui.dock = &dat->main_gui;
	dat->preset_gui.hidden = true;
	dat->preset_gui.allow_user_hide = true;
	gui_autosize(&dat->preset_gui);
}

static void	init_gun_recoil_gui(t_gun_tool_data *dat,
								t_editor *ed, t_sdlcontext *sdl)
{
	dat->recoil_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
								(t_point){20, 20}, "Recoil"});
	dat->recoil_gui.dock = &dat->main_gui;
	dat->recoil_gui.hidden = true;
	dat->recoil_gui.allow_user_hide = true;
	gui_autosize(&dat->recoil_gui);
}

void	gun_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_gun_tool_data	*dat;

	dat = ed->tool->tool_data;
	if (dat->main_gui.sdl == NULL)
		init_gun_main_gui(dat, ed, sdl);
	if (dat->offset_gui.sdl == NULL)
		init_gun_offset_gui(dat, ed, sdl);
	if (dat->preset_gui.sdl == NULL)
		init_gun_preset_gui(dat, ed, sdl);
	if (dat->recoil_gui.sdl == NULL)
		init_gun_recoil_gui(dat, ed, sdl);
	ed->player.gun->disabled = false;
	ed->player.locked = true;
}
