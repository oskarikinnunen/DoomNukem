/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 15:12:04 by okinnune         ###   ########.fr       */
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

static t_vector3	normalized_inputvector(t_input input, t_player player)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;
	t_vector3	right;
	t_vector3	strafe;

	movement = vector3_zero();
	forward = vector3_mul_vector3(player.lookdir,
			(t_vector3){1.0f, 1.0f, 0.0f});
	forward = vector3_normalise(forward);
	movement = vector3_mul(forward, -input.move.y);
	right = vector3_crossproduct(forward, vector3_up());
	strafe = vector3_mul(right, input.move.x);
	movement = vector3_add(movement, strafe);
	if (player.noclip)
	{
		movement.z += 1.5f * input.jump;
		movement.z -= 1.5f * input.crouch;
	}
	movement = vector3_mul(movement, 1.0f);
	movement = vector3_clamp_magnitude(movement, 1.0f);
	return (movement);
}

void	player_gun_raycast(t_player *player, t_world *world)
{
	/*t_ray			ray;
	t_raycastinfo	info;
	t_npc			*hit_npc;
	uint32_t		r;


	ray.origin = player->headposition;
	ray.dir = player->lookdir;
	ft_bzero(&info, sizeof(info));*/
	t_entity	*hit_ent;

	hit_ent = player->raycastinfo.hit_entity;
	if (hit_ent != NULL
		&& hit_ent->component.type == COMP_NPC)
	{
		protagonist_play_audio(player, world, "hitmarker.wav");
		npc_get_hit(hit_ent, world);
	}
	/*if (raycast(ray, &info, world))
	{
		if (info.hit_entity != NULL
			&& info.hit_entity->component.type == COMP_NPC)
		{
			protagonist_play_audio(player, world, "hitmarker.wav");
			npc_get_hit(info.hit_entity, world);
		}
	}*/
}

void	protagonist_play_audio(t_player *player,
	t_world *world, char *soundname)
{
	static t_audiosource	source;

	source.volume = 1.0f;
	source.sample = get_sample(world->sdl, soundname);
	_audiosource_2d_start(world->sdl, &source);
}

static void	gun_update_reload_status(t_player *player,
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

static void	gun_update_shoot_status(t_player *player,
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

static void	gun_update_reload_anim(t_player *player, t_gun *gun,
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

void	player_update_gun(t_player *player, t_world *world)
{
	t_gun			*gun;
	t_vector3		neutralpos;

	if (player->gun == NULL || player->gun->disabled)
		return ;
	gun = player->gun;
	gun->entity->transform.position = gun->stats.holsterpos;
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

	player->gun_ammos[player->gun_selection] = player->gun->bullets;
	player->gun_selection += player->input.nextgun;
	player->gun_selection -= player->input.prevgun;
	player->gun_selection %= GUNPRESETCOUNT;
	if (player->guns[player->gun_selection].player_owned == false)
		player->gun_selection = 0;
	player->gun = &player->guns[player->gun_selection];
	i = 0;
	while (i < GUNPRESETCOUNT)
	{
		player->guns[i].entity->hidden = true;
		i++;
	}
	player->gun->entity->hidden = false;
}

static void	player_raycast(t_player *player, t_world *world)
{
	t_ray	ray;

	ray.origin = player->headposition;
	ray.dir = player->lookdir;
	raycast(ray, &player->raycastinfo, world);
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
		if (!player->noclip)
			playermovement_normal(player, world);
		else
			playermovement_noclip(player, world);
	}
	player->headposition = vector3_add(player->transform.position,
			(t_vector3){.z = player->height * 0.75f});
	player->head_transform.position = player->headposition;
	player->head_transform.rotation = player->transform.rotation;
	player->head_transform.scale = player->transform.scale;
}
