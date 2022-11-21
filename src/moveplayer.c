/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/21 18:10:53 by okinnune         ###   ########.fr       */
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
	forward = vector3_mul_vector3(player->lookdir, (t_vector3){1.0f, 1.0f, 0.0f});
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
	if ((keystate >> KEYS_CTRLMASK) & 1)
	{
		speed *= 0.5f;
		player->height = 30.0f;
	}
	else
		player->height = 60.0f;
		
	if (player->jump.active)
		movement.z += cos(player->jump.lerp * 4.5f) * 1.8f;
	else if (player->position.z > player->height && movement.z >= 0.0f)
		movement.z -= 4.0f;
	else if ((keystate >> KEYS_SPACEMASK) & 1 && !player->jump.active)
		start_anim(&player->jump, anim_forwards);
	movement = vector3_mul_vector3(movement, (t_vector3){speed, speed, 1.0f});
	movement = vector3_clamp_magnitude(movement, speed);
	return (movement);
}
*/

static t_vector3	player_movementvector(t_input input, t_player player)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;

	movement = vector3_zero();
	//forward.z = 0;
	forward = vector3_mul_vector3(player.lookdir, (t_vector3){1.0f, 1.0f, 0.0f});
	forward = vector3_normalise(forward);
	movement = vector3_mul(forward, -input.move.y);
	t_vector3 right = vector3_crossproduct(forward, vector3_up());
	t_vector3 strafe = vector3_mul(right, input.move.x);
	movement = vector3_add(movement, strafe);
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
	t_vector2 delta_angle = vector2_mul(game->input.turn, game->clock.delta);
	game->player.angle = vector2_sub(game->player.angle, delta_angle);
	game->player.angle.y = ft_clampf(game->player.angle.y, -RAD90 * 0.99f, RAD90 * 0.99f);
	game->player.lookdir = lookdirection(game->player.angle);
	move_vector = player_movementvector(game->input, game->player);
	move_vector = vector3_mul(move_vector, game->clock.delta * MOVESPEED);
	game->player.position = vector3_add(game->player.position, move_vector);
	game->player.position.z = ft_clampf(game->player.position.z, game->player.height, 1000.0f);
}