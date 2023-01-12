/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/12 16:20:25 by okinnune         ###   ########.fr       */
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
			if (!entity->rigid && entity_lookedat(ed, sdl, entity))
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
	if (dat->sel_ent != NULL || vector3_cmp(dat->info.hit_pos, vector3_zero()))
		return ;
	objectgui_update(&dat->objectgui, &dat->ent);
	if (dat->ent != NULL)
	{
		dat->entitygui.hidden = false;
		gui_start(&dat->entitygui);
		gui_preset_scale_and_rotate(&dat->ent->transform, &dat->entitygui);
		gui_labeled_int("Object triangles: ", dat->ent->obj->face_count, &dat->entitygui);
		gui_end(&dat->entitygui);
		findbounds(dat->ent);
		dat->ent->transform.position = dat->info.hit_pos;
		//dat->ent->transform.position = raycast(ed);//vector3_movetowards(ent->transform.position, dir, ed->clock.delta * 1.0f);
		dat->ent->transform.position.z -= dat->ent->z_bound.min * dat->ent->transform.scale.z;
		dat->ent->transform.rotation.x += ed->hid.mouse.scroll_delta * 0.25f;
		sdl->render.wireframe = true;
		sdl->render.gizmocolor = AMBER_3;
		render_entity(sdl, &sdl->render, dat->ent);
		sdl->render.wireframe = false;
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT) && ed->hid.mouse.relative && dat->ent != NULL)
	{
		t_entity *went = spawn_entity(&ed->world);
		entity_assign_object(&ed->world, went, dat->ent->obj);
		/*went->obj = dat->ent->obj;*/
		ft_strcpy(went->object_name, dat->ent->object_name); //TODO: fix in world save or in assign object
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
	//t_entity	*hover;
	t_entity	*ent;
	t_autogui	*gui;

	if (dat->ent != NULL)
		return ;
	/*hover = NULL;
	hover = selected_entity(ed, *sdl);
	if (hover != NULL)
	{
		sdl->render.wireframe = true;
		sdl->render.gizmocolor = AMBER_1;
		render_entity(sdl, &sdl->render, hover);
		sdl->render.wireframe = false;
		if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
			dat->sel_ent = hover;
	}*/
	if (dat->info.hit_entity != NULL &&
		!dat->info.hit_entity->rigid && mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		dat->sel_ent = dat->info.hit_entity;
		//dat->sel_ent->ignore_raycasts = true;
	}
	if (dat->sel_ent != NULL)
	{
		gui = &dat->entitygui;
		ent = dat->sel_ent;
		sdl->render.wireframe = true;
		sdl->render.gizmocolor = AMBER_3;
		render_entity(sdl, &sdl->render, ent);
		sdl->render.wireframe = false;

		gui_start(gui);
		gui_preset_transform(&ent->transform, gui);
		gui_emptyvertical(15, gui);
		if (ent->lightmap != NULL)
		{
			gui_labeled_bool_edit("Dynamic lighting: ", &ent->lightmap->dynamic, gui);
			gui_labeled_int("Cur light: ", ent->lightmap->dynamic_data, gui);
		}
		if (gui_button("Reset rotation", gui))
			ent->transform.rotation = vector3_zero();
		if (gui_button("Reset scale", gui))
			ent->transform.scale = vector3_one();
		/*if (gui_button("Snap to floor", gui))
		{
			ent->transform.position.z = 0;
			ent->transform.position.z -= ent->z_bound.min * ent->transform.scale.z;
		}*/

		if (gui_shortcut_button("Delete", KEYS_DELETEMASK, gui))
		{
			destroy_entity(&ed->world, ent);
			dat->sel_ent = NULL;
			gui_end(gui);
			return ;
		}
		gui_end(gui);
		if (ent == dat->info.hit_entity && ed->hid.mouse.held == MOUSE_LEFT && ed->hid.mouse.relative)
		{
			dat->grabbing = true;
			ent->ignore_raycasts = true;
			return ;
		}
		if ((ed->hid.mouse.held == 0 || !ed->hid.mouse.relative) || dat->entitygui.locking_player)
		{
			ent->ignore_raycasts = false;
			dat->grabbing = false;
			return ;
		}
			
		if (dat->grabbing && !vector3_cmp(dat->info.hit_pos, vector3_zero()))
		{
			ent->transform.position = dat->info.hit_pos;
			//ent->transform.position.z -= ent->z_bound.min * ent->transform.scale.z;
			ent->transform.rotation.x += ed->hid.mouse.scroll_delta * ft_degtorad(5.0f);
		}
		if ((ed->hid.keystate >> KEYS_DELETEMASK) & 1)
		{
			destroy_entity(&ed->world, ent);
			dat->sel_ent = NULL;
		}
		if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		{
			dat->sel_ent->ignore_raycasts = false;
			dat->sel_ent = NULL;
		}
	}
}

