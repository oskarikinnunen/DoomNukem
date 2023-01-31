/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_map_io.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:36:29 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/31 15:50:24 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "filechunks.h"
#include "file_io.h"
#include "editor_tools.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

int		find_chunk_count(int fd)
{
	char	buf[CHUNKSIZE + 1] = { };
	int		br;
	int		count;

	br = read(fd, buf, CHUNKSIZE);
	count = 0;
	while (br > 0)
	{
		if (ft_strcmp(buf, "CEND") == 0)
			break;
		count++;
		br = read(fd, buf, CHUNKSIZE);
	}
	lseek(fd, -(count + 1) * CHUNKSIZE, SEEK_CUR);
	return (count);
}

t_list	*parse_chunk(int fd, size_t size)
{
	char		*buf;
	int			br;
	t_list		*head;
	t_list		*node;
	int			count;

	buf = ft_memalloc(size);
	if (buf == NULL)
		doomlog(LOGEC_MALLOC, NULL);
	count = find_chunk_count(fd);
	count = (count * CHUNKSIZE) / size;
	// printf("found %i chunks \n", count); //TODO: don't remove, going to be used when logging is implemented in the near future
	head = NULL;
	br = read(fd, buf, size);
	while (count > 0)
	{
		list_push(&head, buf, size);
		count--;
		br = read(fd, buf, size);
	}
	free(buf);
	return (head);
}

void	save_chunk(char *filename, char *chunkname, t_list *content)
{
	t_list		*l;
	int			written;
	int			fd;

	l = content;
	fd = fileopen(filename, O_RDWR | O_APPEND);
	written = 0;
	write(fd, chunkname, CHUNKSIZE);
	while (l != NULL)
	{
		written += write(fd, l->content, l->content_size);
		l = l->next;
	}
	//printf("wrote %i to file \n", written); //TODO: don't remove, going to be used for logging
	if (written % CHUNKSIZE != 0) //No need for padding since struct size 'should be' always multiple of 4? (Compiler does padding for structs) TODO: research more, will this happen when compiling for other platforms?
	{
		//printf("pad size = %i \n", CHUNKSIZE - (written % CHUNKSIZE)); //TODO: don't remove, going to be used for logging
		write(fd, "PADD", CHUNKSIZE - (written % CHUNKSIZE));
	}
	write(fd, "CEND", CHUNKSIZE);
}

t_list *load_chunk(char *filename, char *chunkname, size_t size)
{
	int		fd;
	int		br;
	char	buf[CHUNKSIZE + 1];
	t_list	*result;
	int		i;

	fd = open(filename, O_RDONLY, 0666);
	if (fd == -1)
	{
		//printf("couldn't open file %s for chunk reading \n", filename);
		return (NULL);
	}
	else
	{
		//printf("opened file %s for chunk %s reading \n", filename, chunkname);
	}
	ft_bzero(buf, CHUNKSIZE + 1);
	br = read(fd, buf, CHUNKSIZE);
	while (br > 0)
	{
		if (ft_strcmp(chunkname, buf) == 0)
		{
			//printf("found chunk %s \n", chunkname);
			result = parse_chunk(fd, size);
			close(fd);
			return (result);
		}
		br = read(fd, buf, CHUNKSIZE);
	}
	close(fd);
	return (NULL);
}

int		ft_fileread(int fd, t_filecontent *f)
{
	char buf[300000]; //TODO: make fancier version

	f->length = read(fd, buf, 300000);
	f->content = ft_memdup(buf, f->length);
	return (f->length);
}

int		ft_nfileread(int fd, t_filecontent *f, size_t size)
{
	char buf[300000];

	f->length = read(fd, buf, size);
	f->content = ft_memdup(buf, size);
	return (f->length);
}

uint64_t	read_len(int fd)
{
	uint64_t	res;
	char		clen[8];
	int			i;

	read(fd, clen, 8);
	res = *(uint64_t *)clen;
	return (res);
}

t_filecontent	load_filecontent(char	*worldname, char	*fc_name)
{
	int				fd;
	t_filecontent	fc;
	int				rbytes;
	char			buf[CHUNKSIZE + 1];

	ft_bzero(&fc, sizeof(t_filecontent));
	fd = open(worldname, O_RDONLY);
	if (fd == -1)
		return (fc);
	rbytes = read(fd, buf, CHUNKSIZE);
	while (rbytes > 0)
	{
		if (ft_strncmp(buf, "FCNK", 4) == 0)
		{
			read(fd, fc.name, 128);
			if (ft_strcmp(fc.name, fc_name) == 0)
			{
				fc.length = read_len(fd);
				fc.content = malloc(fc.length);
				read(fd, fc.content, fc.length);
				return (fc);
			}
		}
		rbytes = read(fd, buf, CHUNKSIZE);
	}
}

int load_filecontent_fd(char	*worldname, char *fcname)
{
	t_filecontent	fc;
	int				fd;

	fc = load_filecontent(worldname, fcname);
	fd = open(fcname, O_CREAT | O_RDWR | O_APPEND, 0666);
	write(fd, fc.content, fc.length);
	return (fd);
}

char	*uint64_to_char(uint64_t	u64)
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

void	pack_file(char	*packname, char *filename)
{
	int				fd;
	void			*temp;
	t_filecontent	fc;
	
	fd = open(filename, O_RDONLY, 0666);
	if (fd == -1)
		return ;
	ft_fileread(fd, &fc);
	ft_strcpy(fc.name, filename);
	close(fd);
	printf("file read '%s' \n", (char *)fc.content);
	fd = open(packname, O_RDWR | O_APPEND, 0666);
	if (fd == -1)
		return ;
	size_t total = 0;
	total += write(fd, "FCNK", 4);
	total += write(fd, fc.name, sizeof(char) * 128);	
	total += write(fd, uint64_to_char(fc.length), 8);
	total += write(fd, fc.content, sizeof(char) * fc.length);
	//TODO: ensure that this fits the 4 byte alignment of the chunk reader
	//total += write(fd, "PADD", fc.length % 4);
	//printf("wrote packed file content len %lu \n", total); 
	close(fd);
}

void	force_pack_file(char	*packname, char *filename)
{
	int				fd;
	void			*temp;
	t_filecontent	fc;
	
	fd = open(filename, O_RDONLY, 0666);
	if (fd == -1)
		return ;
	ft_fileread(fd, &fc);
	ft_strcpy(fc.name, filename);
	close(fd);
	printf("file read '%s' \n", (char *)fc.content);
	fd = open(packname, O_RDWR | O_APPEND, 0666);
	if (fd == -1)
		fd = open(packname, O_CREAT | O_RDWR | O_APPEND, 0666); //TODO: protect after this
	write(fd, "FCNK", 4);
	write(fd, fc.name, sizeof(char) * 128);
	write(fd, uint64_to_char(fc.length), 8);
	write(fd, fc.content, sizeof(char) * fc.length);
	write(fd, "PADD", fc.length % 4);
	close(fd);
}

#pragma GCC diagnostic pop