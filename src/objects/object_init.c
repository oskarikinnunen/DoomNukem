/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 19:50:18 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 12:07:26 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "file_io.h"

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
			object->materials[m_i].img = \
				get_image_by_name(*sdl, object->materials[m_i].texturename);
			m_i++;
		}
		i++;
	}
}

//TODO: return debug object!!
t_object	*get_object_by_name(t_sdlcontext sdl, char *name)
{
	int		i;
	char	fullname[256];

	ft_bzero(fullname, sizeof(fullname));
	ft_strncpy_term(fullname, name, 250);
	if (ft_strstr(name, ".obj") == NULL)
		ft_strcat(fullname, ".obj");
	i = 0;
	while (i < sdl.objectcount)
	{
		if (ft_strcmp(sdl.objects[i].name, fullname) == 0)
			return (&sdl.objects[i]);
		i++;
	}
	return (&sdl.objects[0]);
}
