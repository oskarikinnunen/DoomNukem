/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_music.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 22:05:25 by raho              #+#    #+#             */
/*   Updated: 2023/03/17 15:09:43 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"
#include <dirent.h>
#include "file_io.h"

static void	unpack_and_save_music(int music_i, char *music_name, t_audio *audio)
{
	load_and_write_filecontent(LEVEL0FILE, music_name, TEMPMUSIC);
	create_music(music_i, TEMPMUSIC, audio);
	if (audio->music[music_i].sound != NULL)
		ft_strcpy(audio->music[music_i].name, extract_filename(music_name));
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and saved music file:", \
			audio->music[music_i].name, NULL});
	remove(TEMPMUSIC);
}

static int	parse_music_list(int fd, t_audio *audio)
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
			unpack_and_save_music(i, music_path, audio);
			free(music_path);
			music_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &music_path);
	}
	return (ret);
}

void	playmode_load_music(t_audio *audio)
{
	int	fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING MUSIC");
	load_and_write_filecontent(LEVEL0FILE, MUSICLISTPATH, TEMPMUSICLIST);
	audio->music_count = count_asset_list(TEMPMUSICLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPMUSICLIST, "music_count =", s_itoa(audio->music_count), NULL});
	audio->music = ft_memalloc(sizeof(t_audiosample) * audio->music_count);
	if (audio->music == NULL)
		doomlog(LOG_EC_MALLOC, "playmode_load_music");
	fd = fileopen(TEMPMUSICLIST, O_RDONLY);
	ret = parse_music_list(fd, audio);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_music");
	fileclose(fd, TEMPMUSICLIST);
	remove(TEMPMUSICLIST);
}
