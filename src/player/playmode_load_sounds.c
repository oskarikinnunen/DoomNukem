/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_sounds.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 22:05:13 by raho              #+#    #+#             */
/*   Updated: 2023/03/17 20:08:19 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"
#include <dirent.h>
#include "file_io.h"

static void	unpack_and_load_sound(int sample_i, char *sound_name,
									t_audio *audio)
{
	load_and_write_filecontent(LEVEL0FILE, sound_name, TEMPSOUND);
	create_sound(sample_i, TEMPSOUND, audio);
	if (audio->samples[sample_i].sound != NULL)
		ft_strcpy(audio->samples[sample_i].name, extract_filename(sound_name));
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded sound file:", \
			audio->samples[sample_i].name, NULL});
	remove(TEMPSOUND);
}

static int	parse_sound_list(int fd, t_audio *audio)
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
			unpack_and_load_sound(i, sound_path, audio);
			free(sound_path);
			sound_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &sound_path);
	}
	return (ret);
}

void	playmode_load_sounds(t_audio *audio)
{
	int	fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING SOUNDS");
	load_and_write_filecontent(LEVEL0FILE, SOUNDLISTPATH, TEMPSOUNDLIST);
	audio->samplecount = count_asset_list(TEMPSOUNDLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPSOUNDLIST, "samplecount =", s_itoa(audio->samplecount), NULL});
	audio->samples = ft_memalloc(sizeof(t_audiosample) * audio->samplecount);
	if (audio->samples == NULL)
		doomlog(LOG_EC_MALLOC, "playmode_load_sounds");
	fd = fileopen(TEMPSOUNDLIST, O_RDONLY);
	ret = parse_sound_list(fd, audio);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_sounds");
	fileclose(fd, TEMPSOUNDLIST);
	remove(TEMPSOUNDLIST);
}
