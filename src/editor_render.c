/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 08:20:49 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/01 15:06:06 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	renderline(t_sdlcontext *sdl, t_editor *ed, t_line line) 
{
	/*t_line	line_ss; //line converted to screenspace

	line_ss.start = point_mul(line.start, TILESIZE);
	line_ss.start = point_add(line_ss.start, ed->offset);
	line_ss.end = point_mul(line.end, TILESIZE);
	line_ss.end = point_add(line_ss.end, ed->offset);
	drawcircle(*sdl, line_ss.start, 5, CLR_GREEN);
	drawcircle(*sdl, line_ss.end, 5, CLR_GREEN);
	drawline(*sdl, line_ss.start, line_ss.end, CLR_PRPL);*/
}

void	renderlines(t_sdlcontext *sdl, t_editor *ed)
{
	t_list	*l;

	l = ed->linelist;
	while (l != NULL)
	{
		renderline(sdl, ed, *(t_line *)l->content);
		l = l->next;
	}
}
