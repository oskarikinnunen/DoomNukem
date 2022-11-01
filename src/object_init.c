/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 19:50:18 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/01 15:29:08 by vlaine           ###   ########.fr       */
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
		while (m_i < object->material_count)
		{
			object->materials[i].img = get_image_by_name(*sdl, object->materials[i].texturename);
			m_i++;
		}
		i++;
	}
}