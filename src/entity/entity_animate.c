/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_animate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 11:42:28 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/22 14:54:06 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

void	entity_start_anim(t_entity *entity, char *animname)
{
	printf("entity has anim name %s \n", entity->obj->o_anim.name);
	if (ft_strcmp(entity->obj->o_anim.name, animname) == 0)
	{
		printf("START anim %s \n", animname);
		entity->animation.framerate = 13;
		entity->animation.lastframe = entity->obj->o_anim.framecount - 2;
		entity->animation.loop = true;
		printf("anim has %i frames (debug %i)\n", entity->animation.lastframe, entity->obj->o_anim.framecount);
		start_anim(&entity->animation, anim_forwards);
	}
}