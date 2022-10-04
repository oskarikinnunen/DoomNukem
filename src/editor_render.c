/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 08:20:49 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 11:43:26 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	renderline(t_sdlcontext *sdl, t_editor *ed, t_line line) 
{
	t_line	line_ss; //line converted to screenspace
	int		circlepos[2];

	ft_memcpy(&line_ss, &line, sizeof(t_line));
	v2mul(line_ss.start, TILESIZE);
	v2add(line_ss.start, ed->mousedrag.pos);
	v2mul(line_ss.end, TILESIZE);
	v2add(line_ss.end, ed->mousedrag.pos);
	drawline((uint32_t *)sdl->surface->pixels, line_ss.start, line_ss.end, CLR_PRPL);
}

static void	render_editline(t_sdlcontext *sdl, t_editor *ed)
{
	t_line	line_ss; //line converted to screenspace
	int		circlepos[2];

	ft_memcpy(&line_ss, &ed->line, sizeof(t_line));
	v2mul(line_ss.start, TILESIZE);
	v2add(line_ss.start, ed->mousedrag.pos);
	mousetoworldspace(circlepos, ed);
	v2mul(circlepos, TILESIZE);
	v2add(circlepos, ed->mousedrag.pos);
	drawline((uint32_t *)sdl->surface->pixels, circlepos, ed->mouse.p, CLR_TURQ);
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
