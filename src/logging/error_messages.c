/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:53:51 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 13:28:37 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	combine_strings_count_length(char **str)
{
	int	x;
	int	y;
	int	len;

	y = 0;
	len = 0;
	while (str[y] != NULL)
	{
		x = 0;
		while (str[y][x] != '\0')
			x++;
		len += x;
		if (str[y + 1] != NULL)
			len += 1;
		y++;
	}
	return (len);
}

char	*combine_strings(char **str)
{
	int		x;
	int		y;
	int		len;
	char	*result;

	len = combine_strings_count_length(str);
	result = ft_strnew(len);
	if (result == NULL)
		return (result);
	y = 0;
	x = 0;
	while (str[y] != NULL)
	{
		ft_strcpy(&result[x], str[y]);
		x += ft_strlen(str[y]);
		result[x] = ' ';
		x++;
		y++;
	}
	result[len] = '\0';
	return (result);
}

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
