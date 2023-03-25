/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 06:45:42 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 15:50:30 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

void	force_mouseunlock(t_hid_info *hid)
{
	hid->mouse.relative = false;
	SDL_SetRelativeMouseMode(hid->mouse.relative);
	hid->mouse.delta = point_zero();
}

void	force_mouselock(t_hid_info *hid)
{
	hid->mouse.relative = true;
	SDL_SetRelativeMouseMode(hid->mouse.relative);
	hid->mouse.delta = point_zero();
	hid->mouse.pos = point_zero();
}

bool	mouse_clicked(t_mouse mouse, int button)
{
	return (mouse.click_unhandled == true && mouse.click_button == button);
}

void	mouse_event(SDL_Event e, t_mouse *mouse)
{
	if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_v)
	{
		e.type = SDL_MOUSEBUTTONUP;
		e.button.button = MOUSE_LEFT;
	}
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		mouse->click_unhandled = true;
		mouse->click_button = e.button.button;
		mouse->held = e.button.button;
		mouse->heldstate |= (e.button.button == MOUSE_LEFT) << MOUSE_LEFT;
		mouse->heldstate |= (e.button.button == MOUSE_RIGHT) << MOUSE_RIGHT;
	}
	if (e.type == SDL_MOUSEBUTTONUP)
	{
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
}
