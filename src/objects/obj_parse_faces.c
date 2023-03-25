/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_faces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 15:55:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 16:23:19 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "objects.h"
#include "doomnukem.h"

// TODO: check and complain if faces are in 'quad' format
// First atoi only takes the first number, so '3/2/9/' would return 3 here
t_face	parse_face(char *line)
{
	t_face	result;
	char	**f_strs;
	int		i;

	ft_bzero(&result, sizeof(t_face));
	f_strs = ft_strsplit(line, ' ');
	if (f_strs == NULL)
		doomlog(LOG_EC_MALLOC, "parse_face");
	i = 0;
	while (i < 3 && f_strs[i] != NULL)
	{
		result.v_indices[i] = ft_atoi(f_strs[i]);
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

static void	handle_face_parsing(t_face_list_parse *flp)
{
	flp->face = parse_face(flp->line + (sizeof("f")));
	flp->face.material_index = flp->mat_index;
	list_push(&flp->list, &flp->face, sizeof(t_face));
}

int	find_material_index(char *matname, t_list *materials)
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
	t_face_list_parse	flp;

	ft_bzero(&flp, sizeof(t_face_list_parse));
	flp.ret = get_next_line(fd, &flp.line);
	while (flp.ret)
	{
		if (flp.line)
		{
			if (ft_strnstr(flp.line, "usemtl ", sizeof("usemtl")))
				flp.mat_index = find_material_index(flp.line + \
									sizeof("usemtl"), materials);
			if (ft_strnstr(flp.line, "f ", sizeof("f")))
				handle_face_parsing(&flp);
			free(flp.line);
			flp.line = NULL;
		}
		flp.ret = get_next_line(fd, &flp.line);
	}
	if (flp.ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "get_face_list");
	if (lseek(fd, 0, SEEK_SET) == -1)
		doomlog(LOG_EC_LSEEK, "get_face_list");
	return (flp.list);
}
