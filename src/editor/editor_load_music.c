/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_load_music.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 21:51:13 by raho              #+#    #+#             */
/*   Updated: 2023/03/16 22:22:23 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"
#include <dirent.h>
#include "file_io.h"

static int	parse_music_list(int fd, t_audio *audio)
{
	char	*music_path;
	int		ret;
	int		i;

	i = 0;
	music_path = NULL;
	ret = get_next_line(fd, &music_path);
	while (ret)
	{
		if (music_path)
		{
			create_music(i, music_path, audio);
			if (audio->music[i].sound != NULL)
				ft_strcpy(audio->music[i].name, extract_filename(music_path));
			doomlog_mul(LOG_NORMAL, (char *[3]){\
					"loaded music file:", audio->music[i].name, NULL});
			free(music_path);
			music_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &music_path);
	}
	return (ret);
}

void	editor_load_music(t_audio *audio)
{
	int	ret;
	int	fd;

	doomlog(LOG_NORMAL, "LOADING MUSIC");
	audio->music_count = count_asset_list(MUSICLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			MUSICLISTPATH, "music_count =", s_itoa(audio->music_count), NULL});
	audio->music = ft_memalloc(sizeof(t_audiosample) * audio->music_count);
	if (audio->music == NULL)
		doomlog(LOG_EC_MALLOC, "editor_load_music");
	fd = fileopen(MUSICLISTPATH, O_RDONLY);
	ret = parse_music_list(fd, audio);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, MUSICLISTPATH);
	fileclose(fd, MUSICLISTPATH);
}
