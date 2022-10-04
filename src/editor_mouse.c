/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 06:45:42 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 08:49:54 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	mousetoworldspace(int v2[2], t_editor *ed)
{
	int	mod[2];
	v2[X] = -(ed->mousedrag.pos[X] - ed->mouse.p[X]);
	v2[Y] = -(ed->mousedrag.pos[Y] - ed->mouse.p[Y]);
	mod[X] = v2[X] % TILESIZE;
	mod[Y] = v2[Y] % TILESIZE;
	v2[X] /= TILESIZE;
	v2[Y] /= TILESIZE;
	if (mod[X] > TILESIZE / 2)
		v2[X]++;
	if (mod[Y] > TILESIZE / 2)
		v2[Y]++;
	printf("mod %i %i \n", mod[X], mod[Y]);
}

static void	mousedrag(t_editor *ed)
{
	int			m2[2];
	t_mousedrag	*d;

	d = &ed->mousedrag;
	if (ed->mouse.held == MOUSE_RIGHT)
	{
		SDL_GetMouseState(&m2[X], &m2[Y]);
		d->drag[X] = m2[X] - d->drag_origin[X];
		d->drag[Y] = m2[Y] - d->drag_origin[Y];
		d->pos[X] = d->pos_origin[X] + d->drag[X];
		d->pos[Y] = d->pos_origin[Y] + d->drag[Y];
		d->pos[X] = ft_clamp(d->pos[X], -(GRIDSIZE * TILESIZE) + WINDOW_W - 20, 20);
		printf("X: %i \n", d->pos[X]);
		d->pos[Y] = ft_clamp(d->pos[Y], -(GRIDSIZE * TILESIZE) + WINDOW_H - 20, 20);
	}
	if (ed->mouse.held == MOUSE_MDL)
	{
		bzero(&d->pos_origin, sizeof(int [2]));
		d->pos[X] = d->pos_origin[X] + d->drag[X];
		d->pos[Y] = d->pos_origin[Y] + d->drag[Y];
	}
}

static void mouseclick(t_editor *ed)
{

	if (ed->mouse.click)
	{
		ed->mouse.click = false;
		ed->state++;
		if (ed->state > place_end)
			ed->state = none;
		if (ed->state == place_start)
		{
			mousetoworldspace(ed->line.start, ed);
		}
			
	}
		
}

void	mouse_event(SDL_Event e, t_editor *ed)
{
	SDL_GetMouseState(&ed->mouse.p[X], &ed->mouse.p[Y]);
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		ed->mouse.held = e.button.button;
		ed->mousedrag.drag_origin[X] = ed->mouse.p[X];
		ed->mousedrag.drag_origin[Y] = ed->mouse.p[Y];
	}
	if (e.type == SDL_MOUSEBUTTONUP)
	{
		ed->mouse.click = (ed->mouse.held == MOUSE_LEFT);
		ed->mouse.held = 0;
		ed->mousedrag.pos_origin[X] += ed->mousedrag.drag[X];
		ed->mousedrag.pos_origin[Y] += ed->mousedrag.drag[Y];
		bzero(&ed->mousedrag.drag, sizeof(int [2]));
	}
	mousedrag(ed);
	mouseclick(ed);
}