/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_audiosource_tools.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:41 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/16 19:58:13 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool	source_is_playing(t_audiosource *source)
{
	FMOD_BOOL	isplaying;

	isplaying = false;
	if (source->channel != NULL)
	{
		if (FMOD_Channel_IsPlaying(source->channel, &isplaying) != FMOD_OK)
			isplaying = false;
	}
	return (isplaying);
}

void	_audiosource_start(t_sdlcontext *sdl, t_audiosource	*source, t_vector3 *pos)
{
	if (FMOD_System_PlaySound(sdl->audio.system, source->sample.sound, NULL, true, &source->channel) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMPLAYSOUND);
	//FMOD_Channel_SetReverbProperties()
	
	if (FMOD_Channel_Set3DAttributes(source->channel, (FMOD_VECTOR *)pos, &((FMOD_VECTOR){0})) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSET3DATTRIBUTES);
	FMOD_Channel_Set3DMinMaxDistance(source->channel, source->_realrange, 10000.0f);
	if (FMOD_Channel_SetVolume(source->channel, source->volume * sdl->audio.sfx_volume) != FMOD_OK)
		error_log(EC_FMOD_CHANNELSETVOLUME);
	if (FMOD_Channel_SetPaused(source->channel, false) != FMOD_OK)
			error_log(EC_FMOD_CHANNELSETPAUSED);
}