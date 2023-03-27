/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_events_buttons.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:59 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:00:23 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	controller_button_press2(SDL_ControllerButtonEvent cbutton,
							t_hid_info *hid, int c)
{
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
		hid->controller[c].left_stick = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
		hid->controller[c].right_stick = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
		hid->controller[c].left_shoulder = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
		hid->controller[c].right_shoulder = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		hid->controller[c].dpad_up = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		hid->controller[c].dpad_down = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		hid->controller[c].dpad_left = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		hid->controller[c].dpad_right = true;
}

void	controller_button_press(SDL_ControllerButtonEvent cbutton,
								t_hid_info *hid)
{
	int	c;

	if (hid->controller[0].connected && \
			hid->controller[0].instance_id == cbutton.which)
		c = 0;
	else
		c = 1;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_A)
		hid->controller[c].a = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_B)
		hid->controller[c].b = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_X)
		hid->controller[c].x = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_Y)
		hid->controller[c].y = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
		hid->controller[c].back = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
		hid->controller[c].guide = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_START)
		hid->controller[c].start = true;
	controller_button_press2(cbutton, hid, c);
}

static void	controller_button_release2(SDL_ControllerButtonEvent cbutton,
							t_hid_info *hid, int c)
{
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
		hid->controller[c].left_stick = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
		hid->controller[c].right_stick = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
		hid->controller[c].left_shoulder = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
		hid->controller[c].right_shoulder = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		hid->controller[c].dpad_up = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		hid->controller[c].dpad_down = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		hid->controller[c].dpad_left = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		hid->controller[c].dpad_right = false;
}

void	controller_button_release(SDL_ControllerButtonEvent cbutton,
										t_hid_info *hid)
{
	int	c;

	if (hid->controller[0].connected && \
			hid->controller[0].instance_id == cbutton.which)
		c = 0;
	else
		c = 1;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_A)
		hid->controller[c].a = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_B)
		hid->controller[c].b = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_X)
		hid->controller[c].x = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_Y)
		hid->controller[c].y = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
		hid->controller[c].back = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
		hid->controller[c].guide = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_START)
		hid->controller[c].start = false;
	controller_button_release2(cbutton, hid, c);
}
