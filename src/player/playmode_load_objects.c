/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_objects.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:32:20 by raho              #+#    #+#             */
/*   Updated: 2023/03/14 15:17:21 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"

t_material	parsemat(int fd, char *name);

static void	playmode_parse_mtllib(t_list **list)
{
	char		*line;
	int			fd;
	t_material	mat;
	int			ret;

	fd = open(TEMPMTL, O_RDONLY);
	if (fd == -1)
		doomlog(LOG_EC_OPEN, "playmode_load_objects");
	line = NULL;
	ret = get_next_line(fd, &line);
	while (ret)
	{
		if (line)
		{
			if (ft_strstr(line, "newmtl ") != NULL)
			{
				mat = parsemat(fd, line + sizeof("newmtl"));
				list_push(list, &mat, sizeof(t_material));
			}
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, TEMPMTL);
	if (close(fd) == -1)
		doomlog(LOG_EC_CLOSE, TEMPMTL);
}

// changes the extension from .obj to .mtl
static void	change_extension(char *filename)
{
	
}

// material needs to be the same name as the object in the directory
static void	unpack_and_load_object(t_sdlcontext *sdl, char *object_name)
{
	static int	i = 0;
	t_list		*materials;
	char		material_name[256];
	char		*occurance;

	materials = NULL;
	ft_strcpy(material_name, "assets/objects/barrel.mtl"); // TODO: copy object name and replace extension with .mtl
	load_and_write_filecontent(LEVEL0FILE, object_name, TEMPOBJ);
	load_and_write_filecontent(LEVEL0FILE, material_name, TEMPMTL);
	sdl->objects[i] = objparse(TEMPOBJ);
	if (sdl->objects[i].vertices != NULL)
		ft_strcpy(sdl->objects[i].name, extract_filename(object_name));
	free(sdl->objects[i].materials); // TODO: remove this and fix the material malloc in objparse
	playmode_parse_mtllib(&materials);
	sdl->objects[i].materials = list_to_ptr(materials, &sdl->objects[i].material_count);
	doomlog_mul(LOG_NORMAL, (char *[3]){\
				"unpacked and loaded .obj file:", sdl->objects[i].name, NULL});
	i++;
	remove(TEMPOBJ);
	remove(TEMPMTL);
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
			unpack_and_load_object(sdl, object_name);
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

	doomlog(LOG_NORMAL, "UNPACKING OBJECTS");
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
