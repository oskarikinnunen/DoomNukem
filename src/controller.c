/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:36:01 by raho              #+#    #+#             */
/*   Updated: 2022/11/25 15:26:29 by okinnune         ###   ########.fr       */
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
static void	axis_move(SDL_ControllerAxisEvent caxis, t_hid_info *hid)
{
	int	c;

	if (hid->controller[0].connected && hid->controller[0].instance_id == caxis.which)
		c = 0;
	else
		c = 1;
	if (caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
		hid->controller[c].leftanalog.x = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
		hid->controller[c].leftanalog.y = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		hid->controller[c].rightanalog.x = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		hid->controller[c].rightanalog.y = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
		hid->controller[c].lefttrigger = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
		hid->controller[c].righttrigger = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	
	/* if (controller->lefttrigger > 0.f)
		printf("lefttrigger: %f\n", controller->lefttrigger);
	if (controller->righttrigger > 0.f)
		printf("righttrigger: %f\n", controller->righttrigger); */
}

static void	button_press(SDL_ControllerButtonEvent cbutton, t_hid_info *hid)
{
	int	c;

	//printf("buttondown: %d\n", cbutton.button);
	
	if (hid->controller[0].connected && hid->controller[0].instance_id == cbutton.which)
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
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
		hid->controller[c].leftstick = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
		hid->controller[c].rightstick = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
		hid->controller[c].leftshoulder = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
		hid->controller[c].rightshoulder = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		hid->controller[c].dpad_up = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		hid->controller[c].dpad_down = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		hid->controller[c].dpad_left = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		hid->controller[c].dpad_right = true;
}

static void	button_release(SDL_ControllerButtonEvent cbutton, t_hid_info *hid)
{
	int	c;

	//printf("buttonup: %d\n", cbutton.button);
	
	if (hid->controller[0].connected && hid->controller[0].instance_id == cbutton.which)
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
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
		hid->controller[c].leftstick = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
		hid->controller[c].rightstick = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
		hid->controller[c].leftshoulder = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
		hid->controller[c].rightshoulder = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		hid->controller[c].dpad_up = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		hid->controller[c].dpad_down = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		hid->controller[c].dpad_left = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		hid->controller[c].dpad_right = false;
}

int	controller_events(SDL_Event e, t_hid_info *hid)
{
	SDL_Joystick	*joystick_id;
	int				c;

	if (e.type == SDL_CONTROLLERDEVICEADDED)
	{
		if (e.cdevice.which < MAX_CONTROLLERS)
		{
			if (SDL_IsGameController(e.cdevice.which))
			{
				if (hid->controller[0].connected)
					c = 1;
				else
					c = 0;
				hid->controller[c].gamecontroller = SDL_GameControllerOpen(e.cdevice.which);
				if (!hid->controller[c].gamecontroller)
					error_log(EC_SDL_GAMECONTROLLEROPEN);
				joystick_id = SDL_GameControllerGetJoystick(hid->controller[c].gamecontroller);
				
				// controller's action states could be updated here if needed
				
				hid->controller[c].instance_id = SDL_JoystickInstanceID(joystick_id);
				if (hid->controller[c].instance_id < 0)
					error_log(EC_SDL_JOYSTICKINSTANCEID);
				else
					hid->controller[c].connected = true;
				printf("controller %i connected\n", (c + 1));
			}
			else
				printf("controller not supported\n");
		}
		else
			printf("max controllers: 2\n");
	}
	else if (e.type == SDL_CONTROLLERDEVICEREMOVED)
	{
		if (hid->controller[0].connected && hid->controller[0].instance_id == e.cdevice.which)
			c = 0;
		else
			c = 1;
		hid->controller[c].connected = false;
		SDL_GameControllerClose(hid->controller[c].gamecontroller);
		ft_memset(&hid->controller[c], 0, sizeof(t_controller));
		hid->controller[c].instance_id = -1;
		printf("controller %i disconnected\n", (c + 1));
	}
	else if (e.type == SDL_CONTROLLERAXISMOTION)
		axis_move(e.caxis, hid);
	else if (e.type == SDL_CONTROLLERBUTTONDOWN)
		button_press(e.cbutton, hid);
	else if (e.type == SDL_CONTROLLERBUTTONUP)
		button_release(e.cbutton, hid);
	if (hid->controller[0].guide == true)
		return (game_exit);
	if (hid->controller[0].back == true)
		return (game_switchmode);
	return (game_continue);
}

void updateinput(t_input *input, int keystate, t_mouse m, t_controller *controller)
{
	input->move = vector2_zero();
	input->move.x -= (keystate >> KEYS_LEFTMASK) & 1;
	input->move.x += (keystate >> KEYS_RIGHTMASK) & 1;
	input->move.y += (keystate >> KEYS_DOWNMASK) & 1;
	input->move.y -= (keystate >> KEYS_UPMASK) & 1;
	input->crouch = (keystate >> KEYS_CTRLMASK) & 1;
	input->jump = (keystate >> KEYS_SPACEMASK) & 1;
	input->run = (keystate >> KEYS_SHIFTMASK) & 1;
	input->turn = vector2_mul(point_to_vector2(m.delta), MOUSESPEED);
	input->move.x += controller->leftanalog.x;
	input->move.y += controller->leftanalog.y;
	input->crouch += controller->b;
	input->jump += controller->a;
	input->turn = vector2_add(input->turn, vector2_mul(controller->rightanalog, CONTROLLER_SENS));
	input->aim = m.heldstate >> MOUSE_RIGHT & 1;
	input->aim += controller->lefttrigger;
	input->shoot = m.heldstate >> MOUSE_LEFT & 1;
	input->shoot += controller->righttrigger;
}

void	initialize_controllers(t_hid_info *hid)
{
	SDL_Joystick	*joystick_id;
	int				index;
	int				joysticks;
	
	index = 0;
	joysticks = SDL_NumJoysticks();
	printf("number of joysticks/controllers plugged in: %i\n", joysticks);
	if (joysticks > 0)
	{
		while (index < joysticks && index < MAX_CONTROLLERS)
		{
			if (SDL_IsGameController(index))
			{
				hid->controller[index].gamecontroller = SDL_GameControllerOpen(index);
				if (!hid->controller[index].gamecontroller)
					error_log(EC_SDL_GAMECONTROLLEROPEN);
				joystick_id = SDL_GameControllerGetJoystick(hid->controller[index].gamecontroller);
				hid->controller[index].instance_id = SDL_JoystickInstanceID(joystick_id);
				if (hid->controller[index].instance_id < 0)
					error_log(EC_SDL_JOYSTICKINSTANCEID);
				else
					hid->controller[index].connected = true;
				printf("controller %i connected\n", (index + 1));
			}
			else
				printf("controller not supported\n");
			index++;
		}
		if (SDL_GameControllerEventState(SDL_QUERY) == 0)
			SDL_GameControllerEventState(SDL_ENABLE);
	}		
}