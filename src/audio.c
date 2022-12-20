/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:14:01 by raho              #+#    #+#             */
/*   Updated: 2022/12/20 12:20:01 by okinnune         ###   ########.fr       */
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
	
	result = FMOD_System_CreateSound(audio->system, "assets/audio/rock-music.wav", FMOD_DEFAULT, NULL, &audio->sample[0].sound);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_CreateSound error! (%d)\n", result);
		exit (-1);
	}
	ft_strcpy(audio->sample[0].name, "rock-music.wav");
	
	result = FMOD_System_CreateSound(audio->system, "assets/audio/bubbles.wav", FMOD_DEFAULT, NULL, &audio->sample[1].sound);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_CreateSound error! (%d)\n", result);
		exit (-1);
	}
	ft_strcpy(audio->sample[1].name, "bubbles.wav");
	
	result = FMOD_System_CreateSound(audio->system, "assets/audio/pistol1.wav", FMOD_DEFAULT, NULL, &audio->sample[2].sound);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_CreateSound error! (%d)\n", result);
		exit (-1);
	}
	ft_strcpy(audio->sample[2].name, "pistol1.wav");
	
	result = FMOD_System_CreateSound(audio->system, "assets/audio/pistol2.wav", FMOD_DEFAULT, NULL, &audio->sample[3].sound);
	if (result != FMOD_OK)
	{
		printf("FMOD_System_CreateSound error! (%d)\n", result);
		exit (-1);
	}
	ft_strcpy(audio->sample[3].name, "pistol2.wav");
	
	audio->samplecount = 4;
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
