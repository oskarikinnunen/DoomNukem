/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_volume.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 13:59:02 by raho              #+#    #+#             */
/*   Updated: 2022/12/27 14:14:20 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

static void	update_maxvolume_music(t_audio *audio)
{
	int	index;

	index = 0;
	while (index < audio->musiccount)
	{
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

static void	update_maxvolume_sounds(t_audio *audio)
{
	int	index;

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
		index++;
	}
}

void	update_maxvolume(t_audio *audio)
{
	audio->max_volume = ft_clampf(audio->max_volume, 0.0f, 1.0f);
	update_maxvolume_sounds(audio);
	update_maxvolume_music(audio);
}
