/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 06:45:42 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/14 20:20:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

t_point	screentoworldspace(t_point point)
{
	t_point	result;
	t_point	mod;

	result = point_div(point, TILESIZE);
	mod.x = point.x % TILESIZE;
	mod.y = point.y % TILESIZE;
	result.x += (bool)(mod.x > TILESIZE / 2);
	result.y += (bool)(mod.y > TILESIZE / 2);
	return (result);
}

//TODO: only clamps d.pos, should also limit drag somehow
static void	mousedrag(t_editor *ed)
{
	t_point		m2;
	t_mousedrag	*d;

	d = ed->mousedrag;
	if (ed->mouse.held != 0)
		SDL_GetMouseState(&m2.x, &m2.y);
	if (ed->mouse.held == MOUSE_RIGHT)
	{
		d->drag.x = m2.x - d->drag_origin.x;
		d->drag.y = m2.y - d->drag_origin.y;
		d->pos.x = d->pos_origin.x + d->drag.x;
		d->pos.y = d->pos_origin.y + d->drag.y;
		if (ed->state != display3d)
		{
			d->pos.x = ft_clamp(d->pos.x, -(GRIDSIZE * TILESIZE) + WINDOW_W - 20, 20);
			d->pos.y = ft_clamp(d->pos.y, -(GRIDSIZE * TILESIZE) + WINDOW_H - 20, 20);
		}
	}
	d = &ed->mousedrag[1]; //MDL mouse drag
	if (ed->mouse.held == MOUSE_MDL && ed->state == display3d)
	{
		d->drag.x = m2.x - d->drag_origin.x;
		d->drag.y = m2.y - d->drag_origin.y;
		d->pos.x = d->pos_origin.x + d->drag.x;
		d->pos.y = d->pos_origin.y + d->drag.y;
		d->pos.y = ft_clamp(d->pos.y, -300, 300);
	}
}

static void	mouseclick(t_editor *ed)
{

	if (ed->mouse.click)
	{
		ed->mouse.click = false;
		if (ed->state == place_end)
		{
			ed->line.end = screentoworldspace(ed->mouse.pos);
			saveline(ed);
			ft_bzero(&ed->line, sizeof(t_line));
			ed->state = place_start;
		}
		else if (ed->state == place_start)
		{
			ed->line.start = screentoworldspace(ed->mouse.pos);
			ed->state = place_end;
		}
	}
}

void	mouse_event(SDL_Event e, t_editor *ed)
{
	SDL_GetMouseState(&ed->mouse.pos.x, &ed->mouse.pos.y);
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		ed->mouse.held = e.button.button;
		ed->mousedrag->drag_origin.x = ed->mouse.pos.x;
		ed->mousedrag->drag_origin.y = ed->mouse.pos.y;
		if (e.button.button == MOUSE_MDL && ed->state == display3d)
		{
			ed->mousedrag[1].drag_origin.x = ed->mouse.pos.x;
			ed->mousedrag[1].drag_origin.y = ed->mouse.pos.y;
		}
	}
	if (e.type == SDL_MOUSEBUTTONUP)
	{
		ed->mouse.click = (ed->mouse.held == MOUSE_LEFT);
		ed->mouse.held = 0;
		ed->mousedrag->pos_origin.x += ed->mousedrag->drag.x; //TODO: use v2add
		ed->mousedrag->pos_origin.y += ed->mousedrag->drag.y;
		ed->mousedrag->drag = point_zero();
		if (e.button.button == MOUSE_MDL && ed->state == display3d)
		{
			ed->mousedrag[1].pos_origin.x += ed->mousedrag[1].drag.x;
			ed->mousedrag[1].pos_origin.y += ed->mousedrag[1].drag.y;
			ed->mousedrag[1].drag = point_zero();
		}
	}
	if (e.type == SDL_MOUSEWHEEL)
	{
		ed->mouse.scroll += e.wheel.y;
		ed->mouse.scroll_delta = e.wheel.y;
		ed->mouse.scroll = ft_clamp(ed->mouse.scroll, -20, 20);
	}
	mousedrag(ed);
	mouseclick(ed);
}