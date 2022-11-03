/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_initialise.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:46:45 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/01 16:54:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "doomnukem.h"

void	entity_loadobject(t_sdlcontext sdl, t_entity *entity, t_list **tolist)
{
	if (entity->obj == NULL)
	{
		if (entity->object_index < sdl.objectcount)
			entity->obj = &sdl.objects[entity->object_index];
		else
			entity->obj = &sdl.objects[0];
	}

}