/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/22 16:48:54 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tools/entitytool.h" 
#include "objects.h"

char	*floatstr(float f)
{
	static char	final[8];
	char	*strinteger;
	char	*strdecimal;
	int		neg;
	int		i;

	i = (int)f;
	f = f - i;
	int dec = 1;
	f = f * (ft_pow(10, dec));
	neg = (f < 0.0f);
	strinteger = ft_itoa(ft_abs(i));
	strdecimal = ft_itoa(ft_abs((int)f));
	//final = ft_strnew(ft_strlen(strinteger) + ft_strlen(strdecimal) + 1 + neg);
	ft_bzero(final, sizeof(final));
	if (neg)
		ft_strcat(final, "-");
	ft_strcat(final, strinteger);
	ft_strcat(final, ",");
	ft_strcat(final, strdecimal);
	free(strinteger);
	free(strdecimal);
	return (final);
}

static t_img	black_image()
{
	static t_img	black;

	black.data = (uint32_t[1]) {0};
	black.length = 1;
	black.size = point_one();
	return (black);
}

static void findbounds(t_entity *ent)
{
	t_bound		zbound;
	t_vector3	v;
	int			i;

	zbound.max = -10000.0f;
	zbound.min= 10000.0f;
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

t_entity *selected_entity(t_editor *ed, t_sdlcontext sdl)
{
	t_entitycache	*cache;
	t_entity		*entity;
	int				i;
	int				found;

	
	cache = &ed->world.entitycache;
	i = 0;
	found = 0;
	while (found < cache->existing_entitycount && i < cache->alloc_count)
	{
		entity = &cache->entities[i];
		if (entity->status != es_free)
		{
			if (!entity->uneditable && entity_lookedat(ed, sdl, entity))
			{
				return (entity);
			}
			found++;
		}
		i++;
	}
	//TODO: error log if found < existing_entitycount
	return (NULL);
}

static void	entity_tool_lazyinit(t_editor *ed, t_sdlcontext *sdl, t_entitytooldata *dat)
{
	if (dat->objectgui.gui.sdl == NULL)
	{
		dat->objectgui.gui = init_gui(sdl, &ed->hid, &ed->player, (t_point) {20, 40}, "Place new entity");
		dat->objectgui.gui.minimum_size.x = 140;
		dat->objectgui.gui.rect.size.x = dat->objectgui.gui.minimum_size.x;
		dat->objectgui.autoclose = false;
	}
	if (dat->entitygui.sdl == NULL)
	{
		dat->entitygui = init_gui(sdl, &ed->hid, &ed->player, (t_point) {20, 40}, "Edit entity");
		dat->entitygui.minimum_size.x = 300;
		dat->entitygui.minimum_size.y = 550;
		dat->entitygui.rect.size.y = 550;
	}
}

static int find_object_index(t_sdlcontext *sdl, t_entity *ent)
{
	int	i;

	i = 0;
	while (i < sdl->objectcount)
	{
		if (&sdl->objects[i] == ent->obj)
			return (i);
		i++;
	}
	return (0);
}

void	entity_tool_place(t_editor *ed, t_sdlcontext *sdl, t_entitytooldata *dat)
{
	if (dat->sel_ent != NULL)
		return ;
	objectgui_update(&dat->objectgui, &dat->ent);
	if (dat->ent != NULL)
	{
		dat->entitygui.dock = &dat->objectgui.gui;
		dat->entitygui.hidden = false;
		gui_start(&dat->entitygui);
		gui_preset_scale_and_rotate(&dat->ent->transform, &dat->entitygui);
		gui_labeled_int("Object triangles: ", dat->ent->obj->face_count, &dat->entitygui);
		gui_end(&dat->entitygui);
		findbounds(dat->ent);
		dat->ent->transform.position = raycast(ed);//vector3_movetowards(ent->transform.position, dir, ed->clock.delta * 1.0f);
		dat->ent->transform.position.z -= dat->ent->z_bound.min * dat->ent->transform.scale.z;
		dat->ent->transform.rotation.x += ed->hid.mouse.scroll_delta * 0.25f;
		ed->render.wireframe = true;
		ed->render.gizmocolor = AMBER_3;
		render_entity(sdl, &ed->render, dat->ent);
		ed->render.wireframe = false;
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT) && ed->hid.mouse.relative && dat->ent != NULL)
	{
		t_entity *went = spawn_entity(&ed->world);
		entity_assign_object(&ed->world, went, dat->ent->obj);
		/*went->obj = dat->ent->obj;*/
		ft_strcpy(went->object_name, dat->ent->object_name);
		went->transform = dat->ent->transform;
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT) && dat->ent != NULL)
	{
		free(dat->ent);
		dat->ent = NULL;
	}
}

