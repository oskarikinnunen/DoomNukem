/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npc_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:18:07 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/22 17:10:28 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/npctool.h"
#include "doomnukem.h"

void	npc_tool_draw(t_editor *ed, t_sdlcontext sdl)
{
	t_npctooldata	*dat;

	dat = ed->tool->tooldata;
	// select object with object selector and load it
	//dat->ent
	/*dat->ent = selected_entity(ed, sdl);
	if (dat->ent != NULL)
	{
		ed->render.wireframe = true;
		ed->render.gizmocolor = CLR_GREEN;
		render_entity(sdl, ed->render, dat->ent);
		ed->render.wireframe = false;
	}*/
}

void	npc_tool_update(t_editor *ed, t_sdlcontext sdl)
{
	
}

t_tool	*get_npc_tool()
{
	static t_tool	tool
	= {
		npc_tool_update, npc_tool_draw
	};
	t_npctooldata	*dat;
	t_entity		*ent;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_npctooldata));
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
		dat = (t_npctooldata *)tool.tooldata;
		/*dat->ent->transform.location = vector3_zero();
		dat->ent->transform.scale = vector3_one();*/
	}
	ft_strcpy(tool.icon_name, "entitytool.png");
	return (&tool);
}
