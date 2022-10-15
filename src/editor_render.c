/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 08:20:49 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/15 12:24:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	renderline(t_sdlcontext *sdl, t_editor *ed, t_line line) 
{
	t_line	line_ss; //line converted to screenspace

	/*ft_memcpy(&line_ss, &line, sizeof(t_line));
	v2mul(line_ss.start, TILESIZE);
	v2add(line_ss.start, ed->mousedrag->pos);
	v2mul(line_ss.end, TILESIZE);
	v2add(line_ss.end, ed->mousedrag->pos);*/
	line_ss.start = point_mul(line.start, TILESIZE);
	line_ss.start = point_add(line_ss.start, ed->mousedrag->pos);
	line_ss.end = point_mul(line.end, TILESIZE);
	line_ss.end = point_add(line_ss.end, ed->mousedrag->pos);
	drawline((uint32_t *)sdl->surface->pixels, line_ss.start, line_ss.end, CLR_PRPL);
}

static void	render_editline(t_sdlcontext *sdl, t_editor *ed)
{
	t_line	line_ss; //line converted to screenspace
	t_point	circlepos;

	line_ss.start = point_mul(ed->line.start, TILESIZE);
	line_ss.start = point_add(line_ss.start, ed->mousedrag->pos);
	line_ss.end = point_mul(ed->line.end, TILESIZE);
	line_ss.end = point_add(line_ss.end, ed->mousedrag->pos);
	circlepos = mousetoworldspace(ed);
	circlepos = point_mul(circlepos, TILESIZE);
	circlepos = point_add(circlepos, ed->mousedrag->pos);
	//circlepos = point_add(circlepos, point_div(ed->mousedrag->pos, TILESIZE));
	//mousetoworldspace(circlepos, ed);
	//circlepos = point_mul(circlepos, TILESIZE);
	//circlepos = point_add(circlepos, ed->mousedrag->pos);
	drawline((uint32_t *)sdl->surface->pixels, circlepos, ed->mouse.pos, CLR_TURQ);
	drawcircle((uint32_t *)sdl->surface->pixels, circlepos, 10, CLR_TURQ);
	if (ed->state == place_start)
		return;
	drawline((uint32_t *)sdl->surface->pixels, line_ss.start, circlepos, CLR_TURQ);
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
	render_editline(sdl, ed);
}
