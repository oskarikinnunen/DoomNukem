/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc_state.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:49:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 22:35:41 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"
#include "npc.h"

void	npc_start_push_anim(t_entity *entity, t_npc *npc, t_world *world)
{
	npc->push_anim.frame_rate = 30;
	npc->push_anim.last_frame = 15;
	start_anim(&npc->push_anim, anim_forwards);
	start_human_anim(entity, "Walk_weapon", world);
	npc->next_action_time = world->clock.time
		+ game_random_range(world, 700, 4000);
}

static t_path	random_path(t_entity *entity, t_npc *npc, t_world *world)
{
	int			i;
	int			r;
	int			last_id;
	t_nav_node	*curnode;
	t_path		result;

	curnode = &world->nav.navmesh[
		get_nearest_target_node(world, entity->transform.position)];
	if (curnode->neighbors == 0)
		return ((t_path){.valid_path = false});
	i = 0;
	while (i < 20)
	{
		r = game_random_range(world, 0, curnode->neighbors);
		curnode = &world->nav.navmesh[curnode->neighbors_id[r]];
		last_id = curnode->index;
		i++;
	}
	result = pathfind(entity->transform.position, curnode->mid_point, world);
	if (result.bi <= 1)
		return ((t_path){.valid_path = false});
	return (result);
}

void	npc_update_cautious_move_state(t_entity *entity,
								t_npc *npc, t_world *world)
{
	npc->next_action_time = world->clock.time + 2000;
	npc->path = random_path(entity, npc, world);
	npc->orig_x_rotation = 0.0f;
}

void	npc_switch_to_death_state(t_entity *entity, t_npc *npc, t_world *world)
{
	npc->state = NPC_STATE_DEAD;
	entity->animation.loop = false;
	entity->animation.persist = true;
}

void	npc_update_state(t_entity *entity, t_npc *npc, t_world *world)
{
	npc_player_raycast(entity, npc, world);
	npc->prev_state = npc->state;
	if (!npc->sees_player && !vector3_cmp(npc->last_seen_player_pos,
			vector3_zero()) && npc->state != NPC_STATE_CHASE)
		npc_switch_to_chase_state(entity, npc, world);
	if (npc->state == NPC_STATE_CHASE && !npc->path.valid_path)
		npc_switch_to_cautious_move_state(entity, npc, world);
	if (npc_sees_player_and_ready_to_aggro(npc))
		npc_switch_to_aggro_state(entity, npc, world);
	if (npc->state == NPC_STATE_AGGRO
		&& world->clock.time > npc->next_action_time)
		npc_start_pushanim_and_switch_to_push_state(entity, npc, world);
	if (npc->state == NPC_STATE_CAUTIOUS_MOVE
		&& world->clock.time > npc->next_action_time && !npc->path.valid_path)
		npc_update_cautious_move_state(entity, npc, world);
	if (npc->health <= 0 && npc->state != NPC_STATE_DEAD)
		npc_switch_to_death_state(entity, npc, world);
}
