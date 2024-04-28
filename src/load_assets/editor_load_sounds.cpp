/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_load_sounds.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 21:33:44 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"
#include "assets.h"
#include "file_io.h"

static int	parse_sound_list(int fd, t_audio *audio)
{
	char	*sound_path;
	int		ret;
	int		i;

	i = 0;
	sound_path = NULL;
	ret = get_next_line(fd, &sound_path);
	while (ret)
	{
		if (sound_path)
		{
			create_sound(i, sound_path, audio);
			if (audio->samples[i].sound != NULL)
				ft_strncpy_term(audio->samples[i].name, \
							extract_file_name(sound_path), 60);
			doomlog_mul(LOG_NORMAL, (char *[3]){\
					"loaded sound file:", audio->samples[i].name, NULL});
			free(sound_path);
			sound_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &sound_path);
	}
	return (ret);
}

void	editor_load_sounds(t_audio *audio)
{
	int	ret;
	int	fd;

	doomlog(LOG_NORMAL, "LOADING SOUNDS");
	audio->samplecount = count_asset_list(SOUNDLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			SOUNDLISTPATH, "samplecount =", s_itoa(audio->samplecount), NULL});
	audio->samples = prot_memalloc(sizeof(t_audio_sample) * audio->samplecount);
	fd = ft_fileopen(SOUNDLISTPATH, O_RDONLY);
	ret = parse_sound_list(fd, audio);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, SOUNDLISTPATH);
	ft_fileclose(fd, SOUNDLISTPATH);
}
