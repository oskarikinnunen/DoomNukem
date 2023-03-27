/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc_anims.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:29:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 15:17:01 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"
#include "movement_defs.h"
#include <stdlib.h>
#include <time.h>
#include "npc.h"

static inline void	start_random_death_anim(t_entity *entity, t_world *world)
{
	uint32_t	random;

	random = game_random_range(world, 1, 4);
	if (random == 1)
		start_human_anim(entity, "Death1", world);
	if (random == 2)
		start_human_anim(entity, "Death2", world);
	if (random == 3)
		start_human_anim(entity, "Death3", world);
	if (random == 4)
		start_human_anim(entity, "Death4", world);
	entity->animation.loop = false;
	entity->animation.persist = true;
}

void	npc_update_anim_state(t_entity *entity, t_npc *npc, t_world *world)
{
	bool	changed;

	changed = npc->prev_state != npc->state;
	if (!changed)
		return ;
	if (npc->state == NPC_STATE_AGGRO)
	{
		start_human_anim(entity, "Shoot_rifle", world);
		update_anim(&entity->animation, world->clock.delta);
	}
	if (npc->state == NPC_STATE_CHASE
		|| npc->state == NPC_STATE_CAUTIOUS_MOVE
		|| npc->state == NPC_STATE_AGGRO_PUSH)
		start_human_anim(entity, "Walk_weapon", world);
	if (npc->state == NPC_STATE_DEAD)
	{
		npc->prev_state = npc->state;
		start_random_death_anim(entity, world);
	}
}

static void	lookaround(t_entity *entity, t_npc *npc, t_world *world)
{
	if (npc->state == NPC_STATE_CAUTIOUS_MOVE && npc->orig_x_rotation != 0)
		entity->transform.rotation.x = ft_fmovetowards(
				entity->transform.rotation.x, npc->orig_x_rotation
				+ (sinf(world->clock.time * 0.0012f) * RAD90) - (RAD90 / 2.0f),
				world->clock.delta * 0.015f);
}

void	npc_update_push_anim(t_entity *entity,
									t_npc *npc, t_world *world)
{
	t_vector2	move;

	if (npc->state != NPC_STATE_AGGRO_PUSH || !npc->push_anim.active)
		return ;
	update_anim(&npc->push_anim, world->clock.delta);
	if (!npc->push_anim.active)
		start_human_anim(entity, "Shoot_rifle", world);
	move = v3tov2(vector3_sub(world->player->transform.position,
				entity->transform.position));
	capsule_add_xy_velocity(move, &npc->phys, world);
}
