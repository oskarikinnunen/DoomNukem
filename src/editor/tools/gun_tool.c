/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 15:49:59 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/06 17:53:15 by okinnune         ###   ########.fr       */
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
			sprintf(str, "Overwrote preset %s", gun->preset_name);
			debugconsole_addmessage(&ed->world.debugconsole, str);
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

	dat = (t_guntooldata *)ed->tool->tooldata;
	ed->player.gun->disabled = false;
	if (instant_text_button(*sdl, &ed->hid.mouse, "Model", (t_point) {20, 140}))
		dat->gtm = gtm_model;
	if (instant_text_button(*sdl, &ed->hid.mouse, "Offset", (t_point) {80, 140}))
		dat->gtm = gtm_offset;
	if (instant_text_button(*sdl, &ed->hid.mouse, "Recoil", (t_point) {140, 140}))
		dat->gtm = gtm_recoil;
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

void	gun_tool_cleanup(t_editor *ed, t_sdlcontext *sdl)
{
	ed->player.gun->disabled = true;
}

t_tool	*get_gun_tool()
{
	static t_tool	tool
	= {
		.update = gun_tool_update, .cleanup = gun_tool_cleanup
	};
	t_guntooldata	*dat;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_guntooldata));
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
