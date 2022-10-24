/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 08:20:49 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/24 20:24:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	renderline(t_sdlcontext *sdl, t_editor *ed, t_line line) 
{
	t_line	line_ss; //line converted to screenspace

	line_ss.start = point_mul(line.start, TILESIZE);
	line_ss.start = point_add(line_ss.start, ed->offset);
	line_ss.end = point_mul(line.end, TILESIZE);
	line_ss.end = point_add(line_ss.end, ed->offset);
	drawline(*sdl, line_ss.start, line_ss.end, CLR_PRPL);
}

static void	render_cursorball(t_sdlcontext *sdl, t_editor *ed)
{
	t_point	circlepos;

	circlepos = mousetoworldspace(ed);
	circlepos = point_mul(circlepos, TILESIZE);
	circlepos = point_add(circlepos, ed->offset);
	drawcircle(*sdl, circlepos, 10, CLR_TURQ);
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
	render_cursorball(sdl, ed);
}
