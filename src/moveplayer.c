/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/09 21:16:51 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"

 // Might need the whole gamecontext but I'm trying to avoid it, (trying to avoid global state)
 // TODO: normalize movement vector
/*
static t_vector3	movementvector(int32_t keystate, float angle)
{
	t_vector3	movement;

	movement = vector3_zero();
	if ((keystate >> KEYS_UPMASK) & 1) 
	{
		movement.x += sin(angle);
		movement.y += cos(angle);
	}
	if ((keystate >> KEYS_DOWNMASK) & 1)
	{
		movement.x -= sin(angle);
		movement.y -= cos(angle);
	}
	// strafe
	if ((keystate >> KEYS_LEFTMASK) & 1)
	{
		movement.x += sin(angle + RAD90);
		movement.y += cos(angle + RAD90);
	}
	if ((keystate >> KEYS_RIGHTMASK) & 1)
	{
		movement.x += -sin(angle + RAD90);
		movement.y += -cos(angle + RAD90);
	}
	//flying
	if ((keystate >> KEYS_SPACEMASK) & 1)
	{
		movement.z += 0.25f;
	}
	if ((keystate >> KEYS_CTRLMASK) & 1)
	{
		movement.z -= 0.25f;
	}
	movement = vector3_clamp_magnitude(movement, MAXMOVEMENTSPEED);
	return (movement);
}
*/

/* Previous working version before implementing updateinput */
/*
static t_vector3	player_movementvector(int32_t keystate, t_vector3 lookdir)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;

	movement = vector3_zero();
	//forward.z = 0;
	forward = vector3_mul_vector3(lookdir, (t_vector3){1.0f, 1.0f, 0.0f});
	forward = vector3_normalise(forward);
	if ((keystate >> KEYS_UPMASK) & 1) 
		movement = vector3_add(movement, forward);
	if ((keystate >> KEYS_DOWNMASK) & 1)
		movement = vector3_sub(movement, forward);
	if ((keystate >> KEYS_LEFTMASK) & 1)
		movement = vector3_sub(movement,
			vector3_crossproduct(forward, vector3_up()));
	if ((keystate >> KEYS_RIGHTMASK) & 1)
		movement = vector3_add(movement,
			vector3_crossproduct(forward, vector3_up()));
	speed = 1.0f + (float)((keystate >> KEYS_SHIFTMASK) & 1);
	movement.z += 1.5f * ((keystate >> KEYS_SPACEMASK) & 1);
	movement.z -= 1.5f * ((keystate >> KEYS_CTRLMASK) & 1);
	movement = vector3_mul(movement, speed);
	movement = vector3_clamp_magnitude(movement, speed);
	return (movement);
}
*/

static t_vector3	player_movementvector(t_input input, t_vector3 lookdir)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;

	movement = vector3_zero();
	//forward.z = 0;
	forward = vector3_mul_vector3(lookdir, (t_vector3){1.0f, 1.0f, 0.0f});
	forward = vector3_normalise(forward);
	//printf("input.move.y: %f	input.move.x: %f\n", input.move.y, input.move.x);
	if (input.move.y < 0) 
		movement = vector3_add(movement, forward);
	if (input.move.y > 0)
		movement = vector3_sub(movement, forward);
	if (input.move.x < 0)
		movement = vector3_sub(movement,
			vector3_crossproduct(forward, vector3_up()));
	if (input.move.x > 0)
		movement = vector3_add(movement,
			vector3_crossproduct(forward, vector3_up()));
	speed = 1.0f + input.run;
	movement.z += 1.5f * input.jump;
	movement.z -= 1.5f * input.crouch;
	movement = vector3_mul(movement, speed);
	movement = vector3_clamp_magnitude(movement, speed);
	return (movement);
}

void	moveplayer(t_game *game)
{
	t_vector3	move_vector;
	t_vector3	potential_pos; //Unused right now, will be used when collision is reimplemented
	float	angle;

	move_vector = vector3_zero();
	angle = 0;
	angle -= game->mouse.delta.x * MOUSESPEED;
	angle *= game->clock.delta;
	game->player.angle.x += angle;
	angle = 0;
	angle -= game->mouse.delta.y * MOUSESPEED;
	angle *= game->clock.delta;
	game->player.angle.y += angle;
	game->player.angle.y = ft_clampf(game->player.angle.y, -RAD90 * 0.99f, RAD90 * 0.99f);
	game->player.lookdir = lookdirection(game->player.angle);
	move_vector = player_movementvector(game->input, game->player.lookdir);
	move_vector = vector3_mul(move_vector, game->clock.delta * MOVESPEED);
	game->player.position = vector3_add(game->player.position, move_vector);
}