/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_content.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:10:06 by raho              #+#    #+#             */
/*   Updated: 2023/03/22 22:04:23 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "doomnukem.h"

uint64_t	read_len(int fd)
{
	uint64_t	res;
	char		clen[8];
	int			i;

	if (read(fd, clen, 8) == -1)
		doomlog(LOG_EC_READ, "filecontent read_len");
	res = *(uint64_t *)clen;
	return (res);
}

t_filecontent	load_filecontent(char	*level_name, char	*asset_name)
{
	int				fd;
	t_filecontent	fc;
	int				rbytes;
	char			buf[CHUNKSIZE + 1];

	ft_bzero(&fc, sizeof(t_filecontent));
	fd = fileopen(level_name, O_RDONLY);
	rbytes = read(fd, buf, CHUNKSIZE);
	while (rbytes > 0)
	{
		buf[CHUNKSIZE] = '\0';
		if (ft_strncmp(buf, "FCNK", 4) == 0)
			if (read_chunk(fd, &fc, level_name, asset_name) == 0)
				return (fc);
		rbytes = read(fd, buf, CHUNKSIZE);
	}
	if (rbytes == -1)
		doomlog(LOG_EC_READ, level_name);
	fileclose(fd, level_name);
	return (fc);
}

int	ft_fileread(int fd, t_filecontent *f)
{
	char	*temp;
	void	*temp2;
	char	buf[32000];
	size_t	len;

	len = read(fd, buf, sizeof(buf));
	while (len)
	{
		temp = ft_memdup(buf, len); // TODO: PROTECT
		temp2 = ft_memjoin(f->content, (const void *)temp, f->length, len);
		if (f->content)
			free(f->content);
		f->content = temp2;
		f->length += len;
		len = read(fd, buf, sizeof(buf));
	}
	if (len == -1)
		doomlog(LOG_EC_READ, "ft_fileread");
	return (f->length);
}

void	load_and_write_filecontent(char *worldname, char *fcname, \
									char *filename)
{
	t_filecontent	fc;
	int				fd;

	fc = load_filecontent(worldname, fcname);
	fd = fileopen(filename, O_CREAT | O_RDWR | O_TRUNC);
	if (write(fd, fc.content, fc.length) == -1)
		doomlog(LOG_EC_WRITE, "load_and_write_filecontent");
	fileclose(fd, filename);
	free(fc.content);
}
