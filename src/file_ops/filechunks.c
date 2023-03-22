/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filechunks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:36:29 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 16:57:20 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "filechunks.h"
#include "file_io.h"
#include "editor_tools.h"

int	find_chunk_count(int fd)
{
	char	buf[CHUNKSIZE + 1];
	int		br;
	int		count;

	buf[CHUNKSIZE] = '\0';
	br = read(fd, buf, CHUNKSIZE);
	count = 0;
	while (br > 0)
	{
		if (ft_strcmp(buf, "CEND") == 0)
			break ;
		count++;
		br = read(fd, buf, CHUNKSIZE);
	}
	if (br == -1)
		doomlog(LOG_EC_READ, "find_chunk_count");
	if (lseek(fd, -(count + 1) * CHUNKSIZE, SEEK_CUR) == -1)
		doomlog(LOG_EC_LSEEK, "find_chunk_count");
	return (count);
}

t_list	*parse_chunk(int fd, size_t size)
{
	char	*buf;
	int		br;
	t_list	*head;
	t_list	*node;
	int		count;

	buf = prot_memalloc(size);
	count = find_chunk_count(fd);
	count = (count * CHUNKSIZE) / size;
	head = NULL;
	br = read(fd, buf, size);
	while (count > 0)
	{
		list_push(&head, buf, size);
		count--;
		br = read(fd, buf, size);
	}
	if (br == -1)
		doomlog(LOG_EC_READ, "parse_chunk");
	free(buf);
	return (head);
}

//TODO: No need for padding because compiler does padding?
void	save_chunk(char *filename, char *chunkname, t_list *content)
{
	t_list	*l;
	int		written;
	int		fd;
	int		temp;

	l = content;
	fd = fileopen(filename, O_RDWR | O_APPEND);
	written = 0;
	temp = 0;
	if (write(fd, chunkname, CHUNKSIZE) == -1)
		doomlog(LOG_EC_WRITE, "save_chunk");
	while (l != NULL)
	{
		temp = write(fd, l->content, l->content_size);
		if (temp == -1)
			doomlog(LOG_EC_WRITE, "save_chunk");
		written += temp;
		l = l->next;
	}
	if (written % CHUNKSIZE != 0)
		if (write(fd, "PADD", CHUNKSIZE - (written % CHUNKSIZE) == -1))
			doomlog(LOG_EC_WRITE, "save_chunk");
	if (write(fd, "CEND", CHUNKSIZE) == -1)
		doomlog(LOG_EC_WRITE, "save_chunk");
	fileclose(fd, filename);
}

t_list	*load_chunk(char *filename, char *chunkname, size_t size)
{
	int		fd;
	int		br;
	char	buf[CHUNKSIZE + 1];
	t_list	*result;
	int		i;

	fd = fileopen(filename, O_RDONLY);
	ft_bzero(buf, CHUNKSIZE + 1);
	br = read(fd, buf, CHUNKSIZE);
	while (br > 0)
	{
		buf[CHUNKSIZE] = '\0';
		if (ft_strcmp(chunkname, buf) == 0)
		{
			result = parse_chunk(fd, size);
			fileclose(fd, filename);
			return (result);
		}
		br = read(fd, buf, CHUNKSIZE);
	}
	if (br == -1)
		doomlog(LOG_EC_READ, filename);
	fileclose(fd, filename);
	return (NULL);
}

int		ft_fileread(int fd, t_filecontent *f)
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

// TODO: OPTIMIZE
t_filecontent	load_filecontent(char	*worldname, char	*fc_name)
{
	int				fd;
	t_filecontent	fc;
	int				rbytes;
	char			buf[CHUNKSIZE + 1];

	ft_bzero(&fc, sizeof(t_filecontent));
	fd = fileopen(worldname, O_RDONLY);
	rbytes = read(fd, buf, CHUNKSIZE);
	while (rbytes > 0)
	{
		buf[CHUNKSIZE] = '\0';
		if (ft_strncmp(buf, "FCNK", 4) == 0)
		{
			if (read(fd, fc.name, 128) == -1)
				doomlog(LOG_EC_READ, worldname);
			if (ft_strcmp(fc.name, fc_name) == 0)
			{
				fc.length = read_len(fd);
				fc.content = prot_memalloc(fc.length);
				if (read(fd, fc.content, fc.length) == -1)
					doomlog(LOG_EC_READ, worldname);
				return (fc);
			}
			else
			{
				uint64_t	curr_len;
				curr_len = read_len(fd);
				// printf("tried to find: %s but found: %s\n", fc_name, fc.name);
				// printf("found unwanted filechunk: %s | len: %llu\n", fc.name, curr_len);
				if (lseek(fd, curr_len + (CHUNKSIZE - (curr_len % CHUNKSIZE)), SEEK_CUR) == -1)
					doomlog(LOG_EC_LSEEK, worldname);
			}
		}
		rbytes = read(fd, buf, CHUNKSIZE);
	}
	if (rbytes == -1)
		doomlog(LOG_EC_READ, worldname);
	fileclose(fd, worldname);
	return (fc);
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
