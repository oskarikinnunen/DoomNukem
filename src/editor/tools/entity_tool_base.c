/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool_base.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:07:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/21 16:08:15 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tools/entitytool.h" 
#include "objects.h"

void	entity_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_entitytooldata	*dat;
	t_entity			*collide;

	dat = (t_entitytooldata *)ed->tool->tooldata;
	entity_tool_raycast(ed, sdl, dat);
	if (dat->new_ent->hidden)
		entity_tool_modify(ed, sdl, dat);
	entity_tool_place(ed, sdl, dat);
}

void	entity_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_entitytooldata	*dat;

	dat = ed->tool->tooldata;
	if (dat->new_ent == NULL)
	{
		dat->new_ent = spawn_entity(&ed->world, NULL);
		dat->new_ent->ignore_raycasts = true;
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
	t_entitytooldata	*dat;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_entitytooldata));
		if (tool.tooldata == NULL)
			doomlog(LOG_EC_MALLOC, NULL);
		dat = (t_entitytooldata *)tool.tooldata;
	}
	ft_strcpy(tool.icon_name, "entitytool.png");
	return (&tool);
}
