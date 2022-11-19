/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_tool_rooms.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 03:20:37 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/08 05:07:02 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "inputhelp.h"
#include "tools/walltool.h"
#include "objects.h"


/* DRAWDEBUGFLOOR */
/*
	takes index, indexes to that point and draws the attempted triangle to screen

*/

void	floorcalc_debugdraw(t_editor *ed, t_sdlcontext *sdl, t_floorcalc fc)
{
	int			i;
	t_vector3	ws;

	i = 0;
	while (i < fc.edgecount)
	{
		ws = (t_vector3){fc.edges[i].x, fc.edges[i].y, 0.0f};
		ed->render.gizmocolor = CLR_RED;
		render_gizmo(*sdl, ed->render, ws, 10);
		i++;
	}
}

/*static void gen_tris()
{

}*/

t_floorcalc	generate_floor(t_walltooldata *dat)
{
	t_floorcalc	fc;
	int			i;

	i = 0;
	fc.edgecount = 0;
	ft_bzero(&fc, sizeof(fc));
	while (i < MAXSELECTED)
	{
		if (dat->selected[i] != NULL)
			fc.edges[fc.edgecount++] = dat->selected[i]->line.start;
		i++;
	}
	return (fc);
}