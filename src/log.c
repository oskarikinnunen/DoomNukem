/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/01/31 16:23:59 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	doomlog(int code, char *str)
{
	static int	start;
	int			fd;

	if (!start)
	{
		fd = open("log.txt", O_CREAT | O_WRONLY | O_TRUNC | O_APPEND);
		if (fd == -1)
			errors(LOGEC_OPEN, 2, "target: log.txt");
		ft_putendl_fd("created/opened the log file succesfully", fd);
		start = 1;
	}
	else
	{
		fd = open("log.txt", O_WRONLY | O_APPEND);
		if (fd == -1)
			errors(LOGEC_OPEN, 2, "target: log.txt");
	}
	if (code == LOG_NORMAL || code == LOG_WARNING)
		ft_putendl_fd(str, fd);
	else
		errors(code, fd, str);
	if (close(fd) == -1)
		errors(LOGEC_CLOSE, fd, "target: log.txt");
}
