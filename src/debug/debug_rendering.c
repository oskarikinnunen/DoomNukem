/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_rendering.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 19:55:24 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/07 13:06:48 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "entity.h"

void	draw_worldspace_icon(t_vector3 pos, t_img *img, t_world *world)
{
	float	qdist;
	qdist = vector3_sqr_dist(pos, world->player->transform.position);
	t_point p = vector3_to_screenspace(pos, *world->sdl);
	if (qdist < 16000.0f)
		draw_image(*world->sdl, p, *img, (t_point){32,32});
	else
		draw_image(*world->sdl, p, *img, (t_point){16,16});
	//t_point p = vector3_to_screenspace(entity->transform.position, *world->sdl);
	/*if (qdist < 16000.0f && !entity->occlusion.is_occluded)
		draw_image(*world->sdl, p, *img, (t_point){32,32});
	else
		draw_image(*world->sdl, p, *img, (t_point){16,16});*/
}

void	draw_entity_icon(t_entity *entity, t_img *img, t_world *world)
{
	float	qdist;
	qdist = vector3_sqr_dist(entity->transform.position, world->player->transform.position);
	t_point p = vector3_to_screenspace(entity->transform.position, *world->sdl);
	if (qdist < 16000.0f && !entity->occlusion.is_occluded)
		draw_image(*world->sdl, p, *img, (t_point){32,32});
	else
		draw_image(*world->sdl, p, *img, (t_point){16,16});
}
