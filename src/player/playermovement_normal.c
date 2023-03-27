/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playermovement_normal.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:22:48 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:18:09 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"

void	play_footstepsound(t_player *player, t_world *world)
{
	static t_audiosource	source;
	float					lerp;

	if (source.sample.sound == NULL)
	{
		source.sample = get_sample(world->sdl, "concrete-footsteps_loop.wav");
		source._real_range = 50.0f;
		source.volume = 1.0f;
		source.play_always = true;
		_audiosource_start(world->sdl, &source, &player->transform.position);
	}
	lerp = vector2_magnitude(v3tov2(player->cp.velocity)) * 10.0f;
	source.volume = lerp * 0.20f;
	if (!player->cp.is_grounded)
	{
		source.volume = 0.0f;
		return ;
	}
	FMOD_Channel_SetPitch(source.channel, ft_clampf(lerp, 1.0f, 1.35f));
	FMOD_Channel_Set3DAttributes(source.channel,
		(FMOD_VECTOR *)&player->transform.position, &((FMOD_VECTOR){0}));
	FMOD_Channel_SetVolume(source.channel,
		source.volume * world->sdl->audio.sfx_volume);
}

//between 0-3000 because game_random_range works less randomly with 0-3
t_audio_sample	random_climbsound(t_world *world)
{
	uint32_t	r;

	r = game_random_range(world, 0, 3000);
	if (r < 1000)
		return (get_sample(world->sdl, "climb1.wav"));
	if (r > 1000 && r < 2000)
	{
		return (get_sample(world->sdl, "climb2.wav"));
	}
	return (get_sample(world->sdl, "climb3.wav"));
}

void	play_landingsound(t_player *player, t_world *world)
{
	static t_audiosource	source;
	float					vlerp;

	if (player->cp.impact_velocity.z > 0.0f)
	{
		source.volume = 0.2f;
		source.sample = random_climbsound(world);
		source._real_range = ft_flerp(40.0f, 160.0f, vlerp);
		_audiosource_start(world->sdl, &source, &player->head_position);
	}
	else
	{
		vlerp = player->cp.impact_velocity.z / (GRAVITY);
		source.volume = ft_clampf(vlerp, 0.2f, 0.4f);
		source._real_range = ft_flerp(40.0f, 160.0f, vlerp);
		if (vlerp < 0.38f)
			source.sample = get_sample(world->sdl, "landing_impact_soft.wav");
		else
			source.sample = get_sample(world->sdl, "landing_impact_hard.wav");
		_audiosource_start(world->sdl, &source, &player->transform.position);
	}
}

void	player_movement_normal(t_player *player, t_world *world)
{
	player_update_physics(player, world);
	player_rotate(player);
	player_move(player, world);
	crouch_update(player, world);
	player_jump(player, world);
	player_update_animations(player, world);
	player_update_physics(player, world);
	capsule_phys_step(&player->cp, world);
	play_footstepsound(player, world);
	if (player->cp.landing_trigger)
	{
		play_landingsound(player, world);
		player->cp.landing_trigger = false;
	}
	player_ceiling_check(player);
}
