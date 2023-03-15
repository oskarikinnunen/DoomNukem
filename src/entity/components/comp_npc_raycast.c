/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc_raycast.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 17:08:18 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/13 17:14:19 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

void	npc_player_raycast(t_entity *entity, t_npc *npc, t_world *world)
{
	t_entity		*player_ent;
	t_ray			r;
	t_raycastinfo	info;

	npc->prev_state = npc->state;
	player_ent = spawn_entity(world);
	player_ent->obj = get_object_by_name(*world->sdl, "Human.obj");
	player_ent->transform.position = world->player->transform.position;
	entity->ignore_raycasts = true;
	r.origin = vector3_add(entity->transform.position, (t_vector3){.z = 5.0f});
	r.dir = vector3_sub(world->player->headposition, r.origin);
	npc->seesplayer = false;
	if (vector3_sqr_dist(world->player->transform.position,
			entity->transform.position) < 100000.0f
		&& raycast_new(r, &info, world)
		&& !world->player->noclip)
	{
		if (info.hit_entity == player_ent)
		{
			npc->seesplayer = true;
			npc->lastseen_playerpos = world->player->transform.position;
		}
	}
	entity->ignore_raycasts = false;
	destroy_entity(world, player_ent);
}
