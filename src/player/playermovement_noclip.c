/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playermovement_noclip.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 18:12:39 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 23:08:34 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"

static t_vector3	sanitize_z(t_vector3 vec)
{
	return ((t_vector3){vec.x, vec.y, 0.0f});
}

static t_vector3	normalized_inputvector(t_input input, t_player player)
{
	t_vector3	movement;
	t_vector3	forward;
	t_vector3	right;
	t_vector3	strafe;
	float		speed;

	movement = vector3_zero();
	forward = player.look_dir;
	forward.z = 0.0f;
	forward = vector3_normalise(forward);
	movement = vector3_mul(sanitize_z(forward), -input.move.y);
	right = vector3_cross_product(forward, vector3_up());
	strafe = vector3_mul(right, input.move.x);
	movement = vector3_add(movement, strafe);
	movement = vector3_clamp_magnitude(movement, 1.0f);
	movement.z = (float)input.jump;
	movement.z -= (float)input.crouch;
	return (movement);
}

void	player_movement_noclip(t_player *player, t_world *world)
{
	t_vector3		move_vec;
	float			speed;

	player->transform.rotation = vector3_sub(
			player->transform.rotation, v2tov3(player->input.turn));
	player->transform.rotation.y = ft_clampf(player->transform.rotation.y,
			-RAD90 * 0.99f, RAD90 * 0.99f);
	player->look_dir = look_direction((t_vector2){player->transform.rotation.x,
			player->transform.rotation.y});
	move_vec = normalized_inputvector(player->input, *player);
	speed = EDITOR_FLYSPEED
		+ (player->input.run * (EDITOR_FLYFASTSPEED - EDITOR_FLYSPEED));
	move_vec = vector3_mul(move_vec, speed * world->clock.delta);
	player->transform.position = vector3_add(player->transform.position,
			move_vec);
}
