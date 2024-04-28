/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:39:11 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:44:24 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "npc.h"

void	npc_play_sound(t_entity *entity, t_world *world, char *soundname)
{
	t_npc			*npc;
	t_audiosource	source;

	if (entity->component.type != COMP_NPC)
		return ;
	npc = entity->component.data;
	source = npc->audiosource;
	source.sample = get_sample(world->sdl, soundname);
	source._real_range = 250.0f;
	source.volume = 1.0f;
	source.play_always = false;
	_audiosource_start(world->sdl, &source, &entity->transform.position);
}

void	npc_shoot(t_entity *entity, t_npc *npc, t_world *world)
{
	uint32_t		hitdice;
	uint32_t		hitchance;

	if (world->player->health == 0)
		return ;
	npc_play_sound(entity, world, "gun_machinegun.wav");
	if (npc->sees_player)
	{
		hitdice = game_random_range(world, 0, 10);
		hitchance = 5;
		if (vector3_magnitude(world->player->cp.velocity) > 0.5f)
			hitchance = 2;
		if (hitdice < hitchance)
		{
			protagonist_play_audio(world->player, world, "protag_hurt.wav");
			world->player->health -= 10;
			world->player->last_hurt_time = world->clock.time;
			world->player->last_hurt_pos = entity->transform.position;
		}
		else
			protagonist_play_audio(world->player, world, "ricochet.wav");
	}
	npc->next_shoot_time = world->clock.time
		+ game_random_range(world, 750, 2200);
}

//TODO: ugly norm
void	entity_rotate_towards_xy(t_entity *entity,
									t_vector2 pos, float delta)
{
	float	angle;

	angle = vector2_anglebetween(vector2_zero(),
			vector2_sub(v3tov2(entity->transform.position), pos));
	entity->transform.rotation.x
		= ft_fmovetowards(entity->transform.rotation.x, angle + RAD90, delta);
}
