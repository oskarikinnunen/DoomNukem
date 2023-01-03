/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:14:16 by raho              #+#    #+#             */
/*   Updated: 2023/01/03 13:54:49 by raho             ###   ########.fr       */
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

//TODO: sometimes channel is invalid when the sound is done playing but the address still stays on the pointer. 
int	check_channel_status(FMOD_CHANNEL *channel)
{
	FMOD_BOOL	isplaying;

	isplaying = 0;
	if (FMOD_Channel_IsPlaying(channel, &isplaying) != FMOD_OK)
		error_log(EC_FMOD_CHANNELISPLAYING);
	return (isplaying);
}

void	update_maxvolume(t_audio *audio)
{
	audio->max_volume = ft_clampf(audio->max_volume, 0.0f, 1.0f);
}
