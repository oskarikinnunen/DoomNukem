/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_map_io.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:36:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/19 17:26:31 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0666);
	if (fd == -1)
		error_log(EC_WRITE);
	return (fd);
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

	linelist = NULL;
	fd = open(filename, O_RDONLY, 0666); //TODO: check if you can load a directory
	if (fd == -1)
		exit(0);
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
