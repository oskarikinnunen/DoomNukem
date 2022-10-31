/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_uvs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:17:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/31 16:13:07 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "vectors.h"
#include "doomnukem.h"

t_vector2	parse_uv(char *line)
{
	t_vector2	result;
	char		**v_strs;
	int			i;

	v_strs = ft_strsplit(line, ' ');
	i = 0;
	while (i < 2 && v_strs[i] != NULL)
	{
		if (i == 0)
			result.x = atof(v_strs[i]);
		if (i == 1)
			result.y = atof(v_strs[i]);
		free(v_strs[i]);
		i++;
	}
	free(v_strs);
	if (i != 2)
		printf("invalid uv string!\n"); //TODO: log?
	return (result);
}

t_list	*get_uv_list(int fd)
{
	char		*line;
	t_list		*list;
	t_vector2	uv;

	list = NULL;
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strnstr(line, "vt ", sizeof("vt")))
		{
			uv = parse_uv(line + sizeof("vt"));
			list_push(&list, &uv, sizeof(t_vector2));
		}
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	return (list);
}