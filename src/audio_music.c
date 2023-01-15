/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_music.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:27:24 by raho              #+#    #+#             */
/*   Updated: 2023/01/03 13:53:40 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

void	change_music(t_audio *audio, const char *name)
{
	stop_music(audio);
	play_music(audio, name);
}

void	stop_music(t_audio *audio)
{
	int	index;
	
	index = 0;
	while (index < audio->musiccount)
	{
		if (audio->music[index].channel)
		{
			if (check_channel_status(audio->music[index].channel))
			{
				if (FMOD_Channel_Stop(audio->music[index].channel) != FMOD_OK)
					error_log(EC_FMOD_CHANNELSTOP);
			}
		}
		index++;
	}
}

void	pause_music(t_audio *audio,bool pause)
{
	int	index;
	
	index = 0;
	while (index < audio->musiccount)
	{
		if (audio->music[index].channel)
		{
			if (FMOD_Channel_SetVolume(audio->music[index].channel, audio->max_volume) != FMOD_OK)
				error_log(EC_FMOD_CHANNELSETVOLUME);
			if (check_channel_status(audio->music[index].channel))
			{
				if (FMOD_Channel_SetPaused(audio->music[index].channel, pause) != FMOD_OK)
					error_log(EC_FMOD_CHANNELSETPAUSED);
			}
		}
		index++;
	}
}

void	play_music(t_audio *audio, const char *name)
{
	int			index;

	index = find_music(audio, name);
	if (index >= 0)
	{
		if (FMOD_System_PlaySound(audio->system, audio->music[index].sound, NULL, true, &audio->music[index].channel) != FMOD_OK)
			error_log(EC_FMOD_SYSTEMPLAYSOUND);
		if (FMOD_Channel_SetVolume(audio->music[index].channel, audio->max_volume) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSETVOLUME);
		if (FMOD_Channel_SetPaused(audio->music[index].channel, false) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSETPAUSED);
	}
	else
		printf("Couldn't find music: %s\n", name);
}