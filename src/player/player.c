/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 14:31:10 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/17 18:10:00 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "entity.h"
#include "doomnukem.h"
#include "movement_defs.h"
#include "player.h"

static void	player_init_weapons(t_player *player, t_world *world)
{
	static t_gunstats	(*preset_funcs[20])(void)
		= {
		gun_machinegun,
		gun_sniper,
		NULL
	};
	int					i;

	i = 0;
	while (i < GUNPRESETCOUNT)
	{
		ft_bzero(&player->guns[i], sizeof(player->guns[i]));
		player->guns[i].stats = (preset_funcs[i])();
		player->guns[i].entity = spawn_entity(world,
				get_object_by_name(*world->sdl,
					player->guns[i].stats.object_name));
		player->guns[i].entity->ignore_raycasts = true;
		player->guns[i].shoot_anim.framerate = 120;
		player->guns[i].shoot_anim.loop = false;
		player->guns[i].shoot_anim.lastframe = 2;
		player->guns[i].view_anim.framerate = 120;
		player->guns[i].view_anim.loop = false;
		player->guns[i].view_anim.lastframe = 4;
		i++;
	}
}

void	player_init(t_player *player, t_sdlcontext *sdl, t_world *world)
{
	ft_bzero(player, sizeof(player));
	player->ammo_arr[AM_ASSAULT] = 120;
	player->ammo_arr[AM_SNIPER] = 40;
	player->ammo_arr[AM_GRENADE] = 20;
	player->ammo_arr[AM_PISTOL] = 60;
	player->transform.scale = vector3_one();
	player->transform.rotation = (t_vector3){-RAD90, -RAD90 * 0.99f, 0.0f};
	player->jump.framerate = 30;
	player->jump.lastframe = 18;
	player->height = PLAYER_HEIGHT;
	player->fov = 90.0f;
	player->health = 120;
	player_init_weapons(player, world);
	player->gun = &player->guns[0];
	player->guns[0].player_owned = true;
	player->guns[1].player_owned = false;
}
