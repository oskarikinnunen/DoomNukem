/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 15:49:59 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/07 13:57:44 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/guntool.h"
#include "doomnukem.h"
#include "objects.h"

void	list_gun_presets(t_editor *ed, t_sdlcontext sdl, t_point pos)
{
	t_list	*l;
	t_gun	*gun;
	char	str[256];
	int		i;

	l = ed->world.guns;
	//print_text_boxed(&sdl, "Gun presets:", pos, sdl.screensize);
	i = 1;
	while (l != NULL)
	{
		gun = l->content;
		ft_bzero(str, sizeof(str));
		sprintf(str, "%s (model: %s)", gun->preset_name, gun->entity.object_name);
		if (instant_text_button(sdl, &ed->hid.mouse, str, point_add(pos, (t_point){10, i * 20})))
		{
			ed->player.gun = gun;
			ed->player.gun->entity.transform.parent = &ed->player.transform;
			ed->player.gun->readytoshoot = true;
		}
		if (instant_text_button(sdl, &ed->hid.mouse, "-", point_add(pos, (t_point){0, i * 20})))
			list_remove(&ed->world.guns, gun, sizeof(t_gun));
		i++;
		l = l->next;
	}
}

void	save_preset(t_editor *ed, t_sdlcontext sdl)
{
	t_list	*l;
	t_gun	*gun;
	char	str[256] = { };

	l = ed->world.guns;
	while (l != NULL)
	{
		gun = l->content;
		if (ft_strcmp(gun->preset_name, ed->player.gun->preset_name) == 0)
		{
			gun = ed->player.gun;
			sprintf(gun->preset_name, "%s(copy)", gun->preset_name);
			list_push(&ed->world.guns, ed->player.gun, sizeof(t_gun));
			/*sprintf(str, "Overwrote preset %s", gun->preset_name);
			debugconsole_addmessage(&ed->world.debugconsole, str);*/
			return ;
		}
		l = l->next;
	}
	list_push(&ed->world.guns, ed->player.gun, sizeof(t_gun));
	sprintf(str, "Saved new preset %s", ed->player.gun->preset_name);
	debugconsole_addmessage(&ed->world.debugconsole, str);
}

void	gun_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_guntooldata	*dat;
	t_autogui		*gui;

	dat = (t_guntooldata *)ed->tool->tooldata;

	gui = &dat->maingui;
	gui_start(gui);
	gui_label("Preset:", gui);
	gui_string_edit(ed->player.gun->preset_name, gui);
	//gui_label(ed->player.gun->preset_name, gui);
	if (gui_button("Load preset", gui)) {
		dat->presetgui.hidden = false;
		dat->recoilgui.hidden = true;
		dat->offsetgui.hidden = true;
	}
	if (gui_button("Save preset", gui))
	{
		save_preset(ed, *sdl);
		dat->presetgui.hidden = false;
		dat->recoilgui.hidden = true;
		dat->offsetgui.hidden = true;
	}
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

		prev = ed->player.gun->holsterpos;
		gui_labeled_vector3_slider("Hip:", &ed->player.gun->holsterpos, 0.1f, gui);
		if (vector3_cmp(ed->player.gun->holsterpos, prev) == false)
			dat->gun_aim = false;

		prev = ed->player.gun->aimpos;
		gui_labeled_vector3_slider("Aim:", &ed->player.gun->aimpos, 0.1f, gui);
		if (vector3_cmp(ed->player.gun->aimpos, prev) == false)
			dat->gun_aim = true;
		ed->hid.input.shoot = false;
		ed->hid.input.aim = dat->gun_aim;
		gui_end(gui);
		ed->player.locked = true;
		moveplayer(&ed->player, &ed->hid.input, ed->clock);
	}
	if (!dat->recoilgui.hidden)
	{
		gui = &dat->recoilgui;
		gui_start(gui);
		gui_labeled_float_slider("Jump y", &ed->player.gun->recoiljump.y, 0.1f, gui);
		gui_labeled_float_slider("Viewjump y", &ed->player.gun->viewrecoil.y, 0.0005f, gui);
		gui_labeled_float_slider("Angle y", &ed->player.gun->recoilangle.y, 0.5f, gui);
		gui_labeled_float_slider("Angle recover", &ed->player.gun->anglerecovery, 0.0001f, gui);
		gui_labeled_bool_edit("Full auto", &ed->player.gun->fullauto, gui);
		gui_labeled_int_slider("Fire delay", &ed->player.gun->firedelay, 1.0f, gui);
		gui_end(gui);
	}
	if (!dat->presetgui.hidden)
	{
		gui = &dat->presetgui;
		t_list	*l;
		t_gun	*gun;

		l = ed->world.guns;
		gui_start(gui);
		while (l != NULL)
		{
			gun = l->content;
			if (ft_strcmp(gun->preset_name, ed->player.gun->preset_name) == 0)
				gui_highlighted_button(gun->preset_name, gui);
			else
			{
				gui_starthorizontal(gui);
				if (gui_button(gun->preset_name, gui))
					ed->player.gun = l->content;
				if (gui_button("X", gui))
				{
					list_remove(&ed->world.guns, l->content, sizeof(t_gun));
					gui_endhorizontal(gui);
					gui_end(gui);
					return ;
				}
					
				gui_endhorizontal(gui);
			}
			l = l->next;
		}
		gui_end(gui);
	}
	/*if (instant_text_button(*sdl, &ed->hid.mouse, "Model", (t_point) {20, 140}))
		dat->gtm = gtm_model;
	if (instant_text_button(*sdl, &ed->hid.mouse, "Offset", (t_point) {80, 140}))
		dat->gtm = gtm_offset;
	if (instant_text_button(*sdl, &ed->hid.mouse, "Recoil", (t_point) {140, 140}))
		dat->gtm = gtm_recoil;*/
	/*if (dat->gtm == gtm_model)
	{
		ed->player.gun->entity.obj = object_selector2(ed, sdl, &ed->player.gun->entity);
	}*/
	/*if (dat->gtm == gtm_offset)
	{
		vector3_slider("aiming  position", &ed->player.gun->aimpos, ed, sdl, (t_point) {20, sdl.window_h / 2 });
		vector3_slider("holster position", &ed->player.gun->holsterpos, ed, sdl, (t_point) {20, (sdl.window_h / 2) + 60});
	}
	if (dat->gtm == gtm_recoil)
	{
		float_slider_name("angle y", &ed->player.gun->recoilangle.y, 0.5f, ed, sdl, (t_point) {20, 200});
		float_slider_name("jump y", &ed->player.gun->recoiljump.y ,0.1f, ed, sdl, (t_point) {20, 220});
		float_slider_name("viewjump y", &ed->player.gun->viewrecoil.y , 0.0005f, ed, sdl, (t_point) {20, 240});
		float_slider_name("angle recovery", &ed->player.gun->anglerecovery, 0.0001f, ed, sdl, (t_point) {20, 260});
		bool_toggle("auto", &ed->player.gun->fullauto, ed, sdl, (t_point) {20, (sdl.window_h / 2) + 100});
		int_slider_name("firedelay", &ed->player.gun->firedelay, 15, ed, sdl, (t_point) {20, (sdl.window_h / 2) + 140});
	}
	string_box("Preset name:", ed->player.gun->preset_name, ed, sdl, (t_point) {20, 80});
	if (instant_text_button(sdl, &ed->hid.mouse, "Save", (t_point){20, 100}))
		save_preset(ed, sdl);
	list_gun_presets(ed, sdl, (t_point){20, 540});*/
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
