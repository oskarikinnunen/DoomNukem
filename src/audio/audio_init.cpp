/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 10:42:33 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 12:29:03 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"
#include "file_io.h"

FMOD_MODE	get_mask(char *sound_path)
{
	if (ft_strstr(sound_path, "loop") != NULL)
		return (FMOD_3D | FMOD_LOOP_NORMAL);
	return (FMOD_3D | FMOD_LOOP_OFF);
}

void	create_sound(int sample_i, char *sound_path, t_audio *audio)
{
	if (FMOD_System_CreateSound(audio->system, sound_path, \
			get_mask(sound_path), NULL, \
			&audio->samples[sample_i].sound) != FMOD_OK)
		doomlog(LOG_EC_FMOD_SYSTEMCREATESOUND, NULL);
}

void	create_music(int music_i, char *music_path, t_audio *audio)
{
	if (FMOD_System_CreateSound(audio->system, music_path, \
			FMOD_2D | FMOD_LOOP_NORMAL, NULL, \
			&audio->music[music_i].sound) != FMOD_OK)
		doomlog(LOG_EC_FMOD_SYSTEMCREATESOUND, NULL);
}

void	create_audio(t_audio *audio)
{
	ft_bzero(audio, sizeof(t_audio));
	doomlog(LOG_NORMAL, "LOADING AUDIO");
	if (FMOD_System_Create(&audio->system, FMOD_VERSION) != FMOD_OK)
		doomlog(LOG_EC_FMOD_SYSTEMCREATE, NULL);
	if (FMOD_System_Init(audio->system, 30, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
		doomlog(LOG_EC_FMOD_SYSTEMINIT, NULL);
	FMOD_System_Set3DSettings(audio->system, 1.0f, 100.0f, 2.0f);
}
