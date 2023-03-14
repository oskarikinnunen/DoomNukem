/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_fonts.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:48:10 by raho              #+#    #+#             */
/*   Updated: 2023/03/14 13:48:55 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"

static void	unpack_and_save_object(t_sdlcontext *sdl, char *object_name)
{
	static int	i = 0;

	load_and_write_filecontent(LEVEL0FILE, object_name, TEMPOBJ);
	sdl->objects[i] = objparse(TEMPOBJ);
	if (sdl->objects[i].vertices != NULL)
		ft_strcpy(sdl->objects[i].name, extract_filename(object_name));
	doomlog_mul(LOG_NORMAL, (char *[3]){\
				"unpacked and saved .obj file:", sdl->objects[i].name, NULL});
	i++;
	remove(TEMPOBJ);
}

static int	parse_object_list(int fd, t_sdlcontext *sdl)
{
	int		ret;
	char	*object_name;

	object_name = NULL;
	ret = get_next_line(fd, &object_name);
	while (ret)
	{
		if (object_name)
		{
			unpack_and_save_object(sdl, object_name);
			free(object_name);
			object_name = NULL;
		}
		ret = get_next_line(fd, &object_name);
	}
	return (ret);
}

void	playmode_load_objects(t_sdlcontext *sdl)
{
	int	fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING FONTS");
	load_and_write_filecontent(LEVEL0FILE, OBJLISTPATH, TEMPOBJLIST);
	sdl->objectcount = count_asset_list(TEMPOBJLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPOBJLIST, "size =", s_itoa(sdl->objectcount), NULL});
	sdl->objects = ft_memalloc(sizeof(t_img) * sdl->objectcount);
	if (sdl->objects == NULL)
		doomlog(LOG_EC_MALLOC, "playmode_load_objects");
	fd = fileopen(TEMPOBJLIST, O_RDONLY);
	ret = parse_object_list(fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_objects");
	fileclose(fd, TEMPOBJLIST);
	remove(TEMPOBJLIST);
}