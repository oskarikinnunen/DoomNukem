/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 14:31:10 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 22:31:58 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "entity.h"
#include "doomnukem.h"
#include "movement_defs.h"
#include "player.h"

static void	gun_entity_set_entity_values(t_gun *gun)
{
	gun->entity->ignore_raycasts = true;
	gun->entity->dont_save = true;
	gun->entity->dynamic_lit = true;
	gun->entity->hidden = true;
	gun->shoot_anim.frame_rate = 120;
	gun->shoot_anim.loop = false;
	gun->shoot_anim.last_frame = 2;
	gun->view_anim.frame_rate = 120;
	gun->view_anim.loop = false;
	gun->view_anim.last_frame = 4;
}

static void	player_init_weapons(t_player *player, t_world *world)
{
	static t_gun_stats	(*preset_funcs[20])(void)
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
		gun_entity_set_entity_values(&player->guns[i]);
		i++;
	}
}

void	player_init(t_player *player, t_sdlcontext *sdl, t_world *world)
{
	ft_bzero(player, sizeof(t_player));
	player->ammo_arr[AM_ASSAULT] = 120;
	player->ammo_arr[AM_SNIPER] = 40;
	player->ammo_arr[AM_GRENADE] = 20;
	player->ammo_arr[AM_PISTOL] = 60;
	player->transform.scale = vector3_one();
	player->transform.rotation = (t_vector3){-RAD90, -RAD90 * 0.99f, 0.0f};
	player->jump.frame_rate = 30;
	player->jump.last_frame = 18;
	player->height = PLAYER_HEIGHT;
	player->fov = 90.0f;
	player->health = 120;
	player_init_weapons(player, world);
	player->gun = &player->guns[0];
	player->guns[0].player_owned = false;
	player->guns[1].player_owned = false;
}
