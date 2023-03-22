/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_sounds.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 22:05:13 by raho              #+#    #+#             */
/*   Updated: 2023/03/22 14:19:05 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"
#include <dirent.h>
#include "file_io.h"

static void	unpack_and_load_sound(int sample_i, char *level_path,
									char *sound_name, t_audio *audio)
{
	if (ft_strstr(sound_name, "loop") != NULL)
	{
		load_and_write_filecontent(level_path, sound_name, TEMPSOUNDLOOP);
		create_sound(sample_i, TEMPSOUNDLOOP, audio);
	}
	else
	{
		load_and_write_filecontent(level_path, sound_name, TEMPSOUND);
		create_sound(sample_i, TEMPSOUND, audio);
	}
	if (audio->samples[sample_i].sound != NULL)
		ft_strcpy(audio->samples[sample_i].name, extract_filename(sound_name));
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded sound file:", \
			audio->samples[sample_i].name, NULL});
}

static int	parse_sound_list(int fd, char *level_path, t_audio *audio)
{
	int		ret;
	int		i;
	char	*sound_path;

	i = 0;
	sound_path = NULL;
	ret = get_next_line(fd, &sound_path);
	while (ret)
	{
		if (sound_path)
		{
			unpack_and_load_sound(i, level_path, sound_path, audio);
			free(sound_path);
			sound_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &sound_path);
	}
	return (ret);
}

void	playmode_load_sounds(char *level_path, t_audio *audio)
{
	int	fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING SOUNDS");
	load_and_write_filecontent(level_path, SOUNDLISTPATH, TEMPSOUNDLIST);
	audio->samplecount = count_asset_list(TEMPSOUNDLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPSOUNDLIST, "samplecount =", s_itoa(audio->samplecount), NULL});
	audio->samples = prot_memalloc(sizeof(t_audiosample) * audio->samplecount);
	fd = fileopen(TEMPSOUNDLIST, O_RDONLY);
	ret = parse_sound_list(fd, level_path, audio);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_sounds");
	fileclose(fd, TEMPSOUNDLIST);
}
