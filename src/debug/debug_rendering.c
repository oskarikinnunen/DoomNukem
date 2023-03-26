/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_rendering.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 19:55:24 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 16:10:36 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "entity.h"

void	draw_worldspace_icon(t_vector3 pos, t_img *img, t_world *world)
{
	float	qdist;
	t_point	p;

	qdist = vector3_sqr_dist(pos, world->player->transform.position);
	p = vector3_to_screenspace(pos, *world->sdl);
	if (qdist < 16000.0f)
		draw_image(*world->sdl, p, *img, (t_point){32, 32});
	else
		draw_image(*world->sdl, p, *img, (t_point){16, 16});
}

void	draw_entity_icon(t_entity *entity, t_img *img, t_world *world)
{
	float	qdist;
	t_point	p;

	qdist = vector3_sqr_dist(entity->transform.position,
			world->player->transform.position);
	p = vector3_to_screenspace(entity->transform.position, *world->sdl);
	if (qdist < 16000.0f && !entity->occlusion.is_occluded)
		draw_image(*world->sdl, p, *img, (t_point){32, 32});
	else
		draw_image(*world->sdl, p, *img, (t_point){16, 16});
}

void	debug_ramps(t_game *game)
{
	t_list			*l;
	t_vector3_tri	*tri;
	int				i;

	l = game->world.ramps;
	i = 0;
	while (l != NULL)
	{
		tri = (t_vector3_tri *)l->content;
		if (i % 2 == 0)
			render_ray3d(game->world.sdl, tri->c, tri->a, CLR_RED);
		else
			render_ray3d(game->world.sdl, tri->b, tri->c, CLR_BLUE);
		i++;
		l = l->next;
	}
}
