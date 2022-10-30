/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 06:45:42 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/30 23:17:16 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "inputhelp.h"

bool	mouse_clicked(t_mouse mouse, int button)
{
	return (mouse.click_unhandled == true && mouse.click_button == button);
}

t_point	mousetoworldspace(t_editor *ed)
{
	return (point_sub(ed->mouse.pos, ed->offset));
}

t_point	mousetogridspace(t_editor *ed)
{
	t_point	result;
	t_point	mod;

	result.x = -(ed->offset.x - ed->mouse.pos.x); //USED MOUSEDRAG
	result.y = -(ed->offset.y - ed->mouse.pos.y); //USED MOUSEDRAG
	mod.x = result.x % TILESIZE;
	mod.y = result.y % TILESIZE;
	result = point_div(result, TILESIZE);
	result.x += (bool)(mod.x > TILESIZE / 2);
	result.y += (bool)(mod.y > TILESIZE / 2);
	return (result);
}

t_point	screentogridspace(t_point point)
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

static void	mouseclick(t_editor *ed)
{
	t_list		*l;
	t_guibutton	button;

	l = ed->buttonlist;

	printf("mouseclick \n");
	while (l != NULL)
	{
		button = *(t_guibutton *)l->content;
		if (pointrectanglecollision(ed->mouse.pos, button.rect) && ed->mouse.click_button == MOUSE_LEFT)
		{
			if (button.onclick != NULL)
				button.onclick(ed);
		}
		l = l->next;
	}
}

void	mouse_event(SDL_Event e, t_editor *ed)
{
	SDL_GetMouseState(&ed->mouse.pos.x, &ed->mouse.pos.y);
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		ed->mouse.held = e.button.button;
	}
	if (e.type == SDL_MOUSEBUTTONUP)
	{
		ed->mouse.click_unhandled = true;
		ed->mouse.click_button = e.button.button;
		ed->mouse.held = 0;
	}
	if (e.type == SDL_MOUSEWHEEL)
	{
		ed->mouse.scroll += e.wheel.y;
		ed->mouse.scroll_delta = e.wheel.y;
		ed->mouse.scroll = ft_clamp(ed->mouse.scroll, -20, 20);
	}
	if (ed->mouse.click_unhandled
		&& ((ed->keystate >> KEYS_CTRLMASK) & 1) == false
		&& ((ed->keystate >> KEYS_SHIFTMASK) & 1) == false)
		mouseclick(ed);
}
