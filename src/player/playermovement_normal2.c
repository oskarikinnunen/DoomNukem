/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playermovement_normal2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:23:38 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 14:57:12 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"

void	player_ceiling_check(t_player *player)
{
	if (player->cp.ceilingtrigger)
	{
		player->jump.active = false;
		player->cp.ceilingtrigger = false;
	}
}

static float	current_maxvelocity(t_player *player)
{
	if (!player->cp.isgrounded)
		return (PLAYER_WALKSPEED * 1.35f
			+ (player->input.run * (PLAYER_RUNSPEED - PLAYER_WALKSPEED)));
	return (PLAYER_WALKSPEED
		+ (player->input.run * (PLAYER_RUNSPEED - PLAYER_WALKSPEED)));
}

t_bound	get_bound(t_character_physics *cp, t_world *world);

void	crouchupdate(t_player *player, t_world *world)
{
	t_bound	bound;

	if (player->input.crouch)
	{
		if (!player->crouchjumped
			&& !player->cp.isgrounded && player->cp.velocity.z >= 0)
		{
			player->cp.velocity.z = 0.22f;
			player->crouchjumped = true;
		}
		if (player->cp.isgrounded)
			player->height = ft_fmovetowards(player->height,
				PLAYER_CROUCHHEIGHT, PLAYER_CROUCHSPEED * world->clock.delta);
	}
	else
	{
		player->height = ft_fmovetowards(player->height,
				PLAYER_HEIGHT, PLAYER_CROUCHSPEED * world->clock.delta);
		if (player->cp.isgrounded)
			player->crouchjumped = false;
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
