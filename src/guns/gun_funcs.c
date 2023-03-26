/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 17:07:17 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:54:23 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"
#include "bresenham.h"
#include "objects.h"
#include "entity.h"
#include "libft.h"
#include "editor_tools.h"
#include "movement_defs.h"
#include "raycast.h"

void	play_gun_audio(t_gun *gun, t_world *world)
{
	static t_audiosource	source;

	if (source.sample.sound == NULL)
	{
		source.sample = get_sample(world->sdl, "gun_grenadelauncher.wav");
		source._real_range = 50.0f;
		source.volume = 1.0f;
		source.play_always = false;
	}
	source.sample = get_sample(world->sdl, gun->stats.audio_name);
	_audiosource_2d_start(world->sdl, &source);
}

void	play_gun_reload_audio(t_world *world)
{
	static t_audiosource	source;

	if (source.sample.sound == NULL)
	{
		source.sample = get_sample(world->sdl, "gun_reload.wav");
		source._real_range = 50.0f;
		source.volume = 1.0f;
		source.play_always = false;
	}
	_audiosource_2d_start(world->sdl, &source);
}

void	gun_update_reload_status(t_player *player,
		t_gun *gun, t_world *world)
{
	if (gun->bullets != gun->stats.magazine_capacity
		&& player->input.reload && !gun->reload_anim.active)
	{
		if (player->ammo_arr[gun->stats.ammo_mask] > 0)
		{
			gun->reload_anim.frame_rate = 30;
			gun->reload_anim.last_frame = gun->stats.reload_time;
			player->ammo_arr[gun->stats.ammo_mask] += gun->bullets;
			start_anim(&gun->reload_anim, anim_forwards);
			play_gun_reload_audio(world);
		}
	}
}

void	gun_update_shoot_status(t_player *player,
		t_gun *gun, t_world *world)
{
	if ((!player->input.shoot || gun->stats.full_auto)
		&& world->clock.time > gun->last_shot_time + gun->stats.fire_delay)
		gun->ready_to_shoot = true;
	if (gun->bullets == 0 || gun->reload_anim.active)
		gun->ready_to_shoot = false;
	if (player->input.shoot && gun->ready_to_shoot)
	{
		start_anim(&gun->shoot_anim, anim_forwards);
		start_anim(&gun->view_anim, anim_forwards);
		gun->ready_to_shoot = false;
		gun->last_shot_time = world->clock.time;
		gun->bullets--;
		play_gun_audio(gun, world);
		player_gun_raycast(player, world);
	}
}

void	gun_update_reload_anim(t_player *player, t_gun *gun,
		t_world *world)
{
	uint32_t	bullets_added;
	float		midlerp;

	update_anim(&gun->reload_anim, world->clock.delta);
	if (!gun->reload_anim.active)
	{
		bullets_added = ft_min(player->ammo_arr[gun->stats.ammo_mask],
				gun->stats.magazine_capacity);
		gun->bullets = bullets_added;
		player->ammo_arr[gun->stats.ammo_mask] -= bullets_added;
	}
	midlerp = 1.0f - (ft_absf(0.5f - gun->reload_anim.lerp) * 2.0f);
	gun->entity->transform.position.z -= midlerp * 2.2f;
	gun->entity->transform.rotation.x = ft_degtorad(25.0f * midlerp);
	gun->entity->transform.rotation.z = ft_degtorad(30.0f * midlerp);
}
