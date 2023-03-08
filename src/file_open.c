/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:16:48 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/08 19:28:01 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	fileclose(int fd, char *filename)
{
	if (close(fd) == -1)
		doomlog(LOG_EC_CLOSE, filename);
}

int	fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0666);
	if (fd == -1)
		doomlog(LOG_EC_OPEN, filename);
	return (fd);
}
