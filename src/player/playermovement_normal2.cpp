/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playermovement_normal2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:23:38 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:25:58 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"

void	player_ceiling_check(t_player *player)
{
	if (player->cp.ceiling_trigger)
	{
		player->jump.active = false;
		player->cp.ceiling_trigger = false;
	}
}

static float	current_maxvelocity(t_player *player)
{
	if (!player->cp.is_grounded)
		return (PLAYER_WALKSPEED * 1.35f
			+ (player->input.run * (PLAYER_RUNSPEED - PLAYER_WALKSPEED)));
	return (PLAYER_WALKSPEED
		+ (player->input.run * (PLAYER_RUNSPEED - PLAYER_WALKSPEED)));
}

t_bound	get_bound(t_character_physics *cp, t_world *world);

void	crouch_update(t_player *player, t_world *world)
{
	t_bound	bound;

	if (player->input.crouch)
	{
		if (!player->crouch_jumped
			&& !player->cp.is_grounded && player->cp.velocity.z >= 0)
		{
			player->cp.velocity.z = 0.22f;
			player->crouch_jumped = true;
		}
		if (player->cp.is_grounded)
			player->height = ft_fmovetowards(player->height, \
				PLAYER_CROUCHHEIGHT, PLAYER_CROUCHSPEED * world->clock.delta);
	}
	else
	{
		player->height = ft_fmovetowards(player->height,
				PLAYER_HEIGHT, PLAYER_CROUCHSPEED * world->clock.delta);
		if (player->cp.is_grounded)
			player->crouch_jumped = false;
	}
	bound = get_bound(&player->cp, world);
	player->height = ft_clampf(player->height,
			PLAYER_CROUCHHEIGHT, bound.max - player->cp.position->z);
}

void	player_update_physics(t_player *player, t_world *world)
{
	float				lerp;
	static float		override;

	player->cp.position = &player->transform.position;
	player->cp.radius = 15.0f;
	player->cp.gravity_override = NULL;
	player->cp.height = player->height;
	player->cp.max_velocity = current_maxvelocity(player);
	if (player->jump.active)
	{
		lerp = 1.5f - player->jump.lerp;
		override = (GRAVITY + (lerp * 0.65f));
		player->cp.gravity_override = &override;
	}
}
