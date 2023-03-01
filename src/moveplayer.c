/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/01 18:44:31 by okinnune         ###   ########.fr       */
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

static t_vector3	normalized_inputvector(t_input input, t_player player)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;

	movement = vector3_zero();
	//forward.z = 0;
	forward = vector3_mul_vector3(player.lookdir, (t_vector3){1.0f, 1.0f, 0.0f});
	forward = vector3_normalise(forward);
	movement = vector3_mul(forward, -input.move.y);
	t_vector3 right = vector3_crossproduct(forward, vector3_up());
	t_vector3 strafe = vector3_mul(right, input.move.x);
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

void player_gun_raycast(t_player *player, t_world *world)
{
	t_ray			ray;
	t_raycastinfo	info;
	t_npc			*hit_npc;

	ray.origin = player->headposition;
	ray.dir = player->lookdir;
	ft_bzero(&info, sizeof(info));
	if (raycast_new(ray, &info, world))
	{
		if (info.hit_entity != NULL && info.hit_entity->component.type == COMP_NPC)
		{
			hit_npc = (t_npc *)info.hit_entity->component.data;
			printf("cur health = %i \n", hit_npc->health);
			printf("future health = %i \n", (hit_npc->health - player->gun->stats.damage));
			hit_npc->health -= player->gun->stats.damage;
			if (hit_npc->health <= 0 && hit_npc->state != NPC_STATE_DEAD)
			{
				uint32_t r = game_random_range(world, 0, 15);
				if (r == 0)
					protagonist_play_audio(player, world, "protag_niceshot.wav");
				if (r == 1)
					protagonist_play_audio(player, world, "protag_thatonesdead.wav");
				if (r == 2)
					protagonist_play_audio(player, world, "protag_onedown.wav");
				if (r == 3)
					protagonist_play_audio(player, world, "protag_anotherone.wav");
				printf("play protag \n");
			}
			hit_npc->hit = true;
			
			hit_npc->phys.new_velocity = vector3_normalise(vector3_sub(info.hit_entity->transform.position, player->headposition));
			hit_npc->phys.new_velocity = vector3_mul(hit_npc->phys.new_velocity, 0.3f);
			hit_npc->phys.new_velocity.z = 0.0f;
			if (hit_npc->state == NPC_STATE_DEAD)
			{
				hit_npc->phys.new_velocity = vector3_mul(hit_npc->phys.new_velocity, 0.25f);
				hit_npc->phys.new_velocity.z = 0.055f;
			}
				
			else
			{
				hit_npc->phys.new_velocity = vector3_mul(hit_npc->phys.new_velocity, 0.38f);
			}
			printf("hit_npc health %i \n", hit_npc->health);
		}
	}
}

void	play_gun_audio(t_gun *gun, t_world *world)
{
	static t_audiosource source;

	if (source.sample.sound == NULL)
	{
		source.sample = get_sample(world->sdl, "gun_grenadelauncher.wav");
		source._realrange = 50.0f;
		source.volume = 1.0f;
		source.play_always = false;
	}
	source.sample = get_sample(world->sdl, gun->stats.audio_name);
	_audiosource_2D_start(world->sdl, &source);
	//FMOD_Channel_
	//FMOD_Channel_Set3DAttributes(source.channel,
	//		(FMOD_VECTOR *) &world->player->headposition, &((FMOD_VECTOR){0}));
	//FMOD_Channel_SetVolume(source.channel, source.volume * world->sdl->audio.sfx_volume);
}

void	play_gun_reload_audio(t_world *world)
{
	static t_audiosource source;

	if (source.sample.sound == NULL)
	{
		source.sample = get_sample(world->sdl, "gun_reload.wav");
		source._realrange = 50.0f;
		source.volume = 1.0f;
		source.play_always = false;
	}
	_audiosource_2D_start(world->sdl, &source);
	//FMOD_Channel_
	//FMOD_Channel_Set3DAttributes(source.channel,
	//		(FMOD_VECTOR *) &world->player->headposition, &((FMOD_VECTOR){0}));
	//FMOD_Channel_SetVolume(source.channel, source.volume * world->sdl->audio.sfx_volume);
}

