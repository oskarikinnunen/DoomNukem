/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 19:50:18 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/03 13:11:31 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

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
		printf("\ngetting image for object %s \n", object->name);
		while (m_i < object->material_count)
		{
			if (ft_strcmp(object->materials[m_i].texturename, "") != 0)
				object->materials[m_i].img = get_image_by_name(*sdl, object->materials[m_i].texturename);
			//printf("REQUESTED IMAGE NAME %s \n", object->materials[i].texturename);
			//printf("GOT		IMAGE NAME %s \n", object->materials[i].img->name);
			m_i++;
		}
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