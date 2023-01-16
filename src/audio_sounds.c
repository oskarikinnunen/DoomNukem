/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_sounds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:14:01 by raho              #+#    #+#             */
/*   Updated: 2023/01/16 19:19:48 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

void	pause_audio(t_audio *audio, bool pause)
{
	//pause_all_sounds(audio, pause);
	//pause_music(audio, pause);
}

void	change_music(t_sdlcontext *sdl, char *musicname)
{
	t_audiosample	sample;

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
	t_audiosample	sample;

	sample = get_music(sdl, musicname);
	if (sample.sound != NULL)
	{
		printf("found sample for music %s \n", musicname);
		FMOD_System_PlaySound(sdl->audio.system, sample.sound, NULL, false, &sdl->audio.music_channel);
	}
}

void	play_quick_audio(t_vector3 pos, char *samplename)
{
	
}
