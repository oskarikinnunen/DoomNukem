/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 19:59:44 by raho              #+#    #+#             */
/*   Updated: 2023/03/22 20:14:20 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "doomnukem.h"

void	clean_create_level_file(char *level)
{
	fileclose(fileopen(level, O_CREAT | O_RDWR | O_TRUNC), level);
	doomlog_mul(LOG_NORMAL, (char *[5]){\
			"cleaned/created the level:", level, "succesfully"});
}

// returns a pointer to the start of the file's name without its path
char	*extract_filename(const char *filepath)
{
	char	*filename;

	filename = ft_strrchr(filepath, '/');
	filename++;
	return (filename);
}

static char	*uint64_to_char(uint64_t	u64)
{
	static char	size[8];

	size[0] = u64 & 0xFF;
	size[1] = u64 >> 8 & 0xFF;
	size[2] = u64 >> 16 & 0xFF;
	size[3] = u64 >> 24 & 0xFF;
	size[4] = u64 >> 32 & 0xFF;
	size[5] = u64 >> 40 & 0xFF;
	size[6] = u64 >> 48 & 0xFF;
	size[7] = u64 >> 56 & 0xFF;
	return (size);
}

void	pack_file_to_level(char *level, char *file)
{
	int				fd;
	void			*temp;
	t_filecontent	fc;

	ft_bzero(&fc, sizeof(t_filecontent));
	fd = fileopen(file, O_RDONLY);
	ft_fileread(fd, &fc);
	ft_strncpy(fc.name, file, 127);
	fileclose(fd, file);
	fileopen(level, O_RDWR | O_APPEND);
	if (write(fd, "FCNK", 4) == -1)
		doomlog(LOG_EC_WRITE, "save_chunk");
	if (write(fd, fc.name, sizeof(char) * 128) == -1)
		doomlog(LOG_EC_WRITE, "save_chunk");
	if (write(fd, uint64_to_char(fc.length), 8) == -1)
		doomlog(LOG_EC_WRITE, "save_chunk");
	if (write(fd, fc.content, sizeof(char) * fc.length) == -1)
		doomlog(LOG_EC_WRITE, "save_chunk");
	if (write(fd, "PADD", CHUNKSIZE - (fc.length % CHUNKSIZE)) == -1)
		doomlog(LOG_EC_WRITE, "save_chunk");
	fileclose(fd, level);
	doomlog_mul(LOG_NORMAL, (char *[5]){\
			"packed file:", file, "to level:", level, NULL});
}