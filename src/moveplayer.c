/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:04:31 by raho             ###   ########.fr       */
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

float	fmovetowards(float f1, float f2, float delta)
{
	float	result;

	result = f1;
	if (f1 < f2)
	{
		result += delta;
		result = ft_clampf(result, f1, f2);
	}
	else
	{
		result -= delta;
		result = ft_clampf(result, f2, f1);
	}
	return (result);
}

void	player_gun_raycast(t_player *player, t_world *world)
{
	t_ray			ray;
	t_raycast_info	info;
	t_npc			*hit_npc;
	uint32_t		r;

	ray.origin = player->head_position;
	ray.dir = player->look_dir;
	ft_bzero(&info, sizeof(info));
	if (raycast(ray, &info, world))
	{
		if (info.hit_entity != NULL
			&& info.hit_entity->component.type == COMP_NPC)
		{
			hit_npc = (t_npc *)info.hit_entity->component.data;
			hit_npc->health -= player->gun->stats.damage;
			protagonist_play_audio(player, world, "hitmarker.wav");
			if (hit_npc->health <= 0 && hit_npc->state != NPC_STATE_DEAD)
			{
				r = game_random_range(world, 0, 15);
				if (r == 0)
					protagonist_play_audio(player, world,
						"protag_niceshot.wav");
				else if (r == 1)
					protagonist_play_audio(player, world,
						"protag_thatonesdead.wav");
				else if (r == 2)
					protagonist_play_audio(player, world,
						"protag_onedown.wav");
				else if (r == 3)
					protagonist_play_audio(player, world,
						"protag_anotherone.wav");
				printf("play protag \n");
				if (r % 2 == 0)
					npc_play_sound(info.hit_entity, world, "npc_death.wav");
				else
					npc_play_sound(info.hit_entity, world, "npc_death2.wav");
			}
			hit_npc->hit = true;
			hit_npc->phys.velocity = vector3_normalise(
					vector3_sub(info.hit_entity->transform.position,
						player->head_position));
			hit_npc->phys.velocity = vector3_mul(hit_npc->phys.velocity, 0.3f);
			hit_npc->phys.velocity.z = 0.0f;
			if (hit_npc->state == NPC_STATE_DEAD)
			{
				hit_npc->phys.velocity = vector3_mul(hit_npc->phys.velocity,
						0.25f);
				hit_npc->phys.velocity.z = 0.055f;
			}
			else
			{
				hit_npc->phys.velocity = vector3_mul(hit_npc->phys.velocity, 0.38f);
			}
		}
	}
}

void	play_gun_audio(t_gun *gun, t_world *world)
{
	static t_audio_source	source;

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
	static t_audio_source	source;

	if (source.sample.sound == NULL)
	{
		source.sample = get_sample(world->sdl, "gun_reload.wav");
		source._real_range = 50.0f;
		source.volume = 1.0f;
		source.play_always = false;
	}
	_audiosource_2d_start(world->sdl, &source);
}

void	protagonist_play_audio(t_player *player,
	t_world *world, char *soundname)
{
	static t_audio_source	source;

	source.volume = 1.0f;
	source.sample = get_sample(world->sdl, soundname);
	_audiosource_2d_start(world->sdl, &source);
}

