/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:36:01 by raho              #+#    #+#             */
/*   Updated: 2023/03/01 21:09:02 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"

static void	keyboard_movement(t_input *input,
							t_mouse mouse, int keystate, int akeystate)
{
	input->move.x -= (keystate >> KEYS_LEFTMASK) & 1;
	input->move.x += (keystate >> KEYS_RIGHTMASK) & 1;
	input->move.y += (keystate >> KEYS_DOWNMASK) & 1;
	input->move.y -= (keystate >> KEYS_UPMASK) & 1;
	input->crouch = (keystate >> KEYS_CTRLMASK) & 1;
	input->jump = (keystate >> KEYS_SPACEMASK) & 1;
	input->run = (keystate >> KEYS_SHIFTMASK) & 1;
	input->turn = vector2_mul(point_to_vector2(mouse.delta), MOUSESPEED);
	input->aim = mouse.heldstate >> MOUSE_RIGHT & 1;
	input->shoot = mouse.heldstate >> MOUSE_LEFT & 1;
	input->use = check_alpha_key(akeystate, 'e');
	input->prevgun = check_alpha_key(akeystate, 'q');
	input->nextgun = check_alpha_key(akeystate, 'e');
	input->reload = check_alpha_key(akeystate, 'r');
	if (input->use)
	{
		printf("input use %i \n", input->use);
		printf("akey %i \n", check_alpha_key(akeystate, 'e'));
	}
}

static void	controller_movement(t_input *input, t_controller *controller)
{
	input->move.x += controller->leftanalog.x;
	input->move.y += controller->leftanalog.y;
	input->crouch += controller->b;
	input->jump += controller->a;
	input->aim += controller->lefttrigger;
	input->shoot += controller->righttrigger;
}

void	updateinput_new(t_input *input, t_hid_info info)
{
	input->move = vector2_zero();
	keyboard_movement(input, info.mouse, info.keystate, info.alphakey_pressed);
	controller_movement(input, info.controller);
}

void	updateinput(t_input *input, int keystate,
					t_mouse m, t_controller *controller)
{
	//controller[1].
	input->move = vector2_zero();
	input->turn = vector2_mul(point_to_vector2(m.delta), MOUSESPEED);
	input->move.x += controller->leftanalog.x;
	input->move.y += controller->leftanalog.y;
	input->crouch += controller->b;
	input->jump += controller->a;
	input->turn = vector2_add(input->turn, \
					vector2_mul(controller->rightanalog, CONTROLLER_SENS));
	input->aim = m.heldstate >> MOUSE_RIGHT & 1;
	input->aim += controller->lefttrigger;
	input->shoot = m.heldstate >> MOUSE_LEFT & 1;
	input->shoot += controller->righttrigger;
}
