/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_faces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 15:55:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 18:35:22 by okinnune         ###   ########.fr       */
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

	ft_bzero(&result, sizeof(t_face));
	f_strs = ft_strsplit(line, ' ');
	if (f_strs == NULL)
		doomlog(LOG_EC_MALLOC, "parse_face");
	i = 0;
	while (i < 3 && f_strs[i] != NULL)
	{
		// TODO: make own version of atof()
		result.v_indices[i] = atoi(f_strs[i]); //Only takes the first number, so '3/2/9' would return 3 here.
		if (ft_strlen(f_strs[i]) > 2 && ft_strstr(f_strs[i], "/") != NULL)
			result.uv_indices[i] = ft_atoi(ft_strstr(f_strs[i], "/") + 1);
		free(f_strs[i]);
		i++;
	}
	free(f_strs);
	if (i != 3)
		doomlog(LOG_FATAL, "invalid face string!");
	return (result);
}

int			find_materialindex(char *matname, t_list *materials)
{
	t_material	*mat;
	t_list		*l;
	int			i;

	mat = NULL;
	l = materials;
	i = 0;
	while (l != NULL)
	{
		mat = (t_material *)l->content;
		if (ft_strcmp(matname, mat->name) == 0)
			return (i);
		l = l->next;
		i++;
	}
	return (0);
}

t_list	*get_face_list(int fd, t_list *materials)
{
	char		*line;
	t_list		*list;
	t_face		face;
	t_material	*mat;
	int			mat_index;
	int			ret;

	list = NULL;
	line = NULL;
	mat_index = 0;
	ret = get_next_line(fd, &line);
	while (ret)
	{
		if (line)
		{
			if (ft_strnstr(line, "usemtl ", sizeof("usemtl")))
			{
				mat_index = find_materialindex(line + sizeof("usemtl"), materials);
			}
			if (ft_strnstr(line, "f ", sizeof("f")))
			{
				face = parse_face(line + (sizeof("f")));
				face.materialindex = mat_index;
				list_push(&list, &face, sizeof(t_face));
			}
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "get_face_list");
	if (lseek(fd, 0, SEEK_SET) == -1)
		doomlog(LOG_EC_LSEEK, "get_face_list");
	return (list);
}