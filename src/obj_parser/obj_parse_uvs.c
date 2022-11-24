/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_uvs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:17:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/22 17:14:26 by okinnune         ###   ########.fr       */
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
	i = 0;
	result = vector2_zero();
	while (i < 2 && uv_strs[i] != NULL)
	{
		if (i == 0)
			result.x = atof(uv_strs[i]);
		if (i == 1)
			result.y = atof(uv_strs[i]);
		free(uv_strs[i]);
		i++;
	}
	free(uv_strs);
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
			float tempf;
			tempf = uv.x;
			uv.x = -uv.y;
			uv.y = tempf;
			list_push(&list, &uv, sizeof(t_vector2));
		}
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	return (list);
}