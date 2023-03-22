/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_objects.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:32:20 by raho              #+#    #+#             */
/*   Updated: 2023/03/22 14:19:31 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"

static void	playmode_parse_mtllib(t_list **list)
{
	char		*line;
	int			fd;
	t_material	mat;
	int			ret;

	fd = fileopen(TEMPMTL, O_RDONLY);
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
	fileclose(fd, TEMPMTL);
}

// returns a string with a different extension.
// both extensions needs to be 3 characters long
static char	*replace_extension(char *file, char *extension)
{
	char	*new;
	char	*temp;
	int		len;

	len = ft_strlen(file);
	new = (char *)malloc(sizeof(char) * len + 1);
	if (new != NULL)
	{
		ft_strcpy(new, file);
		ft_strcpy(&new[len - 3], extension);
	}
	return (new);
}

// material needs to be the same name as the object in the directory
static void	unpack_and_load_object(int obj_i, char *level_path,
									char *object_name, t_sdlcontext *sdl)
{
	t_list		*materials;
	char		*material_name;

	materials = NULL;
	material_name = replace_extension(object_name, "mtl");
	if (material_name == NULL)
		doomlog(LOG_EC_MALLOC, "couldn't replace object_name extension to mtl");
	load_and_write_filecontent(level_path, object_name, TEMPOBJ);
	load_and_write_filecontent(level_path, material_name, TEMPMTL);
	sdl->objects[obj_i] = objparse(TEMPOBJ);
	if (sdl->objects[obj_i].vertices != NULL)
		ft_strcpy(sdl->objects[obj_i].name, extract_filename(object_name));
	free(material_name);
	free(sdl->objects[obj_i].materials); // TODO: remove this and fix the material malloc in objparse
	playmode_parse_mtllib(&materials);
	sdl->objects[obj_i].materials = \
			list_to_ptr(materials, &sdl->objects[obj_i].material_count);
	listdel(&materials);
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded .obj file:", sdl->objects[obj_i].name, NULL});
}

static int	parse_object_list(int fd, char *level_path, t_sdlcontext *sdl)
{
	int		ret;
	int		i;
	char	*object_name;

	i = 0;
	object_name = NULL;
	ret = get_next_line(fd, &object_name);
	while (ret)
	{
		if (object_name)
		{
			unpack_and_load_object(i, level_path, object_name, sdl);
			free(object_name);
			object_name = NULL;
			i++;
		}
		ret = get_next_line(fd, &object_name);
	}
	return (ret);
}

void	playmode_load_objects(char *level_path, t_sdlcontext *sdl)
{
	int	fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING OBJECTS");
	load_and_write_filecontent(level_path, OBJLISTPATH, TEMPOBJLIST);
	sdl->objectcount = count_asset_list(TEMPOBJLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPOBJLIST, "size =", s_itoa(sdl->objectcount), NULL});
	sdl->objects = prot_memalloc(sizeof(t_object) * sdl->objectcount);
	fd = fileopen(TEMPOBJLIST, O_RDONLY);
	ret = parse_object_list(fd, level_path, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_objects");
	fileclose(fd, TEMPOBJLIST);
}
