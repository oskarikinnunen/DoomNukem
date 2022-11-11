/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:36:01 by raho              #+#    #+#             */
/*   Updated: 2022/11/11 17:02:33 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"

// On mac ps4 controller's color indicator didn't light up but SDL recognized it as a joystick.
// On linux VM I had to enable the controller through usb input settings -> the color indicator light up
// and the values for axis and the buttons are different.


// Set the value of the axis to 0 if it doesn't go over the deadzone limit.
static float	dead_zone(float axis)
{
	if (ft_absf(axis) > DEADZONE)
		return (axis);
	else
		return (0.0f);
}

static void	analog_sticks(SDL_JoyAxisEvent jaxis, t_controller *controller)
{
	//if ((jaxis.value < -3200) || (jaxis.value > 3200))
	
	//printf("jaxis.axis %i\n", jaxis.axis);
	
	// Left analog stick
	// left-right
	if (jaxis.axis == 0)
	{
		//printf("left stick - left-right: jaxis.axis: %d\n", jaxis.value);

		// SDL jaxis value range is from -32768 to 32767
		controller->leftanalog.x = dead_zone(ft_clampf((jaxis.value / 32767.f), -1.f, 1.f));
		//printf("left stick - left-right: jaxis.axis: %f\n", controller->leftanalog.x);
	}
	
	// up-down
	if (jaxis.axis == 1)
	{
		//printf("left stick - up-down: jaxis.axis: %d\n", jaxis.value);

		// SDL jaxis value range is from -32768 to 32767
		controller->leftanalog.y = dead_zone(ft_clampf((jaxis.value / 32767.f), -1.f, 1.f));
		//printf("left stick - up-down: jaxis.axis: %f\n", controller->leftanalog.y);
	}


	// Right analog stick
	// left-right
	if (jaxis.axis == 2) // 3 for linux. on mac 3 is for triggers
	
	{
		//printf("right stick - left-right: jaxis.axis: %d\n", jaxis.value);

		// SDL jaxis value range is from -32768 to 32767
		controller->rightanalog.x = dead_zone(ft_clampf((jaxis.value / 32767.f), -1.f, 1.f));
		//printf("right stick - left-right: jaxis.axis: %f\n", controller->rightanalog.x);
	}
			
	// up-down
	if (jaxis.axis == 5) // 4 for linux. on mac 4 is for triggers
	{
		//printf("right stick - up-down: jaxis.axis: %d\n", jaxis.value);

		// SDL jaxis value range is from -32768 to 32767
		controller->rightanalog.y = dead_zone(ft_clampf((jaxis.value / 32767.f), -1.f, 1.f));
		//printf("right stick - up-down: jaxis.axis: %f\n", controller->rightanalog.y);
	}

}

static void	button_press(SDL_JoyButtonEvent jbutton, t_controller *controller)
{
	printf("buttondown: %d\n", jbutton.button);

	if (jbutton.button == 0)
		controller->square = true;
	if (jbutton.button == 1)
		controller->cross = true;
	if (jbutton.button == 2)
		controller->circle = true;
	if (jbutton.button == 3)
		controller->triangle = true;
	if (jbutton.button == 4)
		controller->l1 = true;
	if (jbutton.button == 5)
		controller->r1 = true;
	if (jbutton.button == 6)
		controller->l2 = true;
	if (jbutton.button == 7)
		controller->r2 = true;
	if (jbutton.button == 8)
		controller->share = true;
	if (jbutton.button == 9)
		controller->options = true;
	if (jbutton.button == 10)
		controller->l3 = true;
	if (jbutton.button == 11)
		controller->r3 = true;
	if (jbutton.button == 12)
		controller->home = true;
	if (jbutton.button == 13)
		controller->touchpad = true;
}

static void	button_release(SDL_JoyButtonEvent jbutton, t_controller *controller)
{
	printf("buttonup: %d\n", jbutton.button);

	if (jbutton.button == 0)
		controller->square = false;
	if (jbutton.button == 1)
		controller->cross = false;
	if (jbutton.button == 2)
		controller->circle = false;
	if (jbutton.button == 3)
		controller->triangle = false;
	if (jbutton.button == 4)
		controller->l1 = false;
	if (jbutton.button == 5)
		controller->r1 = false;
	if (jbutton.button == 6)
		controller->l2 = false;
	if (jbutton.button == 7)
		controller->r2 = false;
	if (jbutton.button == 8)
		controller->share = false;
	if (jbutton.button == 9)
		controller->options = false;
	if (jbutton.button == 10)
		controller->l3 = false;
	if (jbutton.button == 11)
		controller->r3 = false;
	if (jbutton.button == 12)
		controller->home = false;
	if (jbutton.button == 13)
		controller->touchpad = false;
}


// 0: middle	1: up		2: right		4: down		8: left
// diagonals are sum of the neighbors (example: down-left would be 12)
void	directional_pad(SDL_JoyHatEvent jhat, t_controller *controller)
{
	printf("joyhat value: %d\n", jhat.value);

	controller->dpad = jhat.value;

	// up
	if (jhat.value == 1)
		;

	// up-right
	if (jhat.value == 3)
		;

	// right
	if (jhat.value == 2)
		;

	// down-right
	if (jhat.value == 6)
		;

	// down
	if (jhat.value == 4)
		;

	// down-left
	if (jhat.value == 12)
		;

	// left
	if (jhat.value == 8)
		;

	// up-left
	if (jhat.value == 9)
		;
}

int	controller_events(SDL_Event e, t_controller *controller, t_platform platform)
{
	//printf("e.type: %i\n", e.type);

	// Analog sticks
	if (e.type == SDL_JOYAXISMOTION)
		analog_sticks(e.jaxis, controller);

	// Pressing buttons
	else if (e.type == SDL_JOYBUTTONDOWN)
		button_press(e.jbutton, controller);

	// Releasing buttons
	else if (e.type == SDL_JOYBUTTONUP)
		button_release(e.jbutton, controller);

	// Pressing/releasing directional pad buttons
	else if (e.type == SDL_JOYHATMOTION)
		directional_pad(e.jhat, controller);

	if (controller->home == true)
		return (game_exit);
	if (controller->touchpad == true)
		return (game_switchmode);
	return (game_continue);
}
