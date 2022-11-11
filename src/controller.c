/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:36:01 by raho              #+#    #+#             */
/*   Updated: 2022/11/11 18:45:27 by raho             ###   ########.fr       */
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
static void	axis_move(SDL_ControllerAxisEvent caxis, t_controller *controller)
{

	if (caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
		controller->leftanalog.x = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
		controller->leftanalog.y = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		controller->rightanalog.x = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		controller->rightanalog.y = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
		controller->lefttrigger = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	if (caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
		controller->righttrigger = dead_zone(ft_clampf((caxis.value / 32767.f), -1.f, 1.f));
	
	if (controller->lefttrigger > 0.f)
		printf("lefttrigger: %f\n", controller->lefttrigger);
	if (controller->righttrigger > 0.f)
		printf("righttrigger: %f\n", controller->righttrigger);
}

static void	button_press(SDL_ControllerButtonEvent cbutton, t_controller *controller)
{
	printf("buttondown: %d\n", cbutton.button);

	if (cbutton.button == SDL_CONTROLLER_BUTTON_A)
		controller->a = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_B)
		controller->b = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_X)
		controller->x = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_Y)
		controller->y = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
		controller->back = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
		controller->guide = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_START)
		controller->start = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
		controller->leftstick = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
		controller->rightstick = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
		controller->leftshoulder = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
		controller->rightshoulder = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		controller->dpad_up = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		controller->dpad_down = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		controller->dpad_left = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		controller->dpad_right = true;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_MAX)
		controller->max = true;
}

static void	button_release(SDL_ControllerButtonEvent cbutton, t_controller *controller)
{
	printf("buttonup: %d\n", cbutton.button);

	if (cbutton.button == SDL_CONTROLLER_BUTTON_A)
		controller->a = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_B)
		controller->b = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_X)
		controller->x = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_Y)
		controller->y = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
		controller->back = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
		controller->guide = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_START)
		controller->start = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
		controller->leftstick = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
		controller->rightstick = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
		controller->leftshoulder = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
		controller->rightshoulder = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		controller->dpad_up = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		controller->dpad_down = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		controller->dpad_left = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		controller->dpad_right = false;
	if (cbutton.button == SDL_CONTROLLER_BUTTON_MAX)
		controller->max = false;
}

int	controller_events(SDL_Event e, t_controller *controller)
{
	if (e.type == SDL_CONTROLLERAXISMOTION)
		axis_move(e.caxis, controller);
	else if (e.type == SDL_CONTROLLERBUTTONDOWN)
		button_press(e.cbutton, controller);
	else if (e.type == SDL_CONTROLLERBUTTONUP)
		button_release(e.cbutton, controller);
	if (controller->guide == true)
		return (game_exit);
	if (controller->back == true)
		return (game_switchmode);
	return (game_continue);
}
