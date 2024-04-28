/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_sounds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:14:01 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 20:35:11 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

void	change_music(t_sdlcontext *sdl, char *musicname)
{
	t_audio_sample	sample;

	sample = get_music(sdl, musicname);
	if (sample.sound != NULL)
	{
		sdl->audio.music_control.nextsong = sample;
		sdl->audio.music_control.active = true;
		sdl->audio.music_control.fade = sdl->audio.music_volume;
		sdl->audio.music_control.lastfade = false;
	}
}

void	play_music(t_sdlcontext *sdl, char *musicname)
{
	t_audio_sample	sample;

	sample = get_music(sdl, musicname);
	if (sample.sound != NULL)
		FMOD_System_PlaySound(sdl->audio.system, sample.sound, \
				NULL, false, &sdl->audio.music_channel);
}
