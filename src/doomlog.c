/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/01/28 17:08:10 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int	init_doomlog(void)
{
	int	fd;

	fd = open("log.txt", O_CREAT | O_WRONLY | O_TRUNC | O_APPEND);
	if (fd == -1)
		doomlog(LOGEC_OPEN, "couldn't create/open the log file", 2);
	doomlog(LOG_NORMAL, fd, "created/opened the log file succesfully");
	return (fd);
}

void	close_doomlog(int fd)
{
	if (close(fd) == -1)
		doomlog(LOGEC_CLOSE, "couldn't close the log file", fd);
}

void	doomlog(int code, int fd, char *str)
{
	if (code >= 2)
	{
		errors(code, fd, str);
		exit (1);
	}
	if (code == LOG_WARNING)
	{
		// what to do with warnings?
		ft_putendl_fd(str, fd);
	}
	if (code == LOG_NORMAL)
		ft_putendl_fd(str, fd);
}
