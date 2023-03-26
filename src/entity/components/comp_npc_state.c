/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc_state.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:49:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 16:28:32 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

void	npc_start_pushanim(t_entity *entity, t_npc *npc, t_world *world)
{
	npc->push_anim.framerate = 30;
	npc->push_anim.lastframe = 15;
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
	t_navnode	*curnode;
	t_path		result;

	curnode = &world->nav.navmesh[
		get_nearest_target_node(world, entity->transform.position)];
	i = 0;
	while (i < 20)
	{
		r = rand() % curnode->neighbors;
		while (curnode->neighbors_id[r] == last_id && curnode->neighbors > 2)
			r = rand() % curnode->neighbors;
		printf("node %i is %i\n", i, curnode->neighbors_id[r]);
		curnode = &world->nav.navmesh[curnode->neighbors_id[r]];
		last_id = curnode->index;
		i++;
	}
	result = pathfind(entity->transform.position, curnode->mid_point, world);
	if (result.bi <= 1)
		result = random_path(entity, npc, world);
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
	printf("going from %i to death state \n", npc->state);
	npc->state = NPC_STATE_DEAD;
	entity->animation.loop = false;
	entity->animation.persist = true;
}

void	npc_update_state(t_entity *entity, t_npc *npc, t_world *world)
{
	npc_player_raycast(entity, npc, world);
	npc->prev_state = npc->state;
	if (!npc->seesplayer && !vector3_cmp(npc->lastseen_playerpos,
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
