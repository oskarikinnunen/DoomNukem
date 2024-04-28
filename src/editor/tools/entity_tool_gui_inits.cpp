/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool_gui_inits.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:48:51 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 19:54:53 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tools/entitytool.h" 
#include "objects.h"

void	entitytool_entitygui_init(t_editor *ed,
							t_sdlcontext *sdl, t_entity_tool_data *dat)
{
	t_autogui	*gui;

	gui = &dat->entity_ed.gui;
	if (gui->sdl == NULL)
	{
		*gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player,
				(t_point){20, 40}, "Edit entity"});
		gui->minimum_size.x = 300;
		gui->minimum_size.y = 200;
	}
	gui = &dat->entity_ed.component_gui;
	if (gui->sdl == NULL)
	{
		*gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player,
				(t_point){20, 40}, "Edit component"});
		gui->minimum_size.x = 300;
		gui->minimum_size.y = 200;
		gui->rect.position = point_sub(sdl->screensize, gui->minimum_size);
		gui->rect.position.y = 0;
		gui->dock = &dat->entity_ed.gui;
	}
}

void	entitytool_worldgui_init(t_editor *ed,
							t_sdlcontext *sdl, t_entity_tool_data *dat)
{
	if (dat->world_gui.sdl == NULL)
	{
		dat->world_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player,
				(t_point){200, 40}, "World entities"});
		dat->world_gui.minimum_size.x = 300;
		dat->world_gui.minimum_size.y = 450;
		dat->world_gui.rect.size = dat->world_gui.minimum_size;
	}
}

void	entitytool_objectgui_init(t_editor *ed,
							t_sdlcontext *sdl, t_entity_tool_data *dat)
{
	if (dat->new_object_gui.sdl == NULL)
	{
		dat->new_object_gui = init_gui((t_gui_init){sdl, &ed->hid, &ed->player,
				(t_point){20, 40}, "Place new entity"});
		dat->new_object_gui.minimum_size.x = 140;
		dat->new_object_gui.rect.size.x = dat->new_object_gui.minimum_size.x;
	}
}
