/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:14:01 by raho              #+#    #+#             */
/*   Updated: 2022/12/21 18:57:23 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"
#include "fmod.h"


void	play_music(t_audio *audio)
{
}

void	play_sound(t_audio *audio, const char *name)
{
	FMOD_RESULT	result;
	int			index;

	index = 0;
	while (index < audio->samplecount)
	{
		if (ft_strequ(audio->sample[index].name, name))
			break;
		index++;
	}

	result = FMOD_System_PlaySound(audio->system, audio->sample[index].sound, NULL, true, &audio->sample[index].channel);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_PlaySound error! (%d)\n", result);
		exit (-1);
	}
	
	result = FMOD_Channel_SetVolume(audio->sample[index].channel, audio->max_volume);
	if (result != FMOD_OK)
	{
		printf("FMOD_Channel_SetVolume error! (%d)\n", result);
		exit (-1);
	}

	result = FMOD_Channel_SetPaused(audio->sample[index].channel, false);
	if (result != FMOD_OK)
	{
		printf("FMOD_Channel_SetPaused error! (%d)\n", result);
		exit (-1);
	}
}

void	play_sound3d(t_audio *audio, const char *name, t_vector3 *pos)
{
	FMOD_RESULT	result;
	FMOD_BOOL	isplaying;
	int			index;

	index = 0;
	while (index < audio->samplecount)
	{
		if (ft_strequ(audio->sample[index].name, name))
			break;
		index++;
	}

	if (audio->sample[index].channel)
	{
		result = FMOD_Channel_IsPlaying(audio->sample[index].channel, &isplaying);
		if (result != FMOD_OK)
		{
			printf("FMOD_Channel_IsPlaying error! (%d)\n", result);
			exit (-1);
		}
		if (isplaying)
			return ;
	} 

	result = FMOD_System_PlaySound(audio->system, audio->sample[index].sound, NULL, true, &audio->sample[index].channel);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_PlaySound error! (%d)\n", result);
		exit (-1);
	}
	
	result = FMOD_Channel_Set3DAttributes(audio->sample[index].channel, pos, &((t_vector3){0}));
	if (result != FMOD_OK)
	{
		printf("FMOD_System_Channel_Set3DAttributes error! (%d)\n", result);
		exit (-1);
	}

	result = FMOD_Channel_SetPaused(audio->sample[index].channel, false);
	if (result != FMOD_OK)
	{
		printf("FMOD_Channel_SetPaused error! (%d)\n", result);
		exit (-1);
	}
}

void	close_audio(t_audio *audio)
{
	FMOD_RESULT result;

	result = FMOD_System_Close(audio->system);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_Close error! (%d)\n", result);
		exit (-1);
	}

	result = FMOD_System_Release(audio->system);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_Release error! (%d)\n", result);
		exit (-1);
	}
}

void	set_max_volume(t_audio *audio, float nb)
{
	audio->max_volume = nb;
}

static void	load_sounds(t_audio *audio)
{
	FMOD_RESULT	result;
	int			index;
	
	index = 0;
	result = FMOD_System_CreateSound(audio->system, "assets/audio/rock-music.wav", FMOD_3D, NULL, &audio->sample[index].sound);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_CreateSound error! (%d)\n", result);
		exit (-1);
	}
	ft_strcpy(audio->sample[index].name, "rock-music.wav");
	index++;

	result = FMOD_System_CreateSound(audio->system, "assets/audio/bubbles.wav", FMOD_3D, NULL, &audio->sample[index].sound);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_CreateSound error! (%d)\n", result);
		exit (-1);
	}
	ft_strcpy(audio->sample[index].name, "bubbles.wav");
	index++;
	
	result = FMOD_System_CreateSound(audio->system, "assets/audio/pistol1.wav", FMOD_3D, NULL, &audio->sample[index].sound);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_CreateSound error! (%d)\n", result);
		exit (-1);
	}
	ft_strcpy(audio->sample[index].name, "pistol1.wav");
	index++;
	
	result = FMOD_System_CreateSound(audio->system, "assets/audio/pistol2.wav", FMOD_3D, NULL, &audio->sample[index].sound);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_CreateSound error! (%d)\n", result);
		exit (-1);
	}
	ft_strcpy(audio->sample[index].name, "pistol2.wav");
	index++;
	
	result = FMOD_System_CreateSound(audio->system, "assets/audio/scifi-rapidfire.mp3", FMOD_3D, NULL, &audio->sample[index].sound);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_CreateSound error! (%d)\n", result);
		exit (-1);
	}
	ft_strcpy(audio->sample[index].name, "scifi-rapidfire.mp3");
	index++;
	
	audio->samplecount = index;
}

void	load_audio(t_audio *audio)
{	
	FMOD_RESULT	result;

	ft_bzero(audio, sizeof(t_audio));

	result = FMOD_System_Create(&audio->system, FMOD_VERSION);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_Create error! (%d)\n", result);
		exit (-1);
	}
	result = FMOD_System_Init(audio->system, 20, FMOD_INIT_NORMAL, FMOD_OUTPUTTYPE_AUTODETECT);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_Init error! (%d)\n", result);
		exit (-1);
	}
	load_sounds(audio);
	set_max_volume(audio, 1.0f);
	
}
