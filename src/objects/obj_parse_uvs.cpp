/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_uvs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:17:47 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 20:45:47 by raho             ###   ########.fr       */
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
		if (i == 0)
			result.x = ft_atof(uv_strs[i]);
		if (i == 1)
			result.y = ft_atof(uv_strs[i]);
		free(uv_strs[i]);
		i++;
	}
	free(uv_strs);
	if (i != 2)
		doomlog(LOG_FATAL, "invalid uv string!");
	return (result);
}

static void	handle_uv_parsing(t_uv_parse *uvp)
{
	uvp->uv = parse_uv(uvp->line + sizeof("vt"));
	uvp->uv.y = 1.0f - uvp->uv.y;
	list_push(&uvp->list, &uvp->uv, sizeof(t_vector2));
}

t_list	*get_uv_list(int fd)
{
	t_uv_parse	uvp;

	ft_bzero(&uvp, sizeof(t_uv_parse));
	uvp.ret = get_next_line(fd, &uvp.line);
	while (uvp.ret)
	{
		if (uvp.line)
		{
			if (ft_strnstr(uvp.line, "vt ", sizeof("vt")))
				handle_uv_parsing(&uvp);
			free(uvp.line);
			uvp.line = NULL;
		}
		uvp.ret = get_next_line(fd, &uvp.line);
	}
	if (uvp.ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "get_uv_list");
	if (lseek(fd, 0, SEEK_SET) == -1)
		doomlog(LOG_EC_LSEEK, "get_uv_list");
	return (uvp.list);
}