void	protagonist_play_audio(t_player *player, t_world *world, char *soundname)
{
	static t_audiosource source;

	source.volume = 1.0f;
	source.sample = get_sample(world->sdl, soundname);
	_audiosource_2D_start(world->sdl, &source);
}

void	updateguntransform(t_player *player, t_world *world)
{
	static float	lerp;
	t_gun			*gun;
	t_vector3		neutralpos;

	if (player->gun == NULL || player->gun->disabled)
		return ;
	gun = player->gun;
	gun->entity->transform.position = gun->stats.holsterpos;
	//gun->entity->transform.position.z -= player.tra;
	//neutralpos = gun->entity->transform.location;
	if (!player->input.shoot && world->clock.prev_time > gun->lastshottime + gun->stats.firedelay)
		gun->readytoshoot = true;
	if (gun->stats.fullauto && world->clock.prev_time > gun->lastshottime + gun->stats.firedelay)
		gun->readytoshoot = true;
	if (gun->bullets == 0 || gun->reload_anim.active)
		gun->readytoshoot = false;
	if (player->input.shoot && gun->readytoshoot)
	{
		start_anim(&gun->shoot_anim, anim_forwards);
		start_anim(&gun->view_anim, anim_forwards);
		printf("shot delta %i \n", world->clock.prev_time - gun->lastshottime);
		gun->readytoshoot = false;
		gun->lastshottime = world->clock.prev_time;
		gun->bullets--;
		play_gun_audio(gun, world);
		player_gun_raycast(player, world);
	}
	if (gun->bullets != gun->stats.magazinecapacity && player->input.reload && !gun->reload_anim.active)
	{
		if (((player->ammo_union.mask >> 8 * gun->stats.ammomask) & 0xFF) > 0)
		{
			gun->reload_anim.framerate = 30;
			gun->reload_anim.lastframe = gun->stats.reloadtime;
			player->ammo_union.ammo_arr[gun->stats.ammomask] += gun->bullets;
			start_anim(&gun->reload_anim, anim_forwards);
			play_gun_reload_audio(world);
		}
		//if ((player->ammo_union.mask >> 8 * gun->stats.ammomask) & 0xFF > 0)
		//{
		
		//}
	}
	if (gun->shoot_anim.active)
		update_anim(&gun->shoot_anim, world->clock.delta);
	else
		gun->shoot_anim.lerp = 0.0f;
	update_anim(&gun->view_anim, world->clock.delta);
	//recoil.y:
	if (!gun->reload_anim.active)
	{
		gun->entity->transform.rotation.x = ft_degtorad(0.0f);
		if (player->input.aim)
			lerp += gun->stats.ads_speed * world->clock.delta;
		else
			lerp -= (gun->stats.ads_speed * 1.25f) * world->clock.delta;
		lerp = ft_clampf(lerp, 0.0f, 1.0f);
		player->fov = 90.0f + (lerp * gun->stats.fov_offset);
		neutralpos = vector3_lerp(gun->stats.holsterpos, gun->stats.aimpos, lerp);
		gun->entity->transform.position = neutralpos;
		if (gun->shoot_anim.active)
		gun->entity->transform.position = vector3_add(gun->entity->transform.position, vector3_mul(vector3_up(), gun->shoot_anim.lerp * gun->stats.recoiljump.y));
	//bobbing:
		gun->entity->transform.position.z += vector2_magnitude(player->input.move) * cosf((world->clock.prev_time * 0.007f)) * 0.05f;
		gun->entity->transform.position.z = ft_fmovetowards(gun->entity->transform.position.z, gun->entity->transform.position.z + player->cp.new_velocity.z, world->clock.delta * 0.1f);
		gun->entity->transform.rotation.z += vector2_magnitude(player->input.move) * cosf((world->clock.prev_time * 0.007f)) * ft_degtorad(0.15f);
		//recoilrecovery:
		gun->entity->transform.rotation.y = fmovetowards(gun->entity->transform.rotation.y, ft_degtorad(player->input.move.y * 1.15f), gun->stats.anglerecovery * world->clock.delta);
		//recoil.y:
		if (gun->shoot_anim.active)
		{
			gun->entity->transform.rotation.y += ft_flerp(0.0f, ft_degtorad(gun->stats.recoilangle.y), gun->shoot_anim.lerp);
		}
		gun->entity->transform.rotation.y = ft_clampf(gun->entity->transform.rotation.y, ft_degtorad(-0.5f), ft_degtorad(15.0f));
		//gun->entity->transform.rotation.x = ft_flerp(0.0f, ft_degtorad(2.0f), gun->shoot_anim.lerp);
		//Slight angle when player moves:
		float	zturn = player->input.move.x * 2.0f;
		if (!player->input.aim)
			zturn += player->input.turn.x * 5.0f;
		zturn = ft_clampf(zturn, ft_degtorad(-2.5f), ft_degtorad(2.5f));
		if (player->locked)
			return ;
		gun->entity->transform.rotation.z = fmovetowards(gun->entity->transform.rotation.z, zturn, 0.0004f * world->clock.delta);
		if (gun->view_anim.active)
			player->transform.rotation.y += gun->view_anim.lerp * world->clock.delta * gun->stats.viewrecoil.y; //Separate view jump animation that is longer than gun jump animation?
	}
	else
	{
		update_anim(&gun->reload_anim, world->clock.delta);
		if (!gun->reload_anim.active)
		{
			uint32_t bullets_added;

			bullets_added = ft_min(player->ammo_union.ammo_arr[gun->stats.ammomask], gun->stats.magazinecapacity);
			gun->bullets = bullets_added;
			player->ammo_union.ammo_arr[gun->stats.ammomask] -= bullets_added;
		}
		float midlerp = 1.0f - (ft_absf(0.5f - gun->reload_anim.lerp) * 2.0f);
		gun->entity->transform.position.z -= midlerp * 2.2f;
		//gun->entity->transform.rotation.x = ft_degtorad(15.0f * midlerp);
		gun->entity->transform.rotation.x = ft_degtorad(25.0f * midlerp);
		gun->entity->transform.rotation.z = ft_degtorad(30.0f * midlerp);
		//gun->entity->transform.rotation.z = ft_degtorad(30.0f);
		//gun->entity->transform.rotation.x = ft_degtorad(15.0f);
	}
}