void	entity_tool_list(t_editor *ed, t_sdlcontext *sdl, t_entitytooldata *dat)
{
	t_autogui		*gui = &dat->worldgui;
	int				i = 0;
	int				found = 0;
	t_entitycache	*cache;
	
	gui_start(gui);
	i = 0;
	cache = &ed->world.entitycache;
	while (i < cache->alloc_count)
	{
		t_entity	ent;
		char		*str;

		ent = cache->entities[i];
		str = ft_itoa(ent.id);
		gui_starthorizontal(gui);
		gui_label(str, gui);
		if (gui_hoverlabel("Show", gui))
		{
			highlight_entity(sdl, &ent, CLR_RED);
		}
		gui_endhorizontal(gui);
		free(str);
		i++;
	}
	gui_end(gui);
}

void	entity_tool_raycast(t_editor *ed, t_sdlcontext *sdl, t_entitytooldata *dat)
{
	t_ray				ray;

	ray.origin = ed->player.transform.position;
	ray.dir = ed->player.lookdir;//vector3_mul(ed->player.lookdir, 10000.f);
	bzero(&dat->info, sizeof(t_raycastinfo));
	if (raycast_new(ray, &dat->info, &ed->world))
	{
		if (dat->info.hit_entity != NULL
			&& !dat->info.hit_entity->rigid)
			highlight_entity(sdl, dat->info.hit_entity, AMBER_4);
	}
	else
		raycast_plane(ray, &dat->info, 0.0f);
}

void	list_prefabs(t_prefab_editor *pe, t_world *world)
{
	t_list		*l;
	t_prefab	*fab;
	t_autogui	*gui;

	gui = &pe->prefab_select;
	l = world->prefabs;
	while (l != NULL)
	{
		fab = l->content;
		if (gui_highlighted_button_if(fab->prefab_name, gui, pe->prefab == fab))
		{
			pe->prefab = fab;
		}
			
		l = l->next;
	}
}

void	select_prefab_type(t_prefab_editor *pe)
{
	t_autogui	*gui;

	gui = &pe->prefab_modify;
	gui_label("Prefab type:", gui);
	if (gui_highlighted_button_if("None", gui, pe->prefab->prefabtype == pft_none))
		pe->prefab->prefabtype = pft_none;
	if (gui_highlighted_button_if("Light", gui, pe->prefab->prefabtype == pft_light))
	{
		pe->prefab->prefabtype = pft_light;
		if (pe->prefab->data != NULL)
		{
			free(pe->prefab->data);
			pe->prefab->data = NULL;
		}
		pe->prefab->data = ft_memalloc(sizeof(t_pointlight)); //TODO: protect
		t_pointlight	*pl;
		pl = pe->prefab->data;
		pl->radius = 100.0f;
		pl->shadows = true;
	}
		
	if (gui_highlighted_button_if("Audio", gui, pe->prefab->prefabtype == pft_audiosource))
		pe->prefab->prefabtype = pft_audiosource;
	if (gui_highlighted_button_if("Interactable", gui, pe->prefab->prefabtype == pft_interactable))
		pe->prefab->prefabtype = pft_interactable;
}

void	edit_prefab_subtype(t_prefab_editor *pe)
{
	t_autogui	*gui;

	gui = &pe->subtype_gui;
	if (pe->prefab->prefabtype == pft_light)
	{
		t_pointlight	*light;
		light = pe->prefab->data;

		gui_start(gui);
		gui_labeled_float_slider("Light radius:", &light->radius, 2.0f, gui);
		gui_end(gui);
		gui->sdl->render.gizmocolor = AMBER_2;
		render_ball(gui->sdl, pe->preview_entity.transform.position, light->radius, AMBER_3);
	}
}

