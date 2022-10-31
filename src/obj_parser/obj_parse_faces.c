/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_faces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 15:55:15 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/31 14:18:44 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "objects.h"
#include "libft.h"
#include "doomnukem.h" //Used for listhelper functions, TODO: move listhelper stuff to own header


//TODO: check and complain if faces are in 'quad' format
t_face	parse_face(char *line)
{
	t_face		result;
	char		**f_strs;
	int			i;

	f_strs = ft_strsplit(line, ' ');
	i = 0;
	while (i < 3 && f_strs[i] != NULL)
	{
		result.v_indices[i] = atoi(f_strs[i]); //Only takes the first number, so '3/2/9' would return 3 here.
		free(f_strs[i]);
		i++;
	}
	free(f_strs);
	if (i != 3)
		printf("invalid face string!\n"); //TODO: log?
	return (result);
}

t_list	*get_face_list(int fd)
{
	char		*line;
	t_list		*list;
	t_face		face;
	//t_vector3	vec;

	list = NULL;
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strnstr(line, "f ", sizeof("f")))
		{
			face = parse_face(line + (sizeof("f")));
			list_push(&list, &face, sizeof(t_face));
		}
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	return (list);
}