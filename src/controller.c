/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:36:01 by raho              #+#    #+#             */
/*   Updated: 2022/11/14 23:00:13 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"

// Set the value of the axis to 0 if it doesn't go over the deadzone limit.
static float	dead_zone(float axis)
{
	if (ft_absf(axis) > DEADZONE)
		return (axis);
	else
		return (0.0f);
}

// SDL caxis value range is from -32768 to 32767
static void	axis_move(SDL_ControllerAxisEvent caxis, t_game *game)
{
	int	c;

	if (game->controller[0].connected && game->controller[0].instance_id == caxis.which)
		c = 0;
	else
		c = 1;
	if (caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
		game->controller[c].leftanalog.x = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
		game->controller[c].leftanalog.y = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		game->controller[c].rightanalog.x = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		game->controller[c].rightanalog.y = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
		game->controller[c].lefttrigger = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
		game->controller[c].righttrigger = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	
	/* if (controller->lefttrigger > 0.f)
		printf("lefttrigger: %f\n", controller->lefttrigger);
	if (controller->righttrigger > 0.f)
		printf("righttrigger: %f\n", controller->righttrigger); */
}

static void	button_press(SDL_ControllerButtonEvent cbutton, t_game *game)
{
	int	c;

	//printf("buttondown: %d\n", cbutton.button);
	
	if (game->controller[0].connected && game->controller[0].instance_id == cbutton.which)
		c = 0;
	else
		c = 1;

	if (cbutton.button == SDL_CONTROLLER_BUTTON_A)
		game->controller[c].a = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_B)
		game->controller[c].b = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_X)
		game->controller[c].x = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_Y)
		game->controller[c].y = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
		game->controller[c].back = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
		game->controller[c].guide = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_START)
		game->controller[c].start = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
		game->controller[c].leftstick = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
		game->controller[c].rightstick = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
		game->controller[c].leftshoulder = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
		game->controller[c].rightshoulder = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		game->controller[c].dpad_up = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		game->controller[c].dpad_down = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		game->controller[c].dpad_left = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		game->controller[c].dpad_right = true;
}

static void	button_release(SDL_ControllerButtonEvent cbutton, t_game *game)
{
	int	c;

	//printf("buttonup: %d\n", cbutton.button);
	
	if (game->controller[0].connected && game->controller[0].instance_id == cbutton.which)
		c = 0;
	else
		c = 1;

	if (cbutton.button == SDL_CONTROLLER_BUTTON_A)
		game->controller[c].a = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_B)
		game->controller[c].b = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_X)
		game->controller[c].x = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_Y)
		game->controller[c].y = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
		game->controller[c].back = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
		game->controller[c].guide = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_START)
		game->controller[c].start = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
		game->controller[c].leftstick = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
		game->controller[c].rightstick = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
		game->controller[c].leftshoulder = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
		game->controller[c].rightshoulder = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		game->controller[c].dpad_up = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		game->controller[c].dpad_down = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		game->controller[c].dpad_left = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		game->controller[c].dpad_right = false;
}

int	controller_events(SDL_Event e, t_game *game)
{
	SDL_Joystick	*joystick_id;
	int				c;

	if (e.type == SDL_CONTROLLERDEVICEADDED)
	{
		if (e.cdevice.which < MAX_CONTROLLERS)
		{
			if (SDL_IsGameController(e.cdevice.which))
			{
				if (game->controller[0].connected)
					c = 1;
				else
					c = 0;
				game->controller[c].gamecontroller = SDL_GameControllerOpen(e.cdevice.which);
				if (!game->controller[c].gamecontroller)
					error_log(EC_SDL_GAMECONTROLLEROPEN);
				joystick_id = SDL_GameControllerGetJoystick(game->controller[c].gamecontroller);
				
				// controller's action states could be updated here if needed
				
				game->controller[c].instance_id = SDL_JoystickInstanceID(joystick_id);
				if (game->controller[c].instance_id < 0)
					error_log(EC_SDL_JOYSTICKINSTANCEID);
				else
					game->controller[c].connected = true;
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
		if (game->controller[0].connected && game->controller[0].instance_id == e.cdevice.which)
			c = 0;
		else
			c = 1;
		game->controller[c].connected = false;
		SDL_GameControllerClose(game->controller[c].gamecontroller);
		ft_memset(&game->controller[c], 0, sizeof(t_controller));
		game->controller[c].instance_id = -1;
		printf("controller %i disconnected\n", (c + 1));
	}
	else if (e.type == SDL_CONTROLLERAXISMOTION)
		axis_move(e.caxis, game);
	else if (e.type == SDL_CONTROLLERBUTTONDOWN)
		button_press(e.cbutton, game);
	else if (e.type == SDL_CONTROLLERBUTTONUP)
		button_release(e.cbutton, game);
	if (game->controller[0].guide == true)
		return (game_exit);
	if (game->controller[0].back == true)
		return (game_switchmode);
	return (game_continue);
}

void	initialize_controllers(t_game *game)
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
				game->controller[index].gamecontroller = SDL_GameControllerOpen(index);
				if (!game->controller[index].gamecontroller)
					error_log(EC_SDL_GAMECONTROLLEROPEN);
				joystick_id = SDL_GameControllerGetJoystick(game->controller[index].gamecontroller);
				game->controller[index].instance_id = SDL_JoystickInstanceID(joystick_id);
				if (game->controller[index].instance_id < 0)
					error_log(EC_SDL_JOYSTICKINSTANCEID);
				else
					game->controller[index].connected = true;
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