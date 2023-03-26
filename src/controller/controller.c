/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:36:01 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 20:59:50 by raho             ###   ########.fr       */
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
}

static void	controller_movement(t_input *input, t_controller *controller)
{
	input->move.x += controller->left_analog.x;
	input->move.y += controller->left_analog.y;
	input->turn = vector2_add(input->turn,
			vector2_mul(controller->right_analog, 0.05f));
	input->crouch += controller->b;
	input->jump += controller->a;
	input->aim += controller->left_trigger;
	input->shoot += controller->right_trigger;
}

void	update_input(t_input *input, t_hid_info info)
{
	input->move = vector2_zero();
	keyboard_movement(input, info.mouse, info.keystate, info.alphakey_pressed);
	controller_movement(input, info.controller);
}
