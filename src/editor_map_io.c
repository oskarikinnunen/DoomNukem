/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_map_io.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:36:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/15 13:12:34 by okinnune         ###   ########.fr       */
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
void	loadmap(t_list **linelist, char *filename)
{
	int		fd;
	int		br;
	t_line	line;
	t_list	*node;

	br = 0;
	fd = open(filename, O_RDWR, 0666);
	*linelist = ft_lstnew(&(t_line){0, 0, 0, 0}, sizeof(t_line));
	if (!*linelist)
		error_log(EC_MALLOC);
	if (fd != -1)
		br = read(fd, &line, sizeof(t_line));
	while (br >= sizeof(t_line))
	{
		node = ft_lstnew(&line, sizeof(t_line));
		if (!node)
			error_log(EC_MALLOC);
		ft_lstapp(linelist, node);
		br = read(fd, &line, sizeof(t_line));
	}
	close(fd);
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
	while (l != NULL)
	{
		write(fd, (t_line *)l->content, sizeof(t_line));
		//printf("wrote %i bytes \n", written);
		l = l->next;
	}
	if (close(fd) == -1)
		error_log(EC_CLOSE);
}
