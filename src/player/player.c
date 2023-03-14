/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 14:31:10 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 13:13:24 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "entity.h"
#include "doomnukem.h"
#include "movement_defs.h"

t_gun	*debug_gun(t_player *player, t_sdlcontext *sdl, t_world *world)
{
	t_gun	*gun;

	gun = ft_memalloc(sizeof(t_gun));
	//gun->entity->obj = 
	//entity_assign_object(world, &gun->entity, get_object_by_name(*sdl, "grenadelauncher.obj"));
	gun->entity = spawn_entity(world, get_object_by_name(*sdl, "grenadelauncher.obj"));
	gun->entity->ignore_raycasts = true;
	gun->entity->transform.scale = vector3_one();
	gun->entity->transform.parent = &player->transform;
	gun->entity->transform.rotation = (t_vector3){0.0f, 0.0f, 0.0f};
	gun->shoot_anim.framerate = 120;
	gun->shoot_anim.loop = false;
	gun->shoot_anim.lastframe = 2;
	gun->view_anim.framerate = 120;
	gun->view_anim.loop = false;
	gun->view_anim.lastframe = 4;
	gun->entity->transform.position = gun->stats.holsterpos;
	change_gun_preset(gun, world->sdl, 0);
	return (gun);
}

void	player_init(t_player *player, t_sdlcontext *sdl, t_world *world)
{
	ft_bzero(player, sizeof(player));
	//player->transform.position = (t_vector3) {500.0f, 500.0f, 500.0f};
	//player->transform.rotation = (t_vector3){-RAD90, -RAD90 * 0.99f, 0.0f}; //TODO: implement transform for player
	player->ammo_union.ammo_u8.assault = 120;
	player->ammo_union.ammo_u8.sniper = 20;
	player->ammo_union.ammo_u8.pistol = 100;
	player->ammo_union.ammo_u8.grenade = 5;
	player->transform.scale = vector3_one();
	player->transform.rotation = (t_vector3){-RAD90, -RAD90 * 0.99f, 0.0f};
	player->jump.framerate = 30;
	player->jump.lastframe = 18;
	player->height = PLAYER_HEIGHT;
	player->fov = 90.0f;
	player->health = 120;
	//player->jump.lerp = 
	player->gun = debug_gun(player, sdl, world);
}
