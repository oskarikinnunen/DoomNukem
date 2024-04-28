/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_sounds.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 22:05:13 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"
#include "assets.h"
#include "file_io.h"

static void	unpack_and_load_sound(int sample_i, int level_fd,
									char *sound_name, t_audio *audio)
{
	if (ft_strstr(sound_name, "loop") != NULL)
	{
		load_and_write_filecontent(level_fd, sound_name, TEMPSOUNDLOOP);
		create_sound(sample_i, TEMPSOUNDLOOP, audio);
	}
	else
	{
		load_and_write_filecontent(level_fd, sound_name, TEMPSOUND);
		create_sound(sample_i, TEMPSOUND, audio);
	}
	if (audio->samples[sample_i].sound != NULL)
		ft_strncpy_term(audio->samples[sample_i].name, \
				extract_file_name(sound_name), 60);
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded sound file:", \
			audio->samples[sample_i].name, NULL});
}

static int	parse_sound_list(int level_fd, t_audio *audio)
{
	int		ret;
	int		i;
	char	*sound_path;
	int		temp_fd;

	i = 0;
	sound_path = NULL;
	temp_fd = ft_fileopen(TEMPSOUNDLIST, O_RDONLY);
	ret = get_next_line(temp_fd, &sound_path);
	while (ret)
	{
		if (sound_path)
		{
			unpack_and_load_sound(i, level_fd, sound_path, audio);
			free(sound_path);
			sound_path = NULL;
			i++;
		}
		ret = get_next_line(temp_fd, &sound_path);
	}
	ft_fileclose(temp_fd, TEMPSOUNDLIST);
	return (ret);
}

void	playmode_load_sounds(int level_fd, t_audio *audio)
{
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING SOUNDS");
	load_and_write_filecontent(level_fd, SOUNDLISTPATH, TEMPSOUNDLIST);
	audio->samplecount = count_asset_list(TEMPSOUNDLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPSOUNDLIST, "samplecount =", s_itoa(audio->samplecount), NULL});
	audio->samples = prot_memalloc(sizeof(t_audio_sample) * audio->samplecount);
	ret = parse_sound_list(level_fd, audio);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_sounds");
}
