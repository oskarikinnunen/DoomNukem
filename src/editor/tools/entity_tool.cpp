/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:15:41 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tools/entitytool.h" 
#include "objects.h"
#include "raycast.h"

void	find_bounds(t_entity *ent)
{
	t_bound		zbound;
	t_vector3	v;
	int			i;

	if (ent->obj == NULL)
		return ;
	zbound.max = -10000.0f;
	zbound.min = 10000.0f;
	i = 0;
	while (i < ent->obj->vertice_count)
	{
		v = ent->obj->vertices[i];
		if (v.z < zbound.min)
			zbound.min = v.z;
		if (v.z > zbound.max)
			zbound.max = v.z;
		i++;
	}
	ent->z_bound = zbound;
}

static void	entitytool_objectgui_update(t_entity_tool_data *dat,
										t_world *world)
{
	t_object	*obj;
	t_autogui	*gui;
	int			i;

	gui = &dat->new_object_gui;
	gui_start(gui);
	i = 0;
	gui_label("Selected:", gui);
	if (dat->new_ent != NULL && ft_strlen((dat->new_ent)->object_name.str) > 0)
		gui_label((dat->new_ent)->object_name.str, gui);
	else
		gui_empty_vertical(20, gui);
	gui_empty_vertical(20, gui);
	while (i < gui->sdl->objectcount)
	{
		obj = &gui->sdl->objects[i];
		if (gui_button(obj->name, gui))
		{
			entity_assign_object(world, dat->new_ent, obj);
		}
		i++;
	}
	gui_end(gui);
}

void	entity_tool_move_entity(t_editor *ed, t_entity *entity,
								t_entity_tool_data *dat)
{
	find_bounds(entity);
	entity->transform.position = dat->info.hit_pos;
	entity->transform.position.z
		-= entity->z_bound.min * entity->transform.scale.z;
	entity->transform.rotation.x
		+= ed->hid.mouse.scroll_delta * 0.261799388f;
}

void	entity_tool_place(t_editor *ed, t_sdlcontext *sdl,
						t_entity_tool_data *dat)
{
	t_entity	*went;

	if (dat->sel_ent != NULL || vector3_cmp(dat->info.hit_pos, vector3_zero()))
		return ;
	entitytool_objectgui_update(dat, &ed->world);
	if (dat->new_ent != NULL && dat->new_ent->obj != NULL)
	{
		entity_tool_move_entity(ed, dat->new_ent, dat);
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT)
		&& ed->hid.mouse.relative && !dat->new_ent->hidden)
	{
		if (dat->new_ent->obj != NULL)
		{
			went = spawn_entity(&ed->world, dat->new_ent->obj);
			went->transform = dat->new_ent->transform;
		}
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
		dat->new_ent->hidden = false;
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		dat->new_ent->hidden = true;
}

void	entity_tool_raycast(t_editor *ed, t_sdlcontext *sdl,
							t_entity_tool_data *dat)
{
	t_ray	ray;

	ray.origin = ed->player.head_position;
	ray.dir = ed->player.look_dir;
	ft_bzero(&dat->info, sizeof(t_raycast_info));
	if (raycast(ray, &dat->info, &ed->world))
	{
		if (dat->info.hit_entity != NULL
			&& !dat->info.hit_entity->rigid)
			highlight_entity(sdl, dat->info.hit_entity, AMBER_4);
		else
			dat->info.hit_entity = NULL;
	}
	else
		raycast_plane(ray, &dat->info, 0.0f);
}
