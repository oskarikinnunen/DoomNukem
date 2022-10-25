/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_map_io.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:36:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/25 17:26:54 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "filechunks.h"

static int	fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0666);
	if (fd == -1)
		error_log(EC_WRITE);
	return (fd);
}

typedef struct s_chunkloader
{
	char	chunkname[CHUNKSIZE + 1];
	t_list	*(*parse)(int);
}	t_chunkloader;

//tatic t_chunkloader *

int		find_chunk_count(int fd)
{
	char	buf[CHUNKSIZE + 1];
	int		br;
	int		count;

	br = read(fd, buf, CHUNKSIZE);

	count = 0;
	while (br > 0)
	{
		printf("chunk count read str %s \n", buf);
		if (ft_strcmp(buf, "CEND") == 0)
			break;
		count++;
		br = read(fd, buf, CHUNKSIZE);
	}
	lseek(fd, -(count + 1) * CHUNKSIZE, SEEK_CUR);
	return (count);
}

void	save_entitylist(t_list *entitylist, char *filename)
{
	t_list		*l;
	t_entity	ent;
	int			written;
	int			fd;

	l = entitylist;
	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC);
	printf("size should ? %lu  \n", sizeof(t_entityID) + sizeof(t_vector2) + sizeof(char));
	written = 0;
	while (l != NULL)
	{
		ent = *(t_entity *)l->content;
		written += write(fd, &ent, sizeof(t_entity));
		l = l->next;
	}
	printf("wrote %i to file \n", written);
	if (written % CHUNKSIZE != 0) //No need for padding since struct size is always multiple of 4? TODO: research
	{
		printf("pad size = %i \n", CHUNKSIZE - (written % CHUNKSIZE));
		write(fd, (int [4]){-42, -42, -42, -42}, CHUNKSIZE - (written % CHUNKSIZE));
	}
		
}

t_list	*parse_entity(int fd)
{
	t_entity	ent;
	char		buf[CHUNKSIZE];
	int			br;
	int			i;

	int count = find_chunk_count(fd);
	count = (count * CHUNKSIZE) / sizeof(t_entity);
	br = read(fd, buf, CHUNKSIZE);
	i = 0;
	while (i < count)
	{
		ent = *(t_entity *)buf;
		printf("entity uid %i pos %f %f\n", ent.id, ent.position.x, ent.position.y);
		i++;
		br = read(fd, buf, CHUNKSIZE);
	}
	//while
	return (NULL);
}

void	mapload_experimental()
{
	int		fd;
	int		br;
	char	buf[CHUNKSIZE + 1];
	int		i;
	static	t_chunkloader cls[2] =
	{
		{"ENT_", parse_entity},
		{"WALX", parse_entity}
	};

	fd = open("mapfile_e1", O_RDONLY, 0666);
	if (fd == -1)
		error_log(EC_OPEN);
	ft_bzero(buf, CHUNKSIZE + 1);
	br = read(fd, buf, CHUNKSIZE);
	while (br > 0)
	{
		i = 0;
		while (i < 2)
		{
			if (ft_strcmp(cls[i].chunkname, buf) == 0) 
			{
				printf("found chunk %s \n", buf);
				cls[i].parse(fd);
			}
			i++;
		}
		br = read(fd, buf, CHUNKSIZE);
	}
}

//loadmap doesn't care if open fails, we just allocate the head of the linelist in the case the file doesn't exist
//TODO: when the file doesn't exist, it's putting one line to 0.0 when checking in 3d mode
t_list	*loadmap(char *filename)
{
	int		fd;
	int		br;
	t_line	line;
	t_list	*linelist;
	t_list	*node;

	mapload_experimental();
	linelist = NULL;
	fd = open(filename, O_RDONLY, 0666); //TODO: check if you can load a directory
	if (fd == -1)
	{
		printf("map reader is shit, exiting the whole program! Pls supply an empty map file\n");
		exit(0);
	}
	br = read(fd, &line, sizeof(t_line));
	while (br >= sizeof(t_line))
	{
		node = ft_lstnew(&line, sizeof(t_line));
		if (!node)
			error_log(EC_MALLOC);
		if (linelist == NULL)
			linelist = node;
		else
			ft_lstapp(&linelist, node);
		br = read(fd, &line, sizeof(t_line));
	}
	close(fd);
	return (linelist);
}

//TODO: make general, make param a t_list **
// Iterates through a linked list containing t_lines and writes it's contents into a file
void	savemap(t_editor *ed, char *filename)
{
	int		fd;
	int		written;
	t_list	*l;

	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC);
	l = ed->linelist;
	int i = 0;
	while (l != NULL)
	{
		write(fd, (t_line *)l->content, sizeof(t_line));
		l = l->next;
		i++;
	}
	if (close(fd) == -1)
		error_log(EC_CLOSE);
}
