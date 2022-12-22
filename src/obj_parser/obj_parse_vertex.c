/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_vertex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 15:38:32 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/22 15:31:03 by okinnune         ###   ########.fr       */
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
	i = 0;
	while (i < 3 && v_strs[i] != NULL)
	{
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
		printf("invalid vertex string!\n"); //TODO: log?
	return (result);
}

t_list	*get_vertex_list(int fd)
{
	char		*line;
	t_list		*list;
	t_vector3	vec;

	list = NULL;
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strnstr(line, "v ", sizeof("v")))
		{
			vec = parse_vertex(line + sizeof("v"));
			vec = vector3_mul(vec, 10.0f);
			list_push(&list, &vec, sizeof(t_vector3));
		}
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	return (list);
}
