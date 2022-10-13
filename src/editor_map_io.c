/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_map_io.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:36:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 19:06:50 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0666);
	if (fd == -1 && flags != O_RDWR) //TODO: fix, second check is just so loadmap doesn't exit the program
		errors(11);
	return (fd);
}

void	loadmap(t_list **linelist, char *filename)
{
	int		fd;
	int		br;
	t_line	line;
	t_list	*node;

	fd = fileopen(filename, O_RDWR);
	br = read(fd, &line, sizeof(t_line));
	while (br >= sizeof(t_line))
	{
		node = ft_lstnew(&line, sizeof(t_line));
		if (!node)
			errors(5);
		if (*linelist == NULL)
			*linelist = node;
		else
			ft_lstapp(linelist, node);
		br = read(fd, &line, sizeof(t_line));
	}
	if (close(fd) == -1)
		errors(12);
}

//TODO: make general, make param a t_list **
// Iterates through a linked list containing t_lines and writes it's contents into a file
void	savemap(t_editor *ed, char *filename)
{
	int		fd;
	t_list	*l;

	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC);
	l = ed->linelist;
	while (l != NULL)
	{
		int written = write(fd, (t_line *)l->content, sizeof(t_line));
		if (written == -1)
			errors(13);
		//printf("wrote %i bytes \n", written);
		l = l->next;
	}
	if (close(fd) == -1)
		errors(12);
}
