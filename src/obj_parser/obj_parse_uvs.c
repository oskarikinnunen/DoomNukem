/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_uvs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:17:47 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/08 18:58:34 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "vectors.h"
#include "doomnukem.h"

t_vector2	parse_uv(char *line)
{
	t_vector2	result;
	char		**uv_strs;
	int			i;

	uv_strs = ft_strsplit(line, ' ');
	if (uv_strs == NULL)
		doomlog(LOG_EC_MALLOC, "parse_uv");
	i = 0;
	result = vector2_zero();
	while (i < 2 && uv_strs[i] != NULL)
	{
		// TODO: make own version of atof()
		if (i == 0)
			result.x = atof(uv_strs[i]);
		if (i == 1)
			result.y = atof(uv_strs[i]);
		free(uv_strs[i]);
		i++;
	}
	free(uv_strs);
	if (i != 2)
		doomlog(LOG_FATAL, "invalid uv string!");
	return (result);
}

t_list	*get_uv_list(int fd)
{
	char		*line;
	t_list		*list;
	t_vector2	uv;
	int			ret;

	list = NULL;
	line = NULL;
	ret = get_next_line(fd, &line);
	while (ret)
	{
		if (line)
		{
			if (ft_strnstr(line, "vt ", sizeof("vt")))
			{
				uv = parse_uv(line + sizeof("vt"));
				uv.y = 1.0f - uv.y;
				list_push(&list, &uv, sizeof(t_vector2));
			}
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "get_uv_list");
	if (lseek(fd, 0, SEEK_SET) == -1)
		doomlog(LOG_EC_LSEEK, "get_uv_list");
	return (list);
}