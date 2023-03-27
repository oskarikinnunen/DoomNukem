/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:57:39 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:24:24 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"
#include "movement_defs.h"
#include "npc.h"
#include <stdlib.h>
#include <time.h>

static void	npc_update_physics(t_npc *npc, t_entity *entity)
{
	npc->phys.position = &entity->transform.position;
	npc->phys.radius = 15.0f;
	npc->phys.gravity_override = NULL;
	npc->phys.height = PLAYER_HEIGHT;
	npc->phys.max_velocity = PLAYER_WALKSPEED;
}

bool	npc_player_within_viewcone(t_entity *npc_ent, t_player *player)
{
	float	angle_between;

	angle_between = vector2_anglebetween(vector2_zero(),
			v3tov2(vector3_sub(npc_ent->transform.position,
					player->transform.position)))
		+ RAD90;
	if (ft_absf(npc_ent->transform.rotation.x - angle_between) < RAD90 / 2.0f)
		return (true);
	return (false);
}

//"animation" for when the npc starts moving towards player,
//	while the npc is in state NPC_STATE_AIM
void	npc_update_state(t_entity *entity, t_npc *npc, t_world *world);

static void	npc_pathfind_step(t_entity *entity, t_npc *npc, t_world *world)
{
	t_vector2	enterpoint_2d;
	t_vector2	move;

	if (!npc->path.valid_path || npc->state == NPC_STATE_AGGRO)
		return ;
	enterpoint_2d = v3tov2(npc->path.path[npc->path.ai].enter_point);
	move = vector2_sub(enterpoint_2d, v3tov2(entity->transform.position));
	move = vector2_mul(vector2_normalise(move), world->clock.delta
			* PLAYER_ACCELERATION);
	capsule_add_xy_velocity(move, &npc->phys, world);
	entity_rotate_towards_xy(entity, enterpoint_2d, \
							0.015f * world->clock.delta);
	if (vector3_sqr_dist(entity->transform.position,
			npc->path.path[npc->path.ai].enter_point) < 160.1f
		&& npc->path.ai < npc->path.bi)
		npc->path.ai++;
	if (npc->path.ai >= npc->path.bi && npc->path.valid_path)
		npc->path.valid_path = false;
}

static void	lookaround(t_entity *entity, t_npc *npc, t_world *world)
{
	if (npc->state == NPC_STATE_CAUTIOUS_MOVE && npc->orig_x_rotation != 0)
		entity->transform.rotation.x = ft_fmovetowards(
				entity->transform.rotation.x, npc->orig_x_rotation
				+ (sinf(world->clock.time * 0.0012f) * RAD90) - (RAD90 / 2.0f),
				world->clock.delta * 0.015f);
}

//TODO: this file is norminette compliant down to here (lines 0-207 are ok)
void	comp_npc_update(t_entity *entity, t_world *world)
{
	t_npc	*npc;

	npc = entity->component.data;
	if (npc == NULL || world->app_mode == APPMODE_EDIT
		|| world->player->health == 0)
		return ;
	update_anim(&entity->animation, world->clock.delta);
	if (npc->state != NPC_STATE_DEAD)
	{
		npc_update_state(entity, npc, world);
		if (npc->state == NPC_STATE_AGGRO || npc->state == NPC_STATE_AGGRO_PUSH)
		{
			if (world->clock.time > npc->next_shoot_time)
				npc_shoot(entity, npc, world);
			entity_rotate_towards_xy(entity,
				v3tov2(npc->last_seen_player_pos), 0.015f * world->clock.delta);
		}
		if (npc->state == NPC_STATE_AGGRO_PUSH)
			npc_update_push_anim(entity, npc, world);
		lookaround(entity, npc, world);
		npc_pathfind_step(entity, npc, world);
	}
	npc_update_physics(npc, entity);
	capsule_phys_step(&npc->phys, world);
	npc_update_anim_state(entity, npc, world);
}
