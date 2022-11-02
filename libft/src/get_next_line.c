/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 16:02:38 by raho              #+#    #+#             */
/*   Updated: 2022/07/25 13:19:05 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	save_line_helper(char **str, char **line, int index)
{
	char	*temp;

	*line = ft_strsub(*str, 0, index);
	if (*line == NULL)
		return (-1);
	temp = ft_strdup(&((*str)[index + 1]));
	if (temp == NULL)
		return (-1);
	free(*str);
	*str = temp;
	if ((*str)[0] == '\0')
	{
		free(*str);
		*str = NULL;
	}
	return (0);
}

static int	save_line(char **str, char **line)
{
	int		index;
	char	check;

	index = 0;
	while ((*str)[index] != '\0' && (*str)[index] != '\n')
		index++;
	if ((*str)[index] == '\n')
	{
		check = save_line_helper(str, line, index);
		if (check == -1)
			return (-1);
	}
	else
	{
		*line = ft_strdup(*str);
		if (*line == NULL)
			return (-1);
		free(*str);
		*str = NULL;
	}
	return (1);
}

static int	result(int ret, char **str, char **line)
{
	if (ret == -1)
		return (-1);
	if (ret == 0 && *str == NULL)
		return (0);
	return (save_line(str, line));
}

static int	read_helper(char **str, char *buff)
{
	char	*temp;

	temp = NULL;
	if (!*str)
	{
		*str = ft_strdup(buff);
		if (!*str)
			return (-1);
	}
	else
	{
		temp = ft_strjoin(*str, buff);
		if (!temp)
			return (-1);
		free(*str);
		*str = temp;
	}
	return (0);
}

int	get_next_line(const int fd, char **line)
{
	int			ret;
	char		buff[BUFF_SIZE + 1];
	static char	*str[4096];

	if (fd < 0 || !(line))
		return (-1);
	ret = read(fd, buff, BUFF_SIZE);
	while (ret > 0)
	{
		buff[ret] = '\0';
		if (read_helper(&str[fd], buff) == -1)
			return (-1);
		if (ft_strchr(str[fd], '\n'))
			break ;
		ret = read(fd, buff, BUFF_SIZE);
	}
	return (result(ret, &str[fd], line));
}
