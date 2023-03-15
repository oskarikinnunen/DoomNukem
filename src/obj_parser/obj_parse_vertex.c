/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_vertex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 15:38:32 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/08 18:58:42 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "objects.h"
#include "libft.h"
#include "doomnukem.h" //Used for listhelper functions, TODO: move listhelper stuff to own header

t_vector3	parse_vertex(char *line)
{
	t_vector3	result;
	char		**v_strs;
	int			i;

	v_strs = ft_strsplit(line, ' ');
	if (v_strs == NULL)
		doomlog(LOG_EC_MALLOC, "parse_vertex");
	i = 0;
	while (i < 3 && v_strs[i] != NULL)
	{
		// TODO: make own version of atof()
		if (i == 0)
			result.x = atof(v_strs[i]);
		if (i == 1)
			result.y = atof(v_strs[i]);
		if (i == 2)
			result.z = atof(v_strs[i]);
		free(v_strs[i]);
		i++;
	}
	free(v_strs);
	if (i != 3)
		doomlog(LOG_FATAL, "invalid vertex string!");
	return (result);
}

t_list	*get_vertex_list(int fd)
{
	char		*line;
	t_list		*list;
	t_vector3	vec;
	int			ret;

	list = NULL;
	line = NULL;
	ret = get_next_line(fd, &line);
	while (ret)
	{
		if (line)
		{
			if (ft_strnstr(line, "v ", sizeof("v")))
			{
				vec = parse_vertex(line + sizeof("v"));
				vec = vector3_mul(vec, 10.0f);
				list_push(&list, &vec, sizeof(t_vector3));
			}
			free (line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "objparse");
	if (lseek(fd, 0, SEEK_SET) == -1)
		doomlog(LOG_EC_LSEEK, "get_vertex_list");
	return (list);
}
