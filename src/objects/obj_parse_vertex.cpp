/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_vertex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 15:38:32 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 20:46:00 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "objects.h"
#include "doomnukem.h"

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
		if (i == 0)
			result.x = ft_atof(v_strs[i]);
		if (i == 1)
			result.y = ft_atof(v_strs[i]);
		if (i == 2)
			result.z = ft_atof(v_strs[i]);
		free(v_strs[i]);
		i++;
	}
	free(v_strs);
	if (i != 3)
		doomlog(LOG_FATAL, "invalid vertex string!");
	return (result);
}

static void	handle_vertex_parsing(t_vertex_parse *vp)
{
	vp->vec = parse_vertex(vp->line + sizeof("v"));
	vp->vec = vector3_mul(vp->vec, 10.0f);
	list_push(&vp->list, &vp->vec, sizeof(t_vector3));
}

t_list	*get_vertex_list(int fd)
{
	t_vertex_parse	vp;

	ft_bzero(&vp, sizeof(t_vertex_parse));
	vp.ret = get_next_line(fd, &vp.line);
	while (vp.ret)
	{
		if (vp.line)
		{
			if (ft_strnstr(vp.line, "v ", sizeof("v")))
				handle_vertex_parsing(&vp);
			free (vp.line);
			vp.line = NULL;
		}
		vp.ret = get_next_line(fd, &vp.line);
	}
	if (vp.ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "objparse");
	if (lseek(fd, 0, SEEK_SET) == -1)
		doomlog(LOG_EC_LSEEK, "get_vertex_list");
	return (vp.list);
}
