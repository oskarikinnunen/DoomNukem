/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_uvs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:17:47 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/03 11:57:52 by okinnune         ###   ########.fr       */
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
	//system("tar")
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strnstr(line, "vt ", sizeof("vt")))
		{
			uv = parse_uv(line + sizeof("vt"));
			uv.y = 1.0f - uv.y;
			//uv = vector2_mul(uv, 100.0f);
			list_push(&list, &uv, sizeof(t_vector2));
		}
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	return (list);
}