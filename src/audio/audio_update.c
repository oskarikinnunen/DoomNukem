/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_update.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 21:18:29 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 20:46:13 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	music_control(uint32_t delta_time, t_sdlcontext *sdl)
{
	t_music_control	*mc;

	mc = &sdl->audio.music_control;
	if (!mc->lastfade)
	{
		mc->fade = ft_fmovetowards(mc->fade, 0.0f, delta_time * \
				0.0005f * sdl->audio.music_volume);
		if (mc->fade == 0.0f)
		{
			FMOD_Channel_SetVolume(sdl->audio.music_channel, 0.0f);
			FMOD_Channel_SetPaused(sdl->audio.music_channel, true);
			FMOD_System_PlaySound(sdl->audio.system, mc->nextsong.sound, \
					NULL, true, &sdl->audio.music_channel);
			FMOD_Channel_SetPaused(sdl->audio.music_channel, false);
			mc->lastfade = true;
		}
	}
	else
	{
		mc->fade = ft_fmovetowards(mc->fade, sdl->audio.music_volume, \
				delta_time * 0.00025f * sdl->audio.music_volume);
		if (mc->fade == sdl->audio.music_volume)
			mc->active = false;
	}
}

void	update_audio(t_world *world)
{
	t_vector3		nf;
	t_sdlcontext	*sdl;

	sdl = world->sdl;
	nf = world->player->look_dir;
	nf = (t_vector3){-nf.x, -nf.y, 0.0f};
	nf = vector3_normalise(nf);
	if (sdl->audio.music_control.active)
	{
		music_control(world->clock.delta, sdl);
		FMOD_Channel_SetVolume(sdl->audio.music_channel, \
				sdl->audio.music_control.fade);
	}
	FMOD_System_Set3DListenerAttributes(sdl->audio.system, 0, \
			(FMOD_VECTOR *)&world->player->head_position, &((FMOD_VECTOR){0}), \
			(FMOD_VECTOR *)&nf, &((FMOD_VECTOR){.z = 1.0f}));
	FMOD_System_Update(sdl->audio.system);
}
