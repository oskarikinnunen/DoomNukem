/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 10:42:33 by raho              #+#    #+#             */
/*   Updated: 2022/12/27 12:24:52 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

static void load_sounds2(t_audio *audio, int index)
{
	audio->samplecount = index;
}

static void	load_sounds1(t_audio *audio)
{
	int			index;
	
	index = 0;
	if (FMOD_System_CreateSound(audio->system, "assets/audio/bubbles.wav", FMOD_3D, NULL, &audio->sample[index].sound) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMCREATESOUND);
	ft_strcpy(audio->sample[index].name, "bubbles.wav");
	index++;
	if (FMOD_System_CreateSound(audio->system, "assets/audio/pistol1.wav", FMOD_3D, NULL, &audio->sample[index].sound) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMCREATESOUND);
	ft_strcpy(audio->sample[index].name, "pistol1.wav");
	index++;
	if (FMOD_System_CreateSound(audio->system, "assets/audio/pistol2.wav", FMOD_3D, NULL, &audio->sample[index].sound) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMCREATESOUND);
	ft_strcpy(audio->sample[index].name, "pistol2.wav");
	index++;
	if (FMOD_System_CreateSound(audio->system, "assets/audio/scifi-rapidfire.mp3", FMOD_3D, NULL, &audio->sample[index].sound) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMCREATESOUND);
	ft_strcpy(audio->sample[index].name, "scifi-rapidfire.mp3");
	index++;
	load_sounds2(audio, index);
}

static void	load_music(t_audio *audio)
{
	int			index;

	index = 0;
	if (FMOD_System_CreateSound(audio->system, "assets/audio/rock-music.wav", FMOD_LOOP_NORMAL, NULL, &audio->music[index].sound) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMCREATESOUND);
	ft_strcpy(audio->music[index].name, "rock-music.wav");
	index++;
	audio->musiccount = index;
}

void	set_max_volume(t_audio *audio, float nb)
{
	audio->max_volume = nb;
}

void	load_audio(t_audio *audio)
{
	ft_bzero(audio, sizeof(t_audio));
	if (FMOD_System_Create(&audio->system, FMOD_VERSION) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMCREATE);
	if (FMOD_System_Init(audio->system, 30, FMOD_INIT_NORMAL, FMOD_OUTPUTTYPE_AUTODETECT) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMINIT);
	load_sounds1(audio);
	load_music(audio);
	set_max_volume(audio, 1.0f);
}