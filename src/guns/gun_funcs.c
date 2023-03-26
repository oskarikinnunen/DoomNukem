/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 17:07:17 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 17:28:25 by okinnune         ###   ########.fr       */
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

void	gun_update_reload_status(t_player *player,
		t_gun *gun, t_world *world)
{
	if (gun->bullets != gun->stats.magazinecapacity
		&& player->input.reload && !gun->reload_anim.active)
	{
		if (player->ammo_arr[gun->stats.ammomask] > 0)
		{
			gun->reload_anim.framerate = 30;
			gun->reload_anim.lastframe = gun->stats.reloadtime;
			player->ammo_arr[gun->stats.ammomask] += gun->bullets;
			start_anim(&gun->reload_anim, anim_forwards);
			play_gun_reload_audio(world);
		}
	}
}

void	gun_update_shoot_status(t_player *player,
		t_gun *gun, t_world *world)
{
	if ((!player->input.shoot || gun->stats.fullauto)
		&& world->clock.time > gun->lastshottime + gun->stats.firedelay)
		gun->readytoshoot = true;
	if (gun->bullets == 0 || gun->reload_anim.active)
		gun->readytoshoot = false;
	if (player->input.shoot && gun->readytoshoot)
	{
		start_anim(&gun->shoot_anim, anim_forwards);
		start_anim(&gun->view_anim, anim_forwards);
		gun->readytoshoot = false;
		gun->lastshottime = world->clock.time;
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
		bullets_added = ft_min(player->ammo_arr[gun->stats.ammomask],
				gun->stats.magazinecapacity);
		gun->bullets = bullets_added;
		player->ammo_arr[gun->stats.ammomask] -= bullets_added;
	}
	midlerp = 1.0f - (ft_absf(0.5f - gun->reload_anim.lerp) * 2.0f);
	gun->entity->transform.position.z -= midlerp * 2.2f;
	gun->entity->transform.rotation.x = ft_degtorad(25.0f * midlerp);
	gun->entity->transform.rotation.z = ft_degtorad(30.0f * midlerp);
}
