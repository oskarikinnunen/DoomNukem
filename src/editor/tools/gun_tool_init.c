/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_tool_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 21:15:46 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 22:28:57 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/guntool.h"
#include "doomnukem.h"

static void	init_gun_main_gui(t_guntooldata *dat,
								t_editor *ed, t_sdlcontext *sdl)
{
	dat->maingui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
							(t_point){20, 100}, "Gun tool"});
	gui_autosize(&dat->maingui);
}

static void	init_gun_offset_gui(t_guntooldata *dat,
								t_editor *ed, t_sdlcontext *sdl)
{
	dat->offsetgui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
								(t_point){20, 20}, "Offset"});
	dat->offsetgui.dock = &dat->maingui;
	dat->offsetgui.hidden = true;
	dat->offsetgui.allow_user_hide = true;
	gui_autosize(&dat->offsetgui);
}

static void	init_gun_preset_gui(t_guntooldata *dat,
								t_editor *ed, t_sdlcontext *sdl)
{
	dat->presetgui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
								(t_point){20, 20}, "Select preset"});
	dat->presetgui.dock = &dat->maingui;
	dat->presetgui.hidden = true;
	dat->presetgui.allow_user_hide = true;
	gui_autosize(&dat->presetgui);
}

static void	init_gun_recoil_gui(t_guntooldata *dat,
								t_editor *ed, t_sdlcontext *sdl)
{
	dat->recoilgui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player, \
								(t_point){20, 20}, "Recoil"});
	dat->recoilgui.dock = &dat->maingui;
	dat->recoilgui.hidden = true;
	dat->recoilgui.allow_user_hide = true;
	gui_autosize(&dat->recoilgui);
}

void	gun_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_guntooldata	*dat;

	dat = ed->tool->tooldata;
	if (dat->maingui.sdl == NULL)
		init_gun_main_gui(dat, ed, sdl);
	if (dat->offsetgui.sdl == NULL)
		init_gun_offset_gui(dat, ed, sdl);
	if (dat->presetgui.sdl == NULL)
		init_gun_preset_gui(dat, ed, sdl);
	if (dat->recoilgui.sdl == NULL)
		init_gun_recoil_gui(dat, ed, sdl);
	ed->player.gun->disabled = false;
	ed->player.locked = true;
}
