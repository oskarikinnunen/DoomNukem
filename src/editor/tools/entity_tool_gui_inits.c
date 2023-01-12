/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool_gui_inits.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:48:51 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/12 15:44:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tools/entitytool.h" 
#include "objects.h"

void	entitytool_entitygui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat)
{
	if (dat->entitygui.sdl == NULL)
	{
		dat->entitygui = init_gui(sdl, &ed->hid, &ed->player, (t_point) {20, 40}, "Edit entity");
		dat->entitygui.minimum_size.x = 300;
		dat->entitygui.minimum_size.y = 200;
		dat->entitygui.rect.position = point_sub(sdl->screensize, dat->entitygui.minimum_size);
		dat->entitygui.rect.position.y = 0;
	}
}

void	entitytool_prefabgui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat)
{
	t_prefab_editor *pe;

	pe = &dat->prefab_editor;
	
	if (pe->prefab_select.sdl == NULL)
	{
		pe->prefab_select = init_gui(sdl, &ed->hid, &ed->player, (t_point){0, 200}, "Select Prefab");
		pe->prefab_select.minimum_size.x = 300;
		pe->prefab_select.minimum_size.y = 200;
	}
	if (pe->prefab_modify.sdl == NULL)
	{
		pe->prefab_modify = init_gui(sdl, &ed->hid, &ed->player, (t_point){200, 200}, "Edit prefab");
		pe->prefab_modify.minimum_size.x = 300;
		pe->prefab_modify.minimum_size.y = 200;
		pe->prefab_modify.dock = &pe->prefab_select;
	}
	if (pe->subtype_gui.sdl == NULL)
	{
		pe->subtype_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point){200, 200}, "Edit subtype");
		pe->subtype_gui.minimum_size.x = 300;
		pe->subtype_gui.minimum_size.y = 200;
		pe->subtype_gui.dock = &pe->prefab_modify;
	}
	if (pe->object_select.sdl == NULL)
	{
		pe->object_select = init_gui(sdl, &ed->hid, &ed->player, (t_point){200, 200}, "Select Object");
		pe->object_select.minimum_size.x = 300;
		pe->object_select.minimum_size.y = 200;
		pe->object_select.dock = &pe->prefab_modify;
		pe->object_select.hidden = true;
	}
}

void	entitytool_worldgui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat)
{
	if (dat->worldgui.sdl == NULL)
	{
		dat->worldgui = init_gui(sdl, &ed->hid, &ed->player, (t_point) {200, 40}, "World entities");
		dat->worldgui.minimum_size.x = 300;
		dat->worldgui.minimum_size.y = 450;
		dat->worldgui.rect.size = dat->worldgui.minimum_size;
	}
}

void	entitytool_objectgui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat)
{
	if (dat->objectgui.gui.sdl == NULL)
	{
		dat->objectgui.gui = init_gui(sdl, &ed->hid, &ed->player, (t_point) {20, 40}, "Place new entity");
		dat->objectgui.gui.minimum_size.x = 140;
		dat->objectgui.gui.rect.size.x = dat->objectgui.gui.minimum_size.x;
		dat->objectgui.autoclose = false;
	}
}