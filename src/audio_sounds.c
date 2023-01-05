/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_sounds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:14:01 by raho              #+#    #+#             */
/*   Updated: 2023/01/03 13:52:11 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

void	pause_audio(t_audio *audio, bool pause)
{
	pause_all_sounds(audio, pause);
	pause_music(audio, pause);
}

void	pause_all_sounds(t_audio *audio, bool pause)
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
}

void	pause_sound(t_audio *audio, const char *name, bool pause)
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
}

void	play_localsound(t_audio *audio, const char *name)
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
}

void	play_worldsound(t_audio *audio, const char *name, t_vector3 *pos)
{
	int			index;

	index =	find_sound(audio, name);
	if (index >= 0)
	{
		if (FMOD_System_PlaySound(audio->system, audio->sample[index].sound, NULL, true, &audio->sample[index].channel) != FMOD_OK)
			error_log(EC_FMOD_SYSTEMPLAYSOUND);
		if (FMOD_Channel_Set3DAttributes(audio->sample[index].channel, pos, &((t_vector3){0})) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSET3DATTRIBUTES);
		if (FMOD_Channel_SetVolume(audio->sample[index].channel, audio->max_volume) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSETVOLUME);
		if (FMOD_Channel_SetPaused(audio->sample[index].channel, false) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSETPAUSED);
	}
	else
		printf("Couldn't find sound: %s\n", name);
}
