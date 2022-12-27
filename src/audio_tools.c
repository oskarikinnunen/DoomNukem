/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:14:16 by raho              #+#    #+#             */
/*   Updated: 2022/12/27 12:22:52 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

int	find_sound(t_audio *audio, const char *name)
{
	int	index;

	index = 0;
	while (index < audio->samplecount)
	{
		if (ft_strequ(audio->sample[index].name, name))
			return (index);
		index++;
	}
	return (-1);
}

int	find_music(t_audio *audio, const char *name)
{
	int	index;

	index = 0;
	while (index < audio->musiccount)
	{
		if (ft_strequ(audio->music[index].name, name))
			return (index);
		index++;
	}
	return (-1);
}

int	check_channel_status(FMOD_CHANNEL *channel)
{
	FMOD_BOOL	isplaying;

	if (FMOD_Channel_IsPlaying(channel, &isplaying) != FMOD_OK)
		error_log(EC_FMOD_CHANNELISPLAYING);
	return (isplaying);
}

static void	limit_volume(float *volume)
{
	if (*volume > 1.0f)
		*volume = 1.0f;
	if (*volume < 0.0f)
		*volume = 0.0f;
}

void	update_volume(t_audio *audio)
{
	int	index;

	limit_volume(&audio->max_volume);
	index = 0;
	while (index < audio->samplecount)
	{
		if (audio->sample[index].channel)
		{
			if (check_channel_status(audio->sample[index].channel))
			{
				if (FMOD_Channel_SetVolume(audio->sample[index].channel, audio->max_volume) != FMOD_OK)
					error_log(EC_FMOD_CHANNELSETPAUSED);
			}
		}
		if (audio->music[index].channel)
		{
			if (check_channel_status(audio->music[index].channel))
			{
				if (FMOD_Channel_SetVolume(audio->music[index].channel, audio->max_volume) != FMOD_OK)
					error_log(EC_FMOD_CHANNELSETPAUSED);
			}
		}
		index++;
	}
}
