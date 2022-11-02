/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 19:50:18 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 13:18:40 by okinnune         ###   ########.fr       */
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