/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:13:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/05 18:14:39 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "tools/walltool.h"

static void	wall_tool_draw(t_editor *ed, t_sdlcontext sdl) //TODO: ROTATE AROUND AXIS, SCREENSPACE
{
	t_walltooldata	*dat;
	t_wall			*wall;

	dat = (t_walltooldata *)ed->tool->tooldata;
	wall = &dat->wall;
	ed->render.gizmocolor = CLR_GREEN;
	ed->render.wireframe = true;
	render_object(sdl, ed->render, &wall->entity);
	ed->render.wireframe = false;
}

static void	wall_tool_update(t_editor *ed) //This needs to access editors state, so pass editor here??
{
	t_walltooldata	*dat;
	t_wall			*wall;
	t_vector3		rc;

	dat = (t_walltooldata *)ed->tool->tooldata;
	wall = &dat->wall;
	rc = raycast(ed);
	if (dat->mode == place_first)
	{
		/*wall->line.start = raycast(ed);
		wall->line.end = raycast(ed);*/
	}
}

t_tool	*get_wall_tool()
{
	static t_tool	tool
	= {
		wall_tool_update, wall_tool_draw
	};
	t_walltooldata	*wtd;

	if (tool.tooldata == NULL)
	{
		//tool.tooldata = ft_memalloc(sizeof(t_entity));
		tool.tooldata = ft_memalloc(sizeof(t_walltooldata));
		
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
		wtd = (t_walltooldata *)tool.tooldata;
		wtd->wall.entity.obj = object_plane();
		wtd->wall.entity.transform.scale = vector3_mul(vector3_one(), 10.0f);
		wtd->mode = place_first;
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
