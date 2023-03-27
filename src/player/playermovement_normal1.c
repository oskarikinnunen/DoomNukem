/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playermovement_normal1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:22:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:25:47 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"

void	player_rotate(t_player *player)
{
	player->transform.rotation = vector3_sub(player->transform.rotation,
			v2tov3(player->input.turn));
	player->transform.rotation.y = ft_clampf(player->transform.rotation.y,
			-RAD90 * 0.99f, RAD90 * 0.99f);
	player->look_dir = look_direction((t_vector2){player->transform.rotation.x,
			player->transform.rotation.y});
}

static t_vector2	normalized_inputvector(t_input input, t_player player)
{
	t_vector2	movement;
	t_vector3	forward;
	t_vector3	right;
	t_vector3	strafe;
	float		speed;

	movement = vector2_zero();
	forward = player.look_dir;
	forward.z = 0.0f;
	forward = vector3_normalise(forward);
	movement = vector2_mul(v3tov2(forward), -input.move.y);
	right = vector3_cross_product(forward, vector3_up());
	strafe = vector3_mul(right, input.move.x);
	movement = vector2_add(movement, v3tov2(strafe));
	movement = vector2_clamp_magnitude(movement, 1.0f);
	return (movement);
}

void	player_move(t_player *player, t_world *world)
{
	t_vector2	velocity_xy;

	velocity_xy = vector2_mul(normalized_inputvector
			(player->input, *player), PLAYER_ACCELERATION * world->clock.delta);
	capsule_add_xy_velocity(velocity_xy, &player->cp, world);
}

void	player_jump(t_player *player, t_world *world)
{
	if (player->input.jump && player->cp.is_grounded
		&& world->clock.time > player->last_jump_time + JUMP_DELAY)
	{
		player->cp.velocity.z = 0.065f;
		player->transform.position.z += 0.1f;
		start_anim(&player->jump, anim_forwards);
		player->last_jump_time = world->clock.time;
	}
	if (player->jump.active)
		update_anim(&player->jump, world->clock.delta);
}

void	player_update_animations(t_player *player, t_world *world)
{
	if (player->jump.active)
		update_anim(&player->jump, world->clock.delta);
}
