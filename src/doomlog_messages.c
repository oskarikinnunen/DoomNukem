/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomlog_messages.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/02/07 15:54:56 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	normal_message_mul(int fd, char **str)
{
	int	index;

	index = 0;
	while (str[index] != NULL)
	{
		ft_putstr_fd(str[index], fd);
		if (str[index + 1] == NULL)
			ft_putchar_fd('\n', fd);
		else
			ft_putchar_fd(' ', fd);
		index++;
	}
}

void	warning_message(int fd, char *str)
{
	ft_putstr_fd("! ", fd);
	ft_putendl_fd(str, fd);
	ft_putstr_fd("! ", 2);
	ft_putendl_fd(str, 2);
}

void	warning_message_mul(int fd, char **str)
{
	int	index;

	index = 0;
	ft_putstr_fd("! ", fd);
	ft_putstr_fd("! ", 2);
	while (str[index] != NULL)
	{
		ft_putstr_fd(str[index], fd);
		ft_putstr_fd(str[index], 2);
		if (str[index + 1] == NULL)
		{
			ft_putchar_fd('\n', fd);
			ft_putchar_fd('\n', 2);
		}
		else
		{
			ft_putchar_fd(' ', fd);
			ft_putchar_fd(' ', 2);
		}
		index++;
	}
}

void	fatal_message(int fd, char *str)
{
	ft_putstr_fd("!!! ", fd);
	ft_putendl_fd(str, fd);
	ft_putstr_fd("!!! ", 2);
	ft_putendl_fd(str, 2);
}

void	fatal_message_mul(int fd, char **str)
{
	int	index;

	index = 0;
	ft_putstr_fd("!!! ", fd);
	ft_putstr_fd("!!! ", 2);
	while (str[index] != NULL)
	{
		ft_putstr_fd(str[index], fd);
		ft_putstr_fd(str[index], 2);
		if (str[index + 1] == NULL)
		{
			ft_putchar_fd('\n', fd);
			ft_putchar_fd('\n', 2);
		}
		else
		{
			ft_putchar_fd(' ', fd);
			ft_putchar_fd(' ', 2);
		}
		index++;
	}
}