void	entity_tool_edit_prefab(t_editor *ed, t_entitytooldata *dat)
{
	t_sdlcontext	*sdl;
	t_autogui		*gui;
	t_prefab_editor	*pe;

	pe = &dat->prefab_editor;
	gui = &pe->prefab_modify;
	sdl = gui->sdl;
	if (pe->prefab != NULL)
	{
		ed->world.sdl->global_wireframe = true;
		if (!pe->view_lock)
			pe->preview_entity.transform.position = dat->info.hit_pos;
		pe->preview_entity.obj = pe->prefab->object;
		pe->preview_entity.hidden = pe->prefab->hidden;
		pe->preview_entity.transform.scale = vector3_one();
		rendergrid(&ed->world, dat->info.hit_pos, 5, CLR_DARKGRAY);
		ed->world.sdl->global_wireframe = false;
		if (!pe->preview_entity.hidden)
			render_entity(sdl, &sdl->render, &pe->preview_entity);
		highlight_entity(sdl, &pe->preview_entity, AMBER_3);
		ed->world.sdl->global_wireframe = true;

		gui_start(gui);
		gui_label("Object:", gui);
		if (pe->prefab->object != NULL)
			gui_label(pe->prefab->object->name, gui);
		gui_labeled_bool_edit("Lock view", &pe->view_lock, gui);
		if (gui_highlighted_button_if("Select object", gui, !pe->object_select.hidden))
			pe->object_select.hidden = !pe->object_select.hidden;
		gui_labeled_bool_edit("Hide object", &pe->prefab->hidden, gui);
		gui_preset_transform(&pe->prefab->offset, gui);
		select_prefab_type(pe);
		gui_end(gui);

		if (pe->prefab->prefabtype != pft_none)
			edit_prefab_subtype(pe);
		//TODO: split
		gui = &pe->object_select;
		gui_start(gui);
		int			i;
		t_object	*obj;

		i = 0;
		if (gui_highlighted_button_if("No object", gui, pe->prefab->object == NULL))
			pe->prefab->object = NULL;

		while (i < ed->world.sdl->objectcount)
		{
			obj = &ed->world.sdl->objects[i];
			if (gui_highlighted_button_if(obj->name, gui, obj == pe->prefab->object))
				pe->prefab->object = obj;
			i++;
		}
		gui_end(gui);
		
	} else {
		ed->world.sdl->global_wireframe = false;
	}
}

void	entity_tool_prefab(t_editor *ed, t_entitytooldata *dat)
{
	t_sdlcontext	*sdl;
	t_autogui		*gui;

	gui = &dat->prefab_editor.prefab_select;
	sdl = ed->world.sdl;
	gui_start(gui);
	list_prefabs(&dat->prefab_editor, &ed->world);
	if (gui_button("Add new prefab", gui))
	{
		t_prefab	fab;
		ft_bzero(&fab, sizeof(fab));
		ft_strcpy(fab.object_name, "cube.obj");
		ft_strcpy(fab.prefab_name, "cubeprefab");
		fab.object = get_object_by_name(*sdl, fab.object_name);
		fab.offset.scale = vector3_one();
		list_push(&ed->world.prefabs, &fab, sizeof(fab));
	}
	gui_end(gui);
	entity_tool_edit_prefab(ed, dat);
	
}

void	entity_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_entitytooldata	*dat;
	t_entity			*collide;

	dat = (t_entitytooldata *)ed->tool->tooldata;
	entity_tool_raycast(ed, sdl, dat);
	//entity_tool_list(ed, sdl, dat);
	entity_tool_prefab(ed, dat);
	//entity_tool_place(ed, sdl, dat);
	entity_tool_modify(ed, sdl, dat);
}

void	entity_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_entitytooldata	*dat;

	dat = ed->tool->tooldata;
	entitytool_entitygui_init(ed, sdl, dat);
	entitytool_worldgui_init(ed, sdl, dat);
	entitytool_prefabgui_init(ed, sdl, dat);
	entitytool_objectgui_init(ed, sdl, dat);
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
