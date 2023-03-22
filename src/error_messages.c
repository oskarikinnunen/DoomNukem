/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:53:51 by raho              #+#    #+#             */
/*   Updated: 2023/03/01 18:39:58 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	error_message(int code, int fd, char *str)
{
	ft_putstr_fd("!!! ", fd);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(" - ", fd);
	error_codes(code, fd);
	ft_putstr_fd("!!! ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(" - ", 2);
	error_codes(code, 2);
}

void	error_message_mul(int code, int fd, char **str)
{
	int	index;

	ft_putstr_fd("!!! ", fd);
	ft_putstr_fd("!!! ", 2);
	index = 0;
	while (str[index] != NULL)
	{
		ft_putstr_fd(str[index], fd);
		ft_putstr_fd(str[index], 2);
		if (str[index + 1] != NULL)
		{
			ft_putchar_fd(' ', fd);
			ft_putchar_fd(' ', 2);
		}
		index++;
	}
	ft_putstr_fd(" - ", fd);
	error_codes(code, fd);
	ft_putstr_fd(" - ", 2);
	error_codes(code, 2);
}
