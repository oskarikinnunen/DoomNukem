/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:13:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/04 16:36:42 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "tools/walltool.h"

t_point	snapped_cursor(t_editor *ed)
{
	t_point	snapped;

	snapped = point_mul(screentogridspace(ed->mouse.pos), 32);
	//snapped = point_add(snapped, point_mod(ed->offset, 32);
	return (snapped);
}

static void	wall_tool_draw(t_editor *ed, t_sdlcontext sdl) //TODO: ROTATE AROUND AXIS, SCREENSPACE
{
	drawcircle(sdl, ed->mouse.pos, 10, CLR_TURQ);
	drawcircle(sdl, snapped_cursor(ed), 10, CLR_PRPL);
	//drawline()
	///point_snap()
	//TODO: point_snap the cursor and draw cursorball?
	return ;
}

static void	wall_tool_update(t_editor *ed) //This needs to access editors state, so pass editor here??
{
	t_walltooldata	*wtd;

	wtd = (t_walltooldata *)ed->tool->tooldata;
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
		//wtd->current = object_plane();
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
