/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool_base.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:07:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 11:50:19 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tools/entitytool.h" 
#include "objects.h"

void	entity_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_entity_tool_data	*dat;
	t_entity			*collide;

	dat = (t_entity_tool_data *)ed->tool->tool_data;
	entity_tool_raycast(ed, sdl, dat);
	if (dat->new_ent->hidden)
		entity_tool_modify(ed, sdl, dat);
	entity_tool_place(ed, sdl, dat);
}

void	entity_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_entity_tool_data	*dat;

	dat = ed->tool->tool_data;
	if (dat->new_ent == NULL)
	{
		dat->new_ent = spawn_entity(&ed->world, NULL);
		dat->new_ent->ignore_raycasts = true;
		dat->new_ent->dont_save = true;
	}
	entitytool_entitygui_init(ed, sdl, dat);
	entitytool_worldgui_init(ed, sdl, dat);
	entitytool_objectgui_init(ed, sdl, dat);
}

t_tool	*get_entity_tool(void)
{
	static t_tool		tool
		= {
		.update = entity_tool_update,
		.init = entity_tool_init
	};
	t_entity_tool_data	*dat;

	if (tool.tool_data == NULL)
	{
		tool.tool_data = prot_memalloc(sizeof(t_entity_tool_data));
		dat = (t_entity_tool_data *)tool.tool_data;
	}
	ft_strncpy_term(tool.icon_name, "entitytool.png", 250);
	return (&tool);
}
