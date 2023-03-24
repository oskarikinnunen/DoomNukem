/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_content.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:10:06 by raho              #+#    #+#             */
/*   Updated: 2023/03/23 22:03:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "doomnukem.h"

uint64_t	read_len(int fd)
{
	uint64_t	res;
	char		clen[8];
	int			i;

	ft_bzero(clen, sizeof(clen));
	res = 0;
	if (read(fd, clen, 8) == -1)
		doomlog(LOG_EC_READ, "filecontent read_len");
	res = *(uint64_t *)clen;
	return (res);
}

static void	restart_fd(int fd, int counter)
{
	if (counter == 2)
		doomlog(LOG_FATAL, \
				"expected level data not found from the level file");
	if (lseek(fd, 0, SEEK_SET) == -1)
		doomlog(LOG_EC_LSEEK, "load_filecontent");
}

t_filecontent	load_filecontent(int fd, char *asset_name)
{
	t_filecontent	fc;
	int				rbytes;
	char			buf[CHUNKSIZE + 1];
	int				counter;

	counter = 0;
	ft_bzero(&fc, sizeof(t_filecontent));
	rbytes = read(fd, buf, CHUNKSIZE);
	while (rbytes >= 0)
	{
		buf[CHUNKSIZE] = '\0';
		if (ft_strncmp(buf, "FCNK", 4) == 0)
			if (read_chunk(fd, &fc, asset_name) == 0)
				return (fc);
		if (rbytes == 0)
		{
			restart_fd(fd, counter);
			counter++;
		}
		rbytes = read(fd, buf, CHUNKSIZE);
	}
	if (rbytes == -1)
		doomlog(LOG_EC_READ, "load_filecontent");
	fileclose(fd, "load_filecontent");
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
		temp = ft_memdup(buf, len);
		if (temp == NULL)
			doomlog(LOG_EC_MALLOC, "ft_fileread");
		temp2 = ft_memjoin(f->content, (const void *)temp, f->length, len);
		if (temp2 == NULL)
			doomlog(LOG_EC_MALLOC, "ft_fileread");
		free(temp);
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

void	load_and_write_filecontent(int fd,
									char *fc_name, char *file_name)
{
	t_filecontent	fc;
	int				temp_fd;

	fc = load_filecontent(fd, fc_name);
	temp_fd = fileopen(file_name, O_CREAT | O_RDWR | O_TRUNC);
	if (write(temp_fd, fc.content, fc.length) == -1)
		doomlog(LOG_EC_WRITE, "load_and_write_filecontent");
	fileclose(temp_fd, file_name);
	free(fc.content);
}
