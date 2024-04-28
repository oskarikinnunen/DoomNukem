/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 12:20:53 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"

static void	parse_mat_map_kd(char *line, t_material *mat)
{
	if (ft_strrchr(line, '/') != NULL)
		ft_strncpy_term(mat->texturename, ft_strrchr(line, '/') + 1, 250);
	else
		ft_strncpy_term(mat->texturename, line + sizeof("map_Kd"), 250);
}

static void	parse_mat_kd(char *line, t_material *mat)
{
	char	**kd_strs;
	int		i;

	kd_strs = ft_strsplit(line + sizeof("Kd"), ' ');
	if (kd_strs == NULL)
		doomlog(LOG_EC_MALLOC, "parsemat");
	i = 0;
	while (i < 3 && kd_strs[i] != NULL)
	{
		mat->kd += ((int)(ft_atof(kd_strs[i]) * 255.0f) & 0xFF) << (8 * i);
		free(kd_strs[i]);
		kd_strs[i] = NULL;
		i++;
	}
	free(kd_strs);
	kd_strs = NULL;
	if (i != 3)
		doomlog(LOG_FATAL, "invalid kd string!");
}

t_material	parse_mat(int fd, char *name)
{
	t_material	mat;
	char		*line;
	int			ret;

	ft_bzero(&mat, sizeof(t_material));
	ft_strncpy_term(mat.name, name, 250);
	line = NULL;
	ret = get_next_line(fd, &line);
	while (ret)
	{
		if (line)
		{
			if (ft_strnstr(line, "Kd ", 3) != NULL)
				parse_mat_kd(line, &mat);
			if (ft_strnstr(line, "map_Kd ", sizeof("map_Kd")) != NULL)
				parse_mat_map_kd(line, &mat);
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "parsemat");
	return (mat);
}

void	parse_mtllib(t_list **list, char *filename)
{
	t_mat_parse	mp;

	ft_bzero(&mp, sizeof(t_mat_parse));
	ft_strncpy_term(mp.mat_path, OBJPATH, 250);
	ft_strcat(mp.mat_path, filename);
	mp.fd = ft_fileopen(mp.mat_path, O_RDONLY);
	mp.ret = get_next_line(mp.fd, &mp.line);
	while (mp.ret)
	{
		if (mp.line)
		{
			if (ft_strstr(mp.line, "newmtl ") != NULL)
			{
				mp.mat = parse_mat(mp.fd, mp.line + sizeof("newmtl"));
				list_push(list, &mp.mat, sizeof(t_material));
			}
			free(mp.line);
			mp.line = NULL;
		}
		mp.ret = get_next_line(mp.fd, &mp.line);
	}
	if (mp.ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, mp.mat_path);
	ft_fileclose(mp.fd, mp.mat_path);
}
