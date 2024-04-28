/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_chunks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:36:29 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:05:55 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
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

void	save_chunk(char *filename, char *chunk_name, t_list *content)
{
	t_list	*l;
	int		written;
	int		fd;
	int		temp;

	l = content;
	fd = ft_fileopen(filename, O_RDWR | O_APPEND);
	written = 0;
	temp = 0;
	if (write(fd, chunk_name, CHUNKSIZE) == -1)
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
	ft_fileclose(fd, filename);
}

t_list	*load_chunk(char *filename, char *chunk_name, size_t size)
{
	int		fd;
	int		br;
	char	buf[CHUNKSIZE + 1];
	t_list	*result;
	int		i;

	fd = ft_fileopen(filename, O_RDONLY);
	ft_bzero(buf, CHUNKSIZE + 1);
	br = read(fd, buf, CHUNKSIZE);
	while (br > 0)
	{
		buf[CHUNKSIZE] = '\0';
		if (ft_strcmp(chunk_name, buf) == 0)
		{
			result = parse_chunk(fd, size);
			ft_fileclose(fd, filename);
			return (result);
		}
		br = read(fd, buf, CHUNKSIZE);
	}
	if (br == -1)
		doomlog(LOG_EC_READ, filename);
	ft_fileclose(fd, filename);
	return (NULL);
}

int	read_chunk(int fd, t_file_content *fc, char *asset_name)
{
	uint64_t	curr_len;

	if (read(fd, fc->name, 128) == -1)
		doomlog(LOG_EC_READ, "read_chunk");
	curr_len = 0;
	if (ft_strcmp(fc->name, asset_name) == 0)
	{
		fc->length = read_len(fd);
		fc->content = prot_memalloc(fc->length);
		if (read(fd, fc->content, fc->length) == -1)
			doomlog(LOG_EC_READ, "read_chunk");
		if (lseek(fd, (CHUNKSIZE - (fc->length % CHUNKSIZE)), \
				SEEK_CUR) == -1)
			doomlog(LOG_EC_LSEEK, "read_chunk");
		return (0);
	}
	else
	{
		curr_len = read_len(fd);
		if (lseek(fd, curr_len + (CHUNKSIZE - (curr_len % CHUNKSIZE)), \
				SEEK_CUR) == -1)
			doomlog(LOG_EC_LSEEK, "read_chunk");
	}
	return (1);
}
