/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomlog.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 20:51:59 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	doomlog_open_error_exit(void)
{
	ft_putstr_fd("!!! doomlog.txt - ", 2);
	error_codes(LOG_EC_OPEN, 2);
	exit (1);
}

static int	doomlog_fd(void)
{
	static int	first_call = 1;
	int			fd;

	if (first_call)
	{
		fd = open("doomlog.txt", O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, \
							S_IRWXU | S_IRWXG | S_IRWXO);
		if (fd == -1)
			doomlog_open_error_exit();
		ft_putendl_fd("created/cleaned the log file succesfully", fd);
		first_call = 0;
	}
	else
	{
		fd = open("doomlog.txt", O_WRONLY | O_APPEND);
		if (fd == -1)
			doomlog_open_error_exit();
	}
	return (fd);
}

void	doomlog_mul(int code, char **str)
{
	int	fd;

	fd = doomlog_fd();
	if (code == LOG_NORMAL)
		normal_message_mul(fd, str);
	else if (code == LOG_WARNING)
		warning_message_mul(fd, str);
	else if (code == LOG_FATAL)
	{
		fatal_message_mul(fd, str);
		exit (1);
	}
	else
	{
		error_message_mul(code, fd, str);
		exit (1);
	}
	if (close(fd) == -1)
	{
		error_message(LOG_EC_CLOSE, fd, "doomlog.txt");
		exit (1);
	}
}

void	doomlog(int code, char *str)
{
	int	fd;

	fd = doomlog_fd();
	if (code == LOG_NORMAL)
		ft_putendl_fd(str, fd);
	else if (code == LOG_WARNING)
		warning_message(fd, str);
	else if (code == LOG_FATAL)
	{
		fatal_message(fd, str);
		exit (1);
	}
	else
	{
		error_message(code, fd, str);
		if (code != LOG_EC_FORK)
			exit (1);
	}
	if (close(fd) == -1)
	{
		error_message(code, fd, "doomlog.txt");
		exit (1);
	}
}
