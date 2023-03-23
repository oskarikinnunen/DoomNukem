/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_music.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 22:05:25 by raho              #+#    #+#             */
/*   Updated: 2023/03/23 19:09:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"
#include "file_io.h"

static void	unpack_and_load_music(int music_i, int level_fd,
									char *music_name, t_audio *audio)
{
	load_and_write_filecontent(level_fd, music_name, TEMPMUSIC);
	create_music(music_i, TEMPMUSIC, audio);
	if (audio->music[music_i].sound != NULL)
		ft_strcpy(audio->music[music_i].name, extract_filename(music_name));
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded music file:", \
			audio->music[music_i].name, NULL});
}

static int	parse_music_list(int level_fd, t_audio *audio)
{
	int		ret;
	int		i;
	char	*music_path;
	int		temp_fd;

	i = 0;
	music_path = NULL;
	temp_fd = fileopen(TEMPMUSICLIST, O_RDONLY);
	ret = get_next_line(temp_fd, &music_path);
	while (ret)
	{
		if (music_path)
		{
			unpack_and_load_music(i, level_fd, music_path, audio);
			free(music_path);
			music_path = NULL;
			i++;
		}
		ret = get_next_line(temp_fd, &music_path);
	}
	fileclose(temp_fd, TEMPMUSICLIST);
	return (ret);
}

void	playmode_load_music(int level_fd, t_audio *audio)
{
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING MUSIC");
	load_and_write_filecontent(level_fd, MUSICLISTPATH, TEMPMUSICLIST);
	audio->music_count = count_asset_list(TEMPMUSICLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPMUSICLIST, "music_count =", s_itoa(audio->music_count), NULL});
	audio->music = prot_memalloc(sizeof(t_audiosample) * audio->music_count);
	ret = parse_music_list(level_fd, audio);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_music");
}
