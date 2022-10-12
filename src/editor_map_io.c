/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_map_io.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:36:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/12 17:42:26 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0666);
	if (fd == -1 && flags != O_RDWR) //TODO: fix, second check is just so loadmap doesn't exit the program
		errors(5);
	return (fd);
}

void	loadmap(t_list **head, char *filename)
{
	int		fd;
	int		br;
	t_line	line;
	t_list	*node;

	fd = fileopen(filename, O_RDWR);
	if (fd <= -1)
		return ;
	br = read(fd, &line, sizeof(t_line));
	while (br >= sizeof(t_line))
	{
		//printf("read %i from file \n", br);
		node = ft_lstnew(&line, sizeof(t_line));
		if (!node)
			errors(4);
		if (*head == NULL)
			*head = node;
		else
			ft_lstapp(head, node);
		br = read(fd, &line, sizeof(t_line));
	}
	if (close(fd) == -1)
		errors(6);
}

//TODO: cant save if map has less walls than opened one.
//Rewrite with lst, instead of editor
void	savemap(t_editor *ed, char *filename)
{
	int		fd;
	t_list	*l;

	fd = fileopen(filename, O_RDWR | O_CREAT);
	if (fd == -1)
		errors(5);
	l = ed->linelist;
	while (l != NULL)
	{
		int written = write(fd, (t_line *)l->content, sizeof(t_line));
		if (written == -1)
			errors(7);
		//printf("wrote %i bytes \n", written);
		l = l->next;
	}
	if (close(fd) == -1)
		errors(6);
}

/*
void	loadmap(t_editor *ed, char *filename)
{
	int		fd;
	int		br;
	t_line	line;
	t_list	*node;

	fd = fileopen(filename, O_RDWR);
	if (fd <= -1)
		return ;
	br = read(fd, &line, sizeof(t_line));
	while (br >= sizeof(t_line))
	{
		//printf("read %i from file \n", br);
		node = ft_lstnew(&line, sizeof(t_line));
		if (ed->linelist == NULL)
			ed->linelist = node;
		else
			ft_lstapp(&ed->linelist, node);
		br = read(fd, &line, sizeof(t_line));
	}
	close(fd);
}
*/