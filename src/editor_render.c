/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 08:20:49 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 09:35:21 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

	/*t_line	line_ss; //line converted to screenspace
	int		circlepos[2];

	ft_memcpy(&line_ss, &ed->line, sizeof(t_line));
	v2mul(line_ss.start, TILESIZE);
	v2add(line_ss.start, ed->mousedrag.pos);
	if (ed->state == place_start)
	{
		v2mul(line_ss.end, TILESIZE);
		v2add(line_ss.end, ed->mousedrag.pos);
	}
	else
		v2mul((mousetoworldspace(line_ss.end, ed), line_ss.end), TILESIZE);
	drawline((uint32_t *)sdl->surface->pixels, line_ss.start, line_ss.end, CLR_PRPL);
	
	mousetoworldspace(circlepos, ed);
	v2mul(circlepos, TILESIZE);
	v2add(circlepos, ed->mousedrag.pos);
	drawline((uint32_t *)sdl->surface->pixels, circlepos, ed->mouse.p, CLR_PRPL);
	drawcircle((uint32_t *)sdl->surface->pixels, circlepos, 10, CLR_PRPL);*/

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
	if (ed->state == place_start)
	{
		v2mul(line_ss.end, TILESIZE);
		v2add(line_ss.end, ed->mousedrag.pos);
	}
	else
		v2mul((mousetoworldspace(line_ss.end, ed), line_ss.end), TILESIZE);
	drawline((uint32_t *)sdl->surface->pixels, line_ss.start, line_ss.end, CLR_TURQ);
	mousetoworldspace(circlepos, ed);
	v2mul(circlepos, TILESIZE);
	v2add(circlepos, ed->mousedrag.pos);
	drawline((uint32_t *)sdl->surface->pixels, circlepos, ed->mouse.p, CLR_TURQ);
	drawcircle((uint32_t *)sdl->surface->pixels, circlepos, 10, CLR_TURQ);
}

void	renderlines(t_sdlcontext *sdl, t_editor *ed)
{
	t_list	*l;

	render_editline(sdl, ed);
	l = ed->linelist;
	while (l != NULL)
	{
		renderline(sdl, ed, *(t_line *)l->content);
		l = l->next;
	}
}
