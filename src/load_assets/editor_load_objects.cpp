/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_load_objects.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 17:22:47 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "assets.h"

static int	parse_object_list(int fd, t_sdlcontext *sdl)
{
	int		i;
	int		ret;
	char	*object_path;

	i = 0;
	object_path = NULL;
	ret = get_next_line(fd, &object_path);
	while (ret)
	{
		if (object_path)
		{
			sdl->objects[i] = objparse(object_path);
			if (sdl->objects[i].vertices != NULL)
				ft_strncpy_term(sdl->objects[i].name, \
						extract_file_name(object_path), 250);
			doomlog_mul(LOG_NORMAL, (char *[3]){\
					"parsed .obj file:", sdl->objects[i].name, NULL});
			free(object_path);
			object_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &object_path);
	}
	return (ret);
}

void	editor_load_objects(t_sdlcontext *sdl)
{
	int	ret;
	int	fd;

	doomlog(LOG_NORMAL, "LOADING OBJECTS");
	sdl->objectcount = count_asset_list(OBJLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){OBJLISTPATH, \
			"size =", s_itoa(sdl->objectcount), NULL});
	sdl->objects = prot_memalloc(sizeof(t_object) * sdl->objectcount);
	fd = ft_fileopen(OBJLISTPATH, O_RDONLY);
	ret = parse_object_list(fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, OBJLISTPATH);
	ft_fileclose(fd, OBJLISTPATH);
}
