/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc_states_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 17:07:49 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 15:16:49 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"
#include "npc.h"

void	npc_switch_to_chase_state(t_entity *entity,
							t_npc *npc, t_world *world)
{
	npc->state = NPC_STATE_CHASE;
	npc->path = pathfind(entity->transform.position,
			npc->last_seen_player_pos, world);
}

void	npc_switch_to_cautious_move_state(t_entity *entity,
									t_npc *npc, t_world *world)
{
	npc->state = NPC_STATE_CAUTIOUS_MOVE;
	npc->next_action_time = world->clock.time + 3000;
	npc->last_seen_player_pos = vector3_zero();
}

bool	npc_sees_player_and_ready_to_aggro(t_npc *npc)
{
	return ((npc->sees_player
			&& npc->state != NPC_STATE_AGGRO
			&& npc->state != NPC_STATE_AGGRO_PUSH)
		|| (npc->sees_player && npc->state == NPC_STATE_AGGRO_PUSH
			&& !npc->push_anim.active));
}

void	npc_switch_to_aggro_state(t_entity *entity, t_npc *npc, t_world *world)
{
	npc->state = NPC_STATE_AGGRO;
	npc->next_action_time = world->clock.time
		+ game_random_range(world, 700, 4000);
}

void	npc_start_pushanim_and_switch_to_push_state(t_entity *entity,
											t_npc *npc, t_world *world)
{
	npc_start_push_anim(entity, npc, world);
	npc->state = NPC_STATE_AGGRO_PUSH;
}
