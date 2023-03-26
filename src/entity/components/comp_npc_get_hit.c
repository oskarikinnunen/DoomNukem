/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc_get_hit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:33:59 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 14:54:15 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"

void	npc_get_hit(t_entity *entity, t_world *world)
{
	t_npc	*hit_npc;
	int		r;

	hit_npc = entity->component.data;
	hit_npc->health -= world->player->gun->stats.damage;
	if (hit_npc->health <= 0 && hit_npc->state != NPC_STATE_DEAD)
	{
		r = game_random_range(world, 0, 15);
		if (r % 2 == 0)
			npc_play_sound(entity, world, "npc_death.wav");
		else
			npc_play_sound(entity, world, "npc_death2.wav");
	}
	hit_npc->hit = true;
	hit_npc->phys.velocity = vector3_normalise(
			vector3_sub(entity->transform.position,
				world->player->headposition));
	hit_npc->phys.velocity = vector3_mul(hit_npc->phys.velocity, 0.3f);
	hit_npc->phys.velocity.z = 0.0f;
	if (hit_npc->state == NPC_STATE_DEAD)
		hit_npc->phys.velocity.z = 0.055f;
	else
		hit_npc->phys.velocity = vector3_mul(hit_npc->phys.velocity, 0.38f);
}
