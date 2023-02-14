/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 19:50:18 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/07 14:09:41 by okinnune         ###   ########.fr       */
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

void	parseanim(t_object *object, char *animname)
{
	int		i;
	int		fd;
	//char	name[256];
	t_objectanimframe	frame;
	char	*base;

	fd = 0;
	i = 0;
	base = basename(object->name);
	object->o_anim.framecount = 0;
	while (fd != -1)
	{
		char name[256] = { 0 };
		sprintf(name, "assets/objects/animations/%s_%s%i.obj", base, animname, i); //TODO: protect
		//printf("name %s \n", name);
		fd = open(name, O_RDONLY);
		//printf("%s \n", name);
		if (fd != -1)
		{
			ft_strcpy(object->o_anim.name, animname); //TODO: protect
			parse_animframe(fd, &frame, object);
			ptr_add((void **)&object->o_anim.frames, &object->o_anim.framecount, sizeof(t_objectanimframe), &frame);
			//ptr_add()
			close(fd);
		}
		i++;
	}
	//printf("anim has %i frames", object->o_anim.framecount);
	//exit(0);
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
		//printf("\ngetting image for object %s \n", object->name);
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