/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc_raycast.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 17:08:18 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 19:27:13 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"
#include "raycast.h"

void	npc_player_raycast(t_entity *entity, t_npc *npc, t_world *world)
{
	t_raycastinfo	info;
	t_ray			r;
 
	npc->prev_state = npc->state;
	entity->ignore_raycasts = true;
	r.origin = vector3_add(entity->transform.position, (t_vector3){0.0f, 0.0f, 5.0f});
	r.dir = vector3_sub(world->player->headposition, r.origin);
	r.dir = vector3_mul(r.dir, 100.0f);
	npc->seesplayer = false;
	r.dir = vector3_normalise(r.dir);
	float dist = vector3_dist(world->player->transform.position, get_entity_world_position(entity));
	if (dist < 100000.0f && raycast(r, &info, world) && !world->player->noclip)
	{
		if (info.distance > dist)
		{
			npc->seesplayer = true;
			npc->lastseen_playerpos = world->player->transform.position;
		}
	}
	entity->ignore_raycasts = false;
}