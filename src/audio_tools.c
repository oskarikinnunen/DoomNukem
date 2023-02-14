/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:14:16 by raho              #+#    #+#             */
/*   Updated: 2023/02/07 15:41:59 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

t_audiosample	get_music(t_sdlcontext *sdl, const char *name)
{
	int				i;
	t_audiosample	error;

	i = 0;
	while (i < sdl->audio.music_count)
	{
		if (ft_strequ(sdl->audio.music[i].name, name))
			return (sdl->audio.music[i]);
		i++;
	}
	ft_bzero(&error, sizeof(t_audiosample));
	return (error);
}

t_audiosample	get_sample(t_sdlcontext *sdl, const char *name)
{
	int	i;
	t_audiosample	error;

	i = 0;
	while (i < sdl->audio.samplecount)
	{
		if (ft_strequ(sdl->audio.samples[i].name, name))
			return (sdl->audio.samples[i]);
		i++;
	}
	ft_bzero(&error, sizeof(t_audiosample));
	return (error);
}

//TODO: sometimes channel is invalid when the sound is done playing but the address still stays on the pointer. 
int	check_channel_status(FMOD_CHANNEL *channel)
{
	FMOD_BOOL	isplaying;

	isplaying = 0;
	if (FMOD_Channel_IsPlaying(channel, &isplaying) != FMOD_OK)
		doomlog(LOG_EC_FMOD_CHANNELISPLAYING, NULL);
	return (isplaying);
}
