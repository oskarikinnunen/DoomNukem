/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:16:48 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 16:45:58 by okinnune         ###   ########.fr       */
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
