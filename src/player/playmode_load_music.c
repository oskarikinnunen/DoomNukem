/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_music.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 22:05:25 by raho              #+#    #+#             */
/*   Updated: 2023/03/22 14:19:40 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"
#include "file_io.h"

static void	unpack_and_load_music(int music_i, char *level_path,
									char *music_name, t_audio *audio)
{
	load_and_write_filecontent(level_path, music_name, TEMPMUSIC);
	create_music(music_i, TEMPMUSIC, audio);
	if (audio->music[music_i].sound != NULL)
		ft_strcpy(audio->music[music_i].name, extract_filename(music_name));
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded music file:", \
			audio->music[music_i].name, NULL});
}

static int	parse_music_list(int fd, char *level_path, t_audio *audio)
{
	int		ret;
	int		i;
	char	*music_path;

	i = 0;
	music_path = NULL;
	ret = get_next_line(fd, &music_path);
	while (ret)
	{
		if (music_path)
		{
			unpack_and_load_music(i, level_path, music_path, audio);
			free(music_path);
			music_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &music_path);
	}
	return (ret);
}

void	playmode_load_music(char *level_path, t_audio *audio)
{
	int	fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING MUSIC");
	load_and_write_filecontent(level_path, MUSICLISTPATH, TEMPMUSICLIST);
	audio->music_count = count_asset_list(TEMPMUSICLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPMUSICLIST, "music_count =", s_itoa(audio->music_count), NULL});
	audio->music = prot_memalloc(sizeof(t_audiosample) * audio->music_count);
	fd = fileopen(TEMPMUSICLIST, O_RDONLY);
	ret = parse_music_list(fd, level_path, audio);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_music");
	fileclose(fd, TEMPMUSICLIST);
}
