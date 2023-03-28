/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 17:26:50 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 09:18:22 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "npc.h"

void	player_gun_raycast(t_player *player, t_world *world)
{
	t_entity	*hit_ent;
	t_decal		d;

	hit_ent = player->raycast_info.hit_entity;
	if (hit_ent != NULL)
	{
		if (hit_ent->component.type == COMP_NPC)
		{
			protagonist_play_audio(player, world, "hitmarker.wav");
			npc_get_hit(hit_ent, world);
		}
		else
		{
			d.img = get_image_by_name(*world->sdl, "bullet_hole");
			d.normal = player->raycast_info.face_normal;
			d.position = player->raycast_info.hit_pos;
			d.size = 5.0f;
			decal(world, d);
		}
	}
}

void	protagonist_play_audio(t_player *player,
	t_world *world, char *soundname)
{
	static t_audiosource	source;

	source.volume = 1.0f;
	source.sample = get_sample(world->sdl, soundname);
	_audiosource_2d_start(world->sdl, &source);
}

void	player_update_gun(t_player *player, t_world *world)
{
	t_gun			*gun;
	t_vector3		neutralpos;

	if (player->gun == NULL || player->gun->disabled
		|| !player->gun->player_owned)
		return ;
	gun = player->gun;
	gun->entity->transform.position = gun->stats.holster_pos;
	gun_update_shoot_status(player, gun, world);
	gun_update_reload_status(player, gun, world);
	if (gun->shoot_anim.active)
		update_anim(&gun->shoot_anim, world->clock.delta);
	else
		gun->shoot_anim.lerp = 0.0f;
	update_anim(&gun->view_anim, world->clock.delta);
	if (!gun->reload_anim.active)
		gun_update_transform(player, gun, world->clock);
	else
		gun_update_reload_anim(player, gun, world);
}
