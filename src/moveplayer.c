/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 09:58:37 by raho             ###   ########.fr       */
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
#include "npc.h"

static t_vector3	normalized_inputvector(t_input input, t_player player)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;
	t_vector3	right;
	t_vector3	strafe;

	movement = vector3_zero();
	forward = vector3_mul_vector3(player.look_dir,
			(t_vector3){1.0f, 1.0f, 0.0f});
	forward = vector3_normalise(forward);
	movement = vector3_mul(forward, -input.move.y);
	right = vector3_cross_product(forward, vector3_up());
	strafe = vector3_mul(right, input.move.x);
	movement = vector3_add(movement, strafe);
	if (player.no_clip)
	{
		movement.z += 1.5f * input.jump;
		movement.z -= 1.5f * input.crouch;
	}
	movement = vector3_mul(movement, 1.0f);
	movement = vector3_clamp_magnitude(movement, 1.0f);
	return (movement);
}

static void	noclip_movement(t_player *player,
		t_vector3 move_vector, t_world *world)
{
	player->transform.position = vector3_add(player->transform.position,
			move_vector);
}

//TODO: fix this logic so it works with more than 2 guns
static void	player_changegun(t_player *player)
{
	int	i;
	int	old_sel;

	old_sel = player->gun_selection;
	player->gun_selection += player->input.nextgun;
	player->gun_selection -= player->input.prevgun;
	player->gun_selection %= GUNPRESETCOUNT;
	if (player->guns[player->gun_selection].player_owned == false)
		player->gun_selection = old_sel;
	player->gun = &player->guns[player->gun_selection];
	i = 0;
	while (i < GUNPRESETCOUNT)
	{
		player->guns[i].entity->hidden = true;
		i++;
	}
	if (player->gun->player_owned)
		player->gun->entity->hidden = false;
}

static void	player_raycast(t_player *player, t_world *world)
{
	t_ray	ray;

	ray.origin = player->head_position;
	ray.dir = player->look_dir;
	raycast(ray, &player->raycast_info, world);
}

void	moveplayer(t_player *player, t_input *input, t_world *world)
{
	t_vector3	move_vector;
	float		angle;

	player->gun->entity->transform.parent = &player->head_transform;
	player_raycast(player, world);
	player->input = *input;
	if ((player->input.nextgun || player->input.prevgun)
		&& !player->gun->reload_anim.active && world->app_mode == APPMODE_PLAY)
		player_changegun(player);
	if (world->app_mode == APPMODE_PLAY)
		player_update_gun(player, world);
	if (!player->locked)
	{
		if (!player->no_clip)
			player_movement_normal(player, world);
		else
			player_movement_noclip(player, world);
	}
	player->head_position = vector3_add(player->transform.position,
			(t_vector3){.z = player->height * 0.75f});
	player->head_transform.position = player->head_position;
	player->head_transform.rotation = player->transform.rotation;
	player->head_transform.scale = player->transform.scale;
}
