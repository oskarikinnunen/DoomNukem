/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_map_io.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:36:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/03 19:37:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "filechunks.h"
#include "editor_tools.h"

static int	fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0666);
	if (fd == -1)
		error_log(EC_WRITE);
	return (fd);
}

int		find_chunk_count(int fd)
{
	char	buf[CHUNKSIZE + 1];
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
		error_log(EC_MALLOC);
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
		return (NULL);
	ft_bzero(buf, CHUNKSIZE + 1);
	br = read(fd, buf, CHUNKSIZE);
	while (br > 0)
	{
		if (ft_strcmp(chunkname, buf) == 0)
		{
			//printf("found chunk %s \n", buf); //TODO: don't remove, going to be used for logging
			result = parse_chunk(fd, size);
			return (result);
		}
		br = read(fd, buf, CHUNKSIZE);
	}
	return (NULL);
}

void	save_editordata(t_editor *ed)
{
	int	fd;
	fd = fileopen("buttons", O_RDWR | O_CREAT | O_TRUNC); //Empty the file or create a new one if it doesn't exist
	close(fd);
	save_chunk("buttons", "BUTN", ed->buttonlist);
}
