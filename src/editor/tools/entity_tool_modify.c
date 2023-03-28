/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool_modify.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 15:02:24 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 11:48:05 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tools/entitytool.h" 
#include "objects.h"

static void	entity_gui_header(t_entity *ent, t_autogui *gui,
							t_world *world, t_entity_tool_data *dat)
{
	t_entity	*clone;

	gui_start_horizontal(gui);
	gui_label("ID: ", gui);
	gui_label(s_itoa(ent->id), gui);
	gui_end_horizontal(gui);
	if (gui_shortcut_button("Clone", 'c', gui))
	{
		clone = spawn_entity(world, ent->obj);
		clone->component.type = ent->component.type;
		entity_set_component_functions(clone, world);
		clone->transform.position = ent->transform.position;
		dat->sel_ent = clone;
	}
}

static void	entity_gui_component(t_entity *ent,
						t_autogui *gui, t_world *world, t_entity_tool_data *dat)
{
	if (gui_highlighted_button_if("Edit component", gui,
			dat->entity_ed.component_toggle))
		dat->entity_ed.component_toggle = !dat->entity_ed.component_toggle;
	if (dat->entity_ed.component_toggle)
	{
		gui_entity_mode(ent, gui, world);
		if (ent->component.type != COMP_NONE)
		{
			gui_start(&dat->entity_ed.component_gui);
			gui_component(ent, &dat->entity_ed.component_gui, world);
			gui_end(&dat->entity_ed.component_gui);
		}
	}
}

static void	entity_gui_transform_and_component(t_entity *ent,
						t_autogui *gui, t_world *world, t_entity_tool_data *dat)
{
	if (gui_highlighted_button_if("Edit transform", gui,
			dat->entity_ed.tr_toggle))
		dat->entity_ed.tr_toggle = !dat->entity_ed.tr_toggle;
	if (dat->entity_ed.tr_toggle)
	{
		gui_preset_transform(&ent->transform, gui);
		gui_empty_vertical(15, gui);
		if (gui_button("Reset rotation", gui))
			ent->transform.rotation = vector3_zero();
		if (gui_button("Reset scale", gui))
			ent->transform.scale = vector3_one();
	}
}

static void	entity_grab_and_delete(t_entity *ent, t_autogui *gui,
								t_editor *ed, t_entity_tool_data *dat)
{
	if (ent == dat->info.hit_entity && ed->hid.mouse.held == MOUSE_LEFT
		&& ed->hid.mouse.relative && !ed->world.player->locked)
	{
		dat->grabbing = true;
		ent->ignore_raycasts = true;
	}
	if ((ed->hid.mouse.held == 0 || !ed->hid.mouse.relative)
		|| gui->locking_player)
	{
		ent->ignore_raycasts = false;
		dat->grabbing = false;
	}
	if (dat->grabbing && !vector3_cmp(dat->info.hit_pos, vector3_zero()))
	{
		find_bounds(ent);
		ent->transform.position = dat->info.hit_pos;
		ent->transform.position.z -= ent->z_bound.min * ent->transform.scale.z;
		ent->transform.rotation.x
			+= ed->hid.mouse.scroll_delta * ft_degtorad(5.0f);
	}
	if ((ed->hid.keystate >> KEYS_DELETEMASK) & 1)
	{
		destroy_entity(&ed->world, ent);
		dat->sel_ent = NULL;
	}
}

//Only goes here if new_ent is NULL
void	entity_tool_modify(t_editor *ed, t_sdlcontext *sdl,
		t_entity_tool_data *dat)
{
	t_entity	*ent;
	t_autogui	*gui;

	if (dat->info.hit_entity != NULL
		&& mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
		dat->sel_ent = dat->info.hit_entity;
	if (dat->sel_ent != NULL)
	{
		gui = &dat->entity_ed.gui;
		ent = dat->sel_ent;
		highlight_entity(sdl, ent, AMBER_3);
		gui_start(gui);
		entity_gui_header(ent, gui, &ed->world, dat);
		entity_gui_transform_and_component(ent, gui, &ed->world, dat);
		entity_gui_component(ent, gui, &ed->world, dat);
		gui_end(gui);
		if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		{
			dat->sel_ent->ignore_raycasts = false;
			dat->sel_ent = NULL;
			dat->new_ent->hidden = false;
			return ;
		}
		entity_grab_and_delete(ent, gui, ed, dat);
	}
}
