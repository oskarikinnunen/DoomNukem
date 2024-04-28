/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_events.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:59 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 22:11:43 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

// Set the value of the axis to 0 if it doesn't go over the deadzone limit.
static float	dead_zone(float axis)
{
	if (ft_absf(axis) > DEADZONE)
		return (axis);
	else
		return (0.0f);
}

// SDL caxis value range is from -32768 to 32767
static void	controller_axis_move(SDL_ControllerAxisEvent caxis, t_hid_info *hid)
{
	int	c;

	if (hid->controller[0].connected && \
			hid->controller[0].instance_id == caxis.which)
		c = 0;
	else
		c = 1;
	if (caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
		hid->controller[c].left_analog.x = \
				dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
		hid->controller[c].left_analog.y = \
				dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		hid->controller[c].right_analog.x = \
				dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		hid->controller[c].right_analog.y = \
				dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
		hid->controller[c].left_trigger = \
				dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
		hid->controller[c].right_trigger = \
				dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
}

static void	handle_new_controller(SDL_Event e, t_hid_info *hid, int c)
{
	SDL_Joystick	*joystick_id;

	if (SDL_IsGameController(e.cdevice.which))
	{
		if (hid->controller[0].connected)
			c = 1;
		else
			c = 0;
		hid->controller[c].gamecontroller = \
				SDL_GameControllerOpen(e.cdevice.which);
		if (!hid->controller[c].gamecontroller)
			doomlog(LOG_EC_SDL_GAMECONTROLLEROPEN, NULL);
		joystick_id = SDL_GameControllerGetJoystick(
				hid->controller[c].gamecontroller);
		hid->controller[c].instance_id = SDL_JoystickInstanceID(joystick_id);
		if (hid->controller[c].instance_id < 0)
			doomlog(LOG_EC_SDL_JOYSTICKINSTANCEID, NULL);
		else
			hid->controller[c].connected = true;
		doomlog_mul(LOG_NORMAL, (char *[4]){
			"controller", s_itoa(c + 1), "connected", NULL});
	}
	else
		doomlog(LOG_WARNING, "controller not supported");
}

void	handle_removed_controller(SDL_Event e, t_hid_info *hid)
{
	int	c;

	if (hid->controller[0].connected && \
			hid->controller[0].instance_id == e.cdevice.which)
		c = 0;
	else
		c = 1;
	hid->controller[c].connected = false;
	SDL_GameControllerClose(hid->controller[c].gamecontroller);
	ft_memset(&hid->controller[c], 0, sizeof(t_controller));
	hid->controller[c].instance_id = -1;
	doomlog_mul(LOG_NORMAL, (char *[4]){\
				"controller", s_itoa(c + 1), "disconnected", NULL});
}

void	controller_events(SDL_Event e, t_hid_info *hid)
{
	SDL_Joystick	*joystick_id;

	if (e.type == SDL_CONTROLLERDEVICEADDED)
	{
		if (e.cdevice.which < MAX_CONTROLLERS)
			handle_new_controller(e, hid, 0);
		else
			doomlog(LOG_WARNING, "max controllers: 2");
	}
	else if (e.type == SDL_CONTROLLERDEVICEREMOVED)
		handle_removed_controller(e, hid);
	else if (e.type == SDL_CONTROLLERAXISMOTION)
		controller_axis_move(e.caxis, hid);
	else if (e.type == SDL_CONTROLLERBUTTONDOWN)
		controller_button_press(e.cbutton, hid);
	else if (e.type == SDL_CONTROLLERBUTTONUP)
		controller_button_release(e.cbutton, hid);
	if (hid->controller[0].guide == true)
		return ;
	if (hid->controller[0].back == true)
		return ;
}
