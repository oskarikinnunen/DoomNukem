/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/01/28 15:03:38 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int	init_log(void)
{
	int	fd;

	fd = open("log.txt", O_CREAT | O_WRONLY | O_TRUNC | O_APPEND);
	if (fd == -1)
	{
		ft_putendl_fd("creating/opening the log file failed", 2);
		return (-1);
	}
	write_to_file("created/opened the log file succesfully", fd);
	return (fd);
}
