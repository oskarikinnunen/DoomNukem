/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomlog.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/02/03 22:52:21 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include <errno.h>

/* joins all the strings in an array together with spaces in between */
/* char	*arr_strjoin(char **str)
{
	char	*result;
	int		len;
	int		h;
	int		i;
	int		j;

	h = 0;
	len = 0;
	while (str[h] != NULL)
	{
		len += ft_strlen(str[h]);
		if (str[h + 1] != NULL)
			len += 1;
		h++;
	}
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (result != NULL)
	{
		j = 0;
		h = 0;
		while (str[h] != NULL)
		{
			i = 0;
			while (str[h][i] != '\0')
				result[j++] = str[h][i++];
			if (str[h + 1] != NULL)
				result[j++] = ' ';
			h++;
		}
		result[j] = '\0';
	}
	return (result);
} */

char	*s_itoa(int i)
{
	static char str[32];
	char 		*a;

	a = ft_itoa(i);
	ft_strcpy(str, a);
	free(a);
	return (str);
}

static int	doomlog_fd(void)
{
	static int	start;
	int			fd;

	if (!start)
	{
		fd = open("doomlog.txt", O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, \
							S_IRWXU | S_IRWXG | S_IRWXO);
		if (fd == -1)
		{
			ft_putstr_fd("!!! doomlog.txt - ", 2);
			error_codes(LOGEC_OPEN, 2);
			exit (1);
		}
		ft_putendl_fd("created/cleaned the log file succesfully", fd);
		start = 1;
	}
	else
	{
		fd = open("doomlog.txt", O_WRONLY | O_APPEND);
		if (fd == -1)
		{
			ft_putstr_fd("!!! doomlog.txt - ", 2);
			error_codes(LOGEC_OPEN, 2);
			exit (1);
		}
	}
	return (fd);
}

static void	normal_message_mul(int fd, char **str)
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

static void	warning_message(int fd, char *str)
{
	ft_putstr_fd("! ", fd);
	ft_putendl_fd(str, fd);
	ft_putstr_fd("! ", 2);
	ft_putendl_fd(str, 2);
}

static void warning_message_mul(int fd, char **str)
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

static void	fatal_message(int fd, char *str)
{
	ft_putstr_fd("!!! ", fd);
	ft_putstr_fd(str, fd);
}

static void	fatal_message_mul(int fd, char **str)
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

static void	error_message(int code, int fd, char *str)
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

static void	error_message_mul(int code, int fd, char **str)
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

void	doomlog_mul(int code, char **str)
{
	int	fd;
	int	index;

	fd = doomlog_fd();
	index = 0;
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
		error_message(LOGEC_CLOSE, fd, "log.txt");
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
		exit (1);
	}
	if (close(fd) == -1)
	{
		error_message(code, fd, "log.txt");
		exit (1);
	}
}