static void gun_update_transform(t_player *player, t_gun *gun, t_clock c)
{
	float		lerp;
	t_vector3	neutralpos;

	gun->entity->transform.rotation.x = ft_degtorad(0.0f);
	if (player->input.aim)
		gun->aim_lerp += gun->stats.ads_speed * c.delta;
	else
		gun->aim_lerp -= (gun->stats.ads_speed * 1.25f) * c.delta;
	gun->aim_lerp = ft_clampf(gun->aim_lerp, 0.0f, 1.0f);
	lerp = gun->aim_lerp;
	player->fov = 90.0f + (gun->aim_lerp * gun->stats.fov_offset);
	neutralpos = vector3_lerp(gun->stats.holster_pos, gun->stats.aim_pos, lerp);
	gun->entity->transform.position = neutralpos;
	if (gun->shoot_anim.active)
		gun->entity->transform.position = vector3_add(gun->entity->transform.position, vector3_mul(vector3_up(), gun->shoot_anim.lerp * gun->stats.recoil_jump.y));
	//bobbing:
	gun->entity->transform.position.z += vector2_magnitude(player->input.move) * cosf((c.time * 0.007f)) * 0.05f;
	gun->entity->transform.position.z = ft_fmovetowards(gun->entity->transform.position.z, gun->entity->transform.position.z + player->cp.velocity.z, c.delta * 0.1f);
	gun->entity->transform.rotation.z += vector2_magnitude(player->input.move) * cosf((c.time * 0.007f)) * ft_degtorad(0.15f);
	//recoilrecovery:
	gun->entity->transform.rotation.y = fmovetowards(gun->entity->transform.rotation.y, ft_degtorad(player->input.move.y * 1.15f), gun->stats.angle_recovery * c.delta);
	//recoil.y:
	if (gun->shoot_anim.active)
		gun->entity->transform.rotation.y += ft_flerp(0.0f, ft_degtorad(gun->stats.recoil_angle.y), gun->shoot_anim.lerp);
	gun->entity->transform.rotation.y = ft_clampf(gun->entity->transform.rotation.y, ft_degtorad(-0.5f), ft_degtorad(15.0f));
	//gun->entity->transform.rotation.x = ft_flerp(0.0f, ft_degtorad(2.0f), gun->shoot_anim.lerp);
	//Slight angle when player moves:
	float	zturn = player->input.move.x * 2.0f;
	if (!player->input.aim)
		zturn += player->input.turn.x * 5.0f;

	zturn = ft_clampf(zturn, ft_degtorad(-2.5f), ft_degtorad(2.5f));
	if (player->locked)
		return ;
	gun->entity->transform.rotation.z = fmovetowards(gun->entity->transform.rotation.z, zturn, 0.0004f * c.delta);
	if (gun->view_anim.active)
		player->transform.rotation.y += gun->view_anim.lerp * c.delta * gun->stats.view_recoil.y;
}
static void gun_update_reload_status(t_player *player, t_gun *gun, t_world *world)
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

static void gun_update_shoot_status(t_player *player, t_gun *gun, t_world *world)
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

static void gun_update_reload_anim(t_player *player, t_gun *gun, t_world *world)
{
	uint32_t	bullets_added;
	float		midlerp;

	update_anim(&gun->reload_anim, world->clock.delta);
	if (!gun->reload_anim.active)
	{
		bullets_added = ft_min(player->ammo_arr[gun->stats.ammo_mask], gun->stats.magazine_capacity);
		gun->bullets = bullets_added;
		player->ammo_arr[gun->stats.ammo_mask] -= bullets_added;
	}
	midlerp = 1.0f - (ft_absf(0.5f - gun->reload_anim.lerp) * 2.0f);
	gun->entity->transform.position.z -= midlerp * 2.2f;
	gun->entity->transform.rotation.x = ft_degtorad(25.0f * midlerp);
	gun->entity->transform.rotation.z = ft_degtorad(30.0f * midlerp);
}

void	updateguntransform(t_player *player, t_world *world)
{
	t_gun			*gun;
	t_vector3		neutralpos;

	if (player->gun == NULL || player->gun->disabled)
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

static void	noclip_movement(t_player *player, t_vector3 move_vector, t_world *world)
{
	player->transform.position = vector3_add(player->transform.position, move_vector);
}

static void	player_changegun(t_player *player)
{
	player->gun_ammos[player->gun_selection] = player->gun->bullets;
	player->gun_selection += player->input.nextgun;
	player->gun_selection -= player->input.prevgun;
	player->gun_selection %= GUNPRESETCOUNT;
	//Switching gun, hiding all the other player gun entities
	if (player->guns[player->gun_selection].player_owned == false) //TODO: fix this logic so it works with more than 2 guns
		player->gun_selection = 0;
	player->gun = &player->guns[player->gun_selection];
	int i = 0;
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
	/*if ((player->input.nextgun || player->input.prevgun)
		&& !player->gun->reload_anim.active && world->app_mode == APPMODE_PLAY)
		player_changegun(player);*/
	if (world->app_mode == APPMODE_PLAY)
		updateguntransform(player, world);
	if (!player->locked)
	{
		if (!player->no_clip)
			player_movement_normal(player, world);
		else
			player_movement_noclip(player, world);
	}
	player->head_position = vector3_add(player->transform.position, (t_vector3){.z = player->height * 0.75f});
	player->head_transform.position = player->head_position;
	player->head_transform.rotation = player->transform.rotation;
	player->head_transform.scale = player->transform.scale;
}
