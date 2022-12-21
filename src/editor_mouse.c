/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 06:45:42 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/21 17:02:42 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
 

bool	mouse_clicked(t_mouse mouse, int button)
{
	return (mouse.click_unhandled == true && mouse.click_button == button);
}

void	mouse_event(SDL_Event e, t_mouse *mouse)
{
	if (!mouse->relative)
		SDL_GetMouseState(&mouse->pos.x, &mouse->pos.y);
	
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_v)
	{
		//printf("down _ v\n");
		e.type = SDL_MOUSEBUTTONDOWN;
		e.button.button = MOUSE_LEFT;
	}

	if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_v)
	{
		//printf("up _ v\n");
		e.type = SDL_MOUSEBUTTONUP;
		e.button.button = MOUSE_LEFT;
	}
	
	if (e.type == SDL_MOUSEBUTTONDOWN) //TODO: bitshift mouseheldstate
	{
		printf("MOUSE DOWN EVENT \n");
		mouse->click_unhandled = true;
		mouse->click_button = e.button.button;
		mouse->held = e.button.button;
		mouse->heldstate |= (e.button.button == MOUSE_LEFT) << MOUSE_LEFT;
		mouse->heldstate |= (e.button.button == MOUSE_RIGHT) << MOUSE_RIGHT;
	}
	if (e.type == SDL_MOUSEBUTTONUP)
	{
		/*mouse->click_unhandled = true;
		mouse->click_button = e.button.button;*/
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
