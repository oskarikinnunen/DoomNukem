/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_sounds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:14:01 by raho              #+#    #+#             */
/*   Updated: 2023/01/15 03:00:46 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

void	pause_audio(t_audio *audio, bool pause)
{
	//pause_all_sounds(audio, pause);
	//pause_music(audio, pause);
}

/*void	pause_all_sounds(t_audio *audio, bool pause)
{
	int	index;

	index = 0;
	while (index < audio->samplecount)
	{
		if (audio->sample[index].channel)
		{
			if (FMOD_Channel_SetVolume(audio->sample[index].channel, audio->max_volume) != FMOD_OK)
				error_log(EC_FMOD_CHANNELSETVOLUME);
			if (check_channel_status(audio->sample[index].channel))
			{
				if (FMOD_Channel_SetPaused(audio->sample[index].channel, pause) != FMOD_OK)
					error_log(EC_FMOD_CHANNELSETPAUSED);
			}
		}
		index++;
	}
}*/

/*void	pause_sound(t_audio *audio, const char *name, bool pause)
{
	int	index;

	index =	find_sound(audio, name);
	if (index >= 0)
	{
		if (audio->sample[index].channel)
		{
			if (FMOD_Channel_SetVolume(audio->sample[index].channel, audio->max_volume) != FMOD_OK)
				error_log(EC_FMOD_CHANNELSETVOLUME);
			if (check_channel_status(audio->sample[index].channel))
			{
				if (FMOD_Channel_SetPaused(audio->sample[index].channel, pause) != FMOD_OK)
					error_log(EC_FMOD_CHANNELSETPAUSED);
			}
		}
	}
	else
		printf("Couldn't find sound: %s\n", name);
}*/

/*void	play_localsound(t_audio *audio, const char *name)
{
	int	index;

	index =	find_sound(audio, name);
	if (index >= 0)
	{
		if (FMOD_System_PlaySound(audio->system, audio->sample[index].sound, NULL, true, &audio->sample[index].channel) != FMOD_OK)
			error_log(EC_FMOD_SYSTEMPLAYSOUND);
		if (FMOD_Channel_SetVolume(audio->sample[index].channel, audio->max_volume) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSETVOLUME);
		if (FMOD_Channel_SetPaused(audio->sample[index].channel, false) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSETPAUSED);
	}
	else
		printf("Couldn't find sound: %s\n", name);
}*/

/*void	change_music(t_sdlcontext *sdl, char *musicname)
{
	
}*/

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

void	audiosource_start(t_sdlcontext *sdl, t_audiosource	*source, t_vector3 *pos) //TODO: remove pos?
{
	if (FMOD_System_PlaySound(sdl->audio.system, source->sample.sound, NULL, true, &source->channel) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMPLAYSOUND);
	//FMOD_Channel_SetReverbProperties()
	
	if (FMOD_Channel_Set3DAttributes(source->channel, pos, &((t_vector3){0})) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSET3DATTRIBUTES);
	//FMOD_System_Set3DSettings()
	FMOD_Channel_Set3DMinMaxDistance(source->channel, source->_realrange, 10000.0f);
	//FMOD_Channel_Set3D
	//FMOD_System_Set3DRolloffCallback()
	
	if (FMOD_Channel_SetVolume(source->channel, source->volume * sdl->audio.sfx_volume) != FMOD_OK)
		error_log(EC_FMOD_CHANNELSETVOLUME);
	if (FMOD_Channel_SetPaused(source->channel, false) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSETPAUSED);
}