static void noclip_movement(t_player *player, t_vector3 move_vector, t_world *world)
{
	player->transform.position = vector3_add(player->transform.position, move_vector);
}

void	moveplayer(t_player *player, t_input *input, t_world *world)
{
	t_vector3	move_vector;
	float		angle;

	player->gun->entity->transform.parent = &player->head_transform;
	player->input = *input; //TODO: move to update_input?;
	if (world->gamemode == MODE_PLAY)
		updateguntransform(player, world);
	if (!player->locked)
	{
		if (!player->noclip)
			playermovement_normal(player, world);
		else
			playermovement_noclip(player, world);
	}
	if ((player->input.nextgun || player->input.prevgun) && !player->gun->reload_anim.active)
	{
		player->gun_ammos[player->gun_selection] = player->gun->bullets;
		player->gun_selection += player->input.nextgun;
		player->gun_selection -= player->input.prevgun;
		player->gun_selection %= GUNPRESETCOUNT;
		change_gun_preset(player->gun, world->sdl, player->gun_selection);
		player->gun->bullets = player->gun_ammos[player->gun_selection];
	}
	player->headposition = vector3_add(player->transform.position, (t_vector3){.z = player->height * 0.75f});
	player->head_transform.position = player->headposition;
	player->head_transform.rotation = player->transform.rotation;
	player->head_transform.scale = player->transform.scale;
}
