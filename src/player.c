/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 14:31:10 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/25 19:35:51 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "entity.h"
#include "doomnukem.h"

t_gun	*debug_gun(t_player *player, t_sdlcontext *sdl)
{
	t_gun	*gun;

	gun = ft_memalloc(sizeof(t_gun));
	gun->entity.obj = get_object_by_name(*sdl, "Revolver.obj");
	gun->entity.transform.scale = vector3_one();
	gun->entity.transform.parent = &player->transform;
	gun->entity.transform.rotation = (t_vector3){0.0f, 0.0f, 0.0f};
	gun->holsterpos = (t_vector3){15.0f, 18.0f, -18.0f};
	gun->anglerecovery = 0.0006f;
	gun->recoiljump.y = 1.0f;
	gun->viewrecoil.y = 0.001f;
	gun->recoilangle.y = 10.0f;
	gun->aimpos = (t_vector3){0.0f, 15.0f, -7.0f};
	gun->firedelay = 100;
	gun->fullauto = true;
	gun->shoot_anim.framerate = 120;
	gun->shoot_anim.loop = false;
	gun->shoot_anim.lastframe = 2;
	gun->view_anim.framerate = 120;
	gun->view_anim.loop = false;
	gun->view_anim.lastframe = 4;
	gun->entity.transform.location = gun->holsterpos;
	return (gun);
}

void	player_init(t_player *player, t_sdlcontext *sdl)
{
	player->transform.location = (t_vector3) {500.0f, 500.0f, 500.0f};
	player->transform.rotation = (t_vector3){-RAD90, -RAD90 * 0.99f, 0.0f}; //TODO: implement transform for player
	player->transform.scale = vector3_one();
	player->transform.rotation = (t_vector3){-RAD90, -RAD90 * 0.99f, 0.0f};
	player->jump.framerate = 30;
	player->jump.lastframe = 15;
	player->gun = debug_gun(player, sdl);
}
