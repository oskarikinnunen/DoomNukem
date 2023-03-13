/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 19:50:18 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/13 14:01:55 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "file_io.h"

char	*basename(char *filename)
{
	char	**split;
	char	*result;
	int		i;

	split = ft_strsplit(filename, '.');
	if (split == NULL)
		doomlog(LOG_EC_MALLOC, "parseanim basename");
	i = 1;
	while (split[i] != NULL)
	{
		free(split[i]);
		i++;
	}
	result = split[0];
	free(split);
	return (result);
}

static char	*glue_anim_name(char *base, char *animname, int i)
{
	char	name[256];
	char	*newname;

	ft_strcpy(name, "assets/objects/animations/");
	ft_strcat(name, base);
	ft_strcat(name, "_");
	ft_strcat(name, animname);
	ft_strcat(name, s_itoa(i));
	ft_strcat(name, ".obj");
	newname = ft_memalloc(sizeof(name));
	if (newname == NULL)
		doomlog(LOG_EC_MALLOC, "parseanim anim_name");
	return (ft_strcpy(newname, name));
}

void	editor_parseanim(t_object *object, char *animname)
{
	int					i;
	int					fd;
	char				*name;
	t_objectanimframe	frame;
	char				*base;

	i = 0;
	fd = 0;
	base = basename(object->name);
	object->o_anim.framecount = 0;
	while (fd != -1)
	{
		name = glue_anim_name(base, animname, i);
		fd = open(name, O_RDONLY);
		if (fd != -1)
		{
			ft_strcpy(object->o_anim.name, animname);
			parse_animframe(fd, &frame, object);
			ptr_add((void **)&object->o_anim.frames, &object->o_anim.framecount, sizeof(t_objectanimframe), &frame);
			fileclose(fd, name);
		}
		free(name);
		i++;
	}
	free(base);
}

void	objects_init(t_sdlcontext *sdl)
{
	int			i;
	int			m_i;
	t_object	*object;

	i = 0;
	while (i < sdl->objectcount)
	{
		object = &sdl->objects[i];
		m_i = 0;
		while (m_i < object->material_count)
		{
			//if (ft_strcmp(object->materials[m_i].texturename, "") != 0)
				object->materials[m_i].img = get_image_by_name(*sdl, object->materials[m_i].texturename);
			/*else
			{
				printf("set null image, kd was %i\n", object->materials[m_i].kd & 0xFF);
				object->materials[m_i].img = NULL;
			}*/
			m_i++;
		}
		update_object_bounds(object);
		i++;
	}
}

t_object	*get_object_by_name(t_sdlcontext sdl, char *name)
{
	int		i;
	char	fullname[256];

	ft_bzero(fullname, sizeof(fullname));
	ft_strcpy(fullname, name);
	if (ft_strstr(name, ".obj") == NULL)
		ft_strcat(fullname, ".obj");
	i = 0;
	while (i < sdl.objectcount)
	{
		if (ft_strcmp(sdl.objects[i].name, fullname) == 0)
			return (&sdl.objects[i]);
		i++;
	}
	return (&sdl.objects[0]); //TODO: debug object!!
}