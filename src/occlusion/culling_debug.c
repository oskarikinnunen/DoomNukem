/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 14:59:10 by vlaine            #+#    #+#             */
/*   Updated: 2023/01/11 08:26:33 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void set_square_from_triangles(t_occlusion *occl, t_point_triangle *t, int count)
{
	t_square	*s;
	t_point		v;
	float		dist;
	int			i;

	s = &occl->box;
	s->max.x = -1000000000;
	s->max.y = -1000000000;
	s->min.x = 1000000000;
	s->min.y = 1000000000;
	occl->z_dist[0] = -100000.0f;
	occl->z_dist[1] = 100000.0f;
	i = 0;
	while (i < count)
	{
		for (int j = 0; j < 3; j++)
		{
			v = t[i].p[j];
			if (v.x < s->min.x)
				s->min.x = v.x;
			if (v.x > s->max.x)
				s->max.x = v.x;
			if (v.y < s->min.y)
				s->min.y = v.y;
			if (v.y > s->max.y)
				s->max.y = v.y;
			dist = 1.0f / t[i].t[j].w;
			if (dist > occl->z_dist[0])
				occl->z_dist[0] = dist;
			if (dist < occl->z_dist[1])
				occl->z_dist[1] = dist;
		}
		i++;
	}
}

void draw_wireframe(t_sdlcontext sdl, t_entity *e, uint32_t clr)
{
	t_square	s;
	int			i;

	if (e->obj->bounds.type != bt_box)
		return;
	s = e->occlusion.box;
	drawline(sdl, (t_point){s.max.x, s.max.y}, (t_point){s.min.x, s.max.y}, clr);
	drawline(sdl, (t_point){s.min.x, s.max.y}, (t_point){s.min.x, s.min.y}, clr);
	drawline(sdl, (t_point){s.min.x, s.min.y}, (t_point){s.max.x, s.min.y}, clr);
	drawline(sdl, (t_point){s.max.x, s.min.y}, (t_point){s.max.x, s.max.y}, clr);
}