void	entity_tool_modify(t_editor *ed, t_sdlcontext *sdl, t_entitytooldata *dat)
{
	t_entity	*hover;
	t_entity	*ent;
	t_autogui	*gui;

	if (dat->ent != NULL)
		return ;
	hover = NULL;
	hover = selected_entity(ed, *sdl);
	if (hover != NULL)
	{
		ed->render.wireframe = true;
		ed->render.gizmocolor = AMBER_1;
		render_entity(sdl, &ed->render, hover);
		ed->render.wireframe = false;
		if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
			dat->sel_ent = hover;
	}
	if (dat->sel_ent != NULL)
	{
		gui = &dat->entitygui;
		ent = dat->sel_ent;
		ed->render.wireframe = true;
		ed->render.gizmocolor = AMBER_3;
		render_entity(sdl, &ed->render, ent);
		ed->render.wireframe = false;

		gui_start(gui);
		gui_preset_transform(&ent->transform, gui);
		gui_emptyvertical(15, gui);
		gui_labeled_bool_edit("Dynamic lighting: ", &ent->lightmap->dynamic, gui);
		gui_labeled_int("Cur light: ", ent->lightmap->dynamic_data, gui);
		if (gui_button("Reset rotation", gui))
			ent->transform.rotation = vector3_zero();
		if (gui_button("Reset scale", gui))
			ent->transform.scale = vector3_one();
		if (gui_button("Snap to floor", gui))
		{
			ent->transform.position.z = 0;
			ent->transform.position.z -= ent->z_bound.min * ent->transform.scale.z;
		}
		if (gui_button("Delete", gui))
		{
			destroy_entity(&ed->world, ent);
			dat->sel_ent = NULL;
			gui_end(gui);
			return ;
		}
		gui_end(gui);
		if (ent == hover && ed->hid.mouse.held == MOUSE_LEFT && ed->hid.mouse.relative)
			dat->grabbing = true;
		if ((ed->hid.mouse.held == 0 || !ed->hid.mouse.relative) || dat->entitygui.locking_player)
			dat->grabbing = false;
		if (dat->grabbing)
		{
			ent->transform.position = raycast(ed);
			ent->transform.position.z -= ent->z_bound.min * ent->transform.scale.z;
			ent->transform.rotation.x += ed->hid.mouse.scroll_delta * 0.25f;
		}
		if ((ed->hid.keystate >> KEYS_DELETEMASK) & 1)
		{
			destroy_entity(&ed->world, ent);
			dat->sel_ent = NULL;
		}
		if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		{
			dat->sel_ent = NULL;
		}
	}
}

void	entity_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_entitytooldata	*dat;
	t_entity			*collide;

	dat = (t_entitytooldata *)ed->tool->tooldata;
	//entity_tool_lazyinit(ed, sdl, dat);
	entity_tool_place(ed, sdl, dat);
	entity_tool_modify(ed, sdl, dat);
}

void	entity_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_entitytooldata	*dat;

	dat = ed->tool->tooldata;
	if (dat->objectgui.gui.sdl == NULL)
	{
		dat->objectgui.gui = init_gui(sdl, &ed->hid, &ed->player, (t_point) {20, 40}, "Place new entity");
		dat->objectgui.gui.minimum_size.x = 140;
		dat->objectgui.gui.rect.size.x = dat->objectgui.gui.minimum_size.x;
		dat->objectgui.autoclose = false;
	}
	if (dat->entitygui.sdl == NULL)
	{
		dat->entitygui = init_gui(sdl, &ed->hid, &ed->player, (t_point) {20, 40}, "Edit entity");
		dat->entitygui.minimum_size.x = 300;
		dat->entitygui.minimum_size.y = 450;
		//dat->entitygui.rect.size.y = 220;
	}
}

t_tool	*get_entity_tool()
{
	static t_tool	tool
	= {
		.update = entity_tool_update,
		.init	= entity_tool_init
	};
	t_entitytooldata	*dat; //TODO: make entity tool use it's own tooldata struct,

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_entitytooldata));
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
		dat = (t_entitytooldata *)tool.tooldata;
		dat->ent = NULL;
	}
	ft_strcpy(tool.icon_name, "entitytool.png");
	return (&tool);
}
