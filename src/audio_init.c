/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 10:42:33 by raho              #+#    #+#             */
/*   Updated: 2023/01/24 11:25:21 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

#include <dirent.h>

bool is_sample(char	*audioname)
{
	return (ft_strstr(audioname, ".wav") != NULL
		|| ft_strstr(audioname, ".mp3") != NULL);
}

bool is_music(char *audioname)
{
	return (ft_strstr(audioname, "music") != NULL);
}

static void allocate_sample_count(t_audio *audio)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/audio";
	int				s_i;
	int				m_i;

	d = opendir(path);
	s_i = 0;
	m_i	= 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG
				&& is_sample(dfile->d_name))
			{
				if (is_music(dfile->d_name))
					m_i++;
				else
					s_i++;
			}
			dfile = readdir(d);
		}
		closedir(d);
	}
	audio->samples = ft_memalloc(s_i * sizeof(t_audiosample));
	audio->music = ft_memalloc(m_i * sizeof(t_audiosample));
	audio->samplecount = s_i;
	audio->music_count = m_i;
	//printf("alloc %i samples \n", s_i);
	//printf("alloc %i samples \n", s_i);
}

static void load_samples(t_audio *audio)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/audio";
	char fullpath	[512];
	int				s_i;
	int				m_i;

	allocate_sample_count(audio);
	d = opendir(path);
	s_i = 0;
	m_i = 0;
	
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG
				&& is_sample(dfile->d_name))
			{
				ft_strcpy(fullpath, path);
				ft_strcat(fullpath, "/");
				ft_strcat(fullpath, dfile->d_name);
				// OLD LOGGING:
				printf("loading audio sample %s \n", fullpath);
				// NEW LOGGING:
				log_addstrings((char **) {"loading audio sample ", fullpath, NULL});
				int	mask = FMOD_3D | FMOD_LOOP_OFF;
				if (!is_music(dfile->d_name))
				{
					bool looped = ft_strstr(dfile->d_name, "loop") != NULL;
					if (looped)
					{
						mask = FMOD_3D | FMOD_LOOP_NORMAL;
						printf("loading %s as looped 3d audio \n", dfile->d_name);
					}
						
					if (FMOD_System_CreateSound(audio->system, fullpath, mask, NULL, &audio->samples[s_i].sound) != FMOD_OK)
					{
						error_log(EC_FMOD_SYSTEMCREATE);
					}
					ft_strcpy(audio->samples[s_i].name, dfile->d_name); //TODO: protect, strncpy
					s_i++;
				}
				else
				{
					mask = FMOD_2D | FMOD_LOOP_NORMAL;
					if (FMOD_System_CreateSound(audio->system, fullpath, mask, NULL, &audio->music[m_i].sound) != FMOD_OK)
					{
						error_log(EC_FMOD_SYSTEMCREATE);
					}
					ft_strcpy(audio->music[m_i].name, dfile->d_name); //TODO: protect, strncpy
					m_i++;
				}
			}
			dfile = readdir(d);
		}
		closedir(d);
	}
	//audio->samplecount = i;
	printf("loaded %i samples \n", audio->samplecount);
	printf("loaded %i music \n", audio->music_count);
}

void	load_audio(t_audio *audio)
{
	ft_bzero(audio, sizeof(t_audio));
	audio->samplecount = 0;
	audio->system = NULL; //todo: use the bzero but apply volume settings from sdl somehow
	if (FMOD_System_Create(&audio->system, FMOD_VERSION) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMCREATE);
	if (FMOD_System_Init(audio->system, 30, FMOD_INIT_NORMAL, FMOD_OUTPUTTYPE_AUTODETECT) != FMOD_OK)
		error_log(EC_FMOD_SYSTEMINIT);
	FMOD_System_Set3DSettings(audio->system, 1.0f, 100.0f, 2.0f);
	load_samples(audio);
}
