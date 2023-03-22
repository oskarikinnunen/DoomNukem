/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 10:42:33 by raho              #+#    #+#             */
/*   Updated: 2023/03/20 18:29:28 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"
#include <dirent.h>
#include "file_io.h"

FMOD_MODE	get_mask(char *sound_path)
{
	if (ft_strstr(sound_path, "loop") != NULL)
		return (FMOD_3D | FMOD_LOOP_NORMAL);
	return (FMOD_3D | FMOD_LOOP_OFF);
}

static void load_sound(t_audio *audio)
{
	char	*sound_path;
	int		ret;
	int		fd;
	int		i;

	doomlog(LOG_NORMAL, "LOADING SOUNDS");
	audio->samplecount = count_asset_list(SOUNDLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){SOUNDLISTPATH, "samplecount =", s_itoa(audio->samplecount), NULL});
	audio->samples = ft_memalloc(sizeof(t_audiosample) * audio->samplecount);
	if (audio->samples == NULL)
		doomlog(LOG_EC_MALLOC, "audio->samples");
	fd = fileopen(SOUNDLISTPATH, O_RDONLY);
	sound_path = NULL;
	ret = get_next_line(fd, &sound_path);
	i = 0;
	while (ret)
	{
		if (sound_path)
		{
			if (FMOD_System_CreateSound(audio->system, sound_path, get_mask(sound_path), NULL, &audio->samples[i].sound) != FMOD_OK)
				doomlog(LOG_EC_FMOD_SYSTEMCREATE, NULL);
			if (audio->samples[i].sound != NULL)
				ft_strncpy(audio->samples[i].name, extract_filename(sound_path), sizeof(audio->samples[i].name));
			doomlog_mul(LOG_NORMAL, (char *[3]){"loaded sound file:", audio->samples[i].name, NULL});
			free(sound_path);
			sound_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &sound_path);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, SOUNDLISTPATH);
	fileclose(fd, SOUNDLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){"loaded", s_itoa(i), "sound files", NULL});
}

static void load_music(t_audio *audio)
{
	char	*music_path;
	int		ret;
	int		fd;
	int		i;

	doomlog(LOG_NORMAL, "LOADING MUSIC");
	audio->music_count = count_asset_list(MUSICLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){MUSICLISTPATH, "music_count =", s_itoa(audio->samplecount), NULL});
	audio->music = ft_memalloc(sizeof(t_audiosample) * audio->music_count);
	if (audio->music == NULL)
		doomlog(LOG_EC_MALLOC, "audio->music");
	fd = fileopen(MUSICLISTPATH, O_RDONLY);
	music_path = NULL;
	ret = get_next_line(fd, &music_path);
	i = 0;
	while (ret)
	{
		if (music_path)
		{
			if (FMOD_System_CreateSound(audio->system, music_path, FMOD_2D | FMOD_LOOP_NORMAL, NULL, &audio->music[i].sound) != FMOD_OK)
				doomlog(LOG_EC_FMOD_SYSTEMCREATE, NULL);
			if (audio->music[i].sound != NULL)
				ft_strncpy(audio->music[i].name, extract_filename(music_path), sizeof(audio->music[i].name));
			doomlog_mul(LOG_NORMAL, (char *[3]){"loaded music file:", audio->music[i].name, NULL});
			free(music_path);
			music_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &music_path);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, MUSICLISTPATH);
	fileclose(fd, MUSICLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){"loaded", s_itoa(i), "music files", NULL});
}

void	editor_load_audio(t_audio *audio)
{
	ft_bzero(audio, sizeof(t_audio));
	audio->system = NULL; // TODO: use the bzero but apply volume settings from sdl somehow
	doomlog(LOG_NORMAL, "LOADING AUDIO");
	if (FMOD_System_Create(&audio->system, FMOD_VERSION) != FMOD_OK)
		doomlog(LOG_EC_FMOD_SYSTEMCREATE, NULL);
	if (FMOD_System_Init(audio->system, 30, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
		doomlog(LOG_EC_FMOD_SYSTEMINIT, NULL);
	FMOD_System_Set3DSettings(audio->system, 1.0f, 100.0f, 2.0f);
	load_sound(audio);
	load_music(audio);
}
