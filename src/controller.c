/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:36:01 by raho              #+#    #+#             */
/*   Updated: 2022/11/09 19:52:22 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	analog_sticks(SDL_JoyAxisEvent jaxis, t_controller *controller)
{
	//if ((jaxis.value < -3200) || (jaxis.value > 3200))
	
	
	// Left analog stick
	// left-right
	if (jaxis.axis == 0)
	{
		//printf("left stick - left-right: jaxis.axis: %d\n", jaxis.value);
		if (jaxis.value < -3200)
			controller->leftanalog.x = -1;
		if (jaxis.value > 3200)
			controller->leftanalog.x = 1;
		else
			controller->leftanalog.x = 0;
	}
	
	// up-down
	if (jaxis.axis == 1)
	{
		//printf("left stick - up-down: jaxis.axis: %d\n", jaxis.value);
		if (jaxis.value < -3200)
			controller->leftanalog.y = -1;
		if (jaxis.value > 3200)
			controller->leftanalog.y = 1;
		else
			controller->leftanalog.y = 0;	
	}


	// Right analog stick
	// left-right
	if (jaxis.axis == 2)
	{
		//printf("right stick - left-right: jaxis.axis: %d\n", jaxis.value);
		if (jaxis.value < -3200)
			controller->rightanalog.x = -1;
		if (jaxis.value > 3200)
			controller->rightanalog.x = 1;
		else
			controller->rightanalog.x = 0;
	}
			
	// up-down
	if (jaxis.axis == 5)
	{
		//printf("right stick - up-down: jaxis.axis: %d\n", jaxis.value);
		if (jaxis.value < -3200)
			controller->rightanalog.y = -1;
		if (jaxis.value > 3200)
			controller->rightanalog.y = 1;
		else
			controller->rightanalog.y = 0;
	}
}

static void	button_press(SDL_JoyButtonEvent jbutton, t_controller *controller)
{
	// printf("buttondown: %d\n", e.jbutton.button);
		
	// square
	if (jbutton.button == 0)
		controller->square = true;
		
	// cross
	if (jbutton.button == 1)
		controller->cross = true;
	
	// circle
	if (jbutton.button == 2)
		controller->circle = true;
	
	// triangle
	if (jbutton.button == 3)
		controller->triangle = true;
	
	// l1
	if (jbutton.button == 4)
		controller->l1 = true;

	// r1
	if (jbutton.button == 5)
		controller->r1 = true;

	// l2
	if (jbutton.button == 6)
		controller->l2 = true;

	// r2
	if (jbutton.button == 7)
		controller->r2 = true;

	// share
	if (jbutton.button == 8)
		controller->share = true;

	// options
	if (jbutton.button == 9)
		controller->options = true;
	
	// l3
	if (jbutton.button == 10)
		controller->l3 = true;

	// r3
	if (jbutton.button == 11)
		controller->r3 = true;

	// home
	if (jbutton.button == 12)
		controller->home = true;

	// touchpad
	if (jbutton.button == 13)
		controller->touchpad = true;
}

static void	button_release(SDL_JoyButtonEvent jbutton, t_controller *controller)
{
	// printf("buttonup: %d\n", e.jbutton.button);
		
	// square
	if (jbutton.button == 0)
		controller->square = false;
	
	// cross
	if (jbutton.button == 1)
		controller->cross = false;
	
	// circle
	if (jbutton.button == 2)
		controller->circle = false;
	
	// triangle
	if (jbutton.button == 3)
		controller->triangle = false;
	
	// l1
	if (jbutton.button == 4)
		controller->l1 = false;

	// r1
	if (jbutton.button == 5)
		controller->r1 = false;

	// l2
	if (jbutton.button == 6)
		controller->l2 = false;

	// r2
	if (jbutton.button == 7)
		controller->r2 = false;

	// share
	if (jbutton.button == 8)
		controller->share = false;

	// options
	if (jbutton.button == 9)
		controller->options = false;
	
	// l3
	if (jbutton.button == 10)
		controller->l3 = false;

	// r3
	if (jbutton.button == 11)
		controller->r3 = false;

	// home
	if (jbutton.button == 12)
		controller->home = false;

	// touchpad
	if (jbutton.button == 13)
		controller->touchpad = false;
}

int	controller_events(SDL_Event e, t_controller *controller)
{
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
	// 0: middle	1: up		2: right		4: down		8: left
	// diagonals are sum of the neighbors (example: down-left would be 12)
	else if (e.type == SDL_JOYHATMOTION)
	{
		printf("joyhat value: %d\n", e.jhat.value);
		controller->dpad = e.jhat.value;
	}

	if (controller->home == true)
		return (game_exit);
	if (controller->touchpad == true)
		return (game_switchmode);
	return (game_continue);
}
