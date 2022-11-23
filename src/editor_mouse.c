/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 06:45:42 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/23 18:48:53 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "inputhelp.h"

bool	mouse_clicked(t_mouse mouse, int button)
{
	return (mouse.click_unhandled == true && mouse.click_button == button);
}

t_point	mousetogridspace(t_editor *ed)
{
	t_point	result;
	t_point	mod;

	result.x = -(ed->position.x - ed->mouse.pos.x); //USED MOUSEDRAG
	result.y = -(ed->position.y - ed->mouse.pos.y); //USED MOUSEDRAG
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

void	mouse_event(SDL_Event e, t_mouse *mouse)
{
	if (!mouse->relative)
		SDL_GetMouseState(&mouse->pos.x, &mouse->pos.y);
	if (e.type == SDL_MOUSEBUTTONDOWN) //TODO: bitshift mouseheldstate
	{
		mouse->held = e.button.button;
		mouse->heldstate |= (e.button.button == MOUSE_LEFT) << MOUSE_LEFT;
		mouse->heldstate |= (e.button.button == MOUSE_RIGHT) << MOUSE_RIGHT;
	}
	if (e.type == SDL_MOUSEBUTTONUP)
	{
		mouse->click_unhandled = true;
		mouse->click_button = e.button.button;
		mouse->heldstate &= ~((e.button.button == MOUSE_LEFT) << MOUSE_LEFT);
		mouse->heldstate &= ~((e.button.button == MOUSE_RIGHT) << MOUSE_RIGHT);
		mouse->held = 0;
	}
	if (e.type == SDL_MOUSEWHEEL)
	{
		mouse->scroll += e.wheel.y;
		mouse->scroll_delta = e.wheel.y;
		mouse->scroll = ft_clamp(mouse->scroll, -20, 20);
	}
	/*if (ed->mouse.click_unhandled
		/*&& ((ed->keystate >> KEYS_CTRLMASK) & 1) == false
		&& ((ed->keystate >> KEYS_SHIFTMASK) & 1) == false)
		mouseclick(ed);*/
}
