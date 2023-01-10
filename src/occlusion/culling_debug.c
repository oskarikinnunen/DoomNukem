/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 14:59:10 by vlaine            #+#    #+#             */
/*   Updated: 2023/01/10 12:57:06 by vlaine           ###   ########.fr       */
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

//sets the first two triangles of our triangle buffer, to min and max from square.
int calculate_tris_from_square(t_square s, t_entity *ent, t_render *render)
{
	t_point_triangle *t;

	t = render->screenspace_ptris;
	if (ent->obj->bounds.type == bt_box)
	{
		t[0].p[0] = (t_point){s.max.x, s.max.y};
		t[0].p[1] = (t_point){s.max.x, s.min.y};
		t[0].p[2] = (t_point){s.min.x, s.max.y};
		t[1].p[0] = t[0].p[1];
		t[1].p[1] = t[0].p[2];
		t[1].p[2] = (t_point){s.min.x, s.min.y};
		render->screenspace_ptri_count = 2;
		return(2); // prob not necessary, but I did use it at one point, but it was prorably legacy code and has been removed
	}
	else if (ent->obj->bounds.type == bt_plane)
	{
		t[0].p[0] = (t_point){s.max.x, s.max.y};
		t[0].p[1] = (t_point){s.max.x, s.min.y};
		t[0].p[2] = (t_point){s.min.x, s.max.y};
		t[1].p[0] = t[0].p[1];
		t[1].p[1] = t[0].p[2];
		t[1].p[2] = (t_point){s.min.x, s.min.y};
		render->screenspace_ptri_count = 2;
	}
	return(render->screenspace_ptri_count);
}

void draw_wireframe(t_sdlcontext sdl, t_render *render, t_entity *e, uint32_t clr)
{
	t_square	s;
	int			i;
	printf("entity id %d\n", e->id);

	s = e->occlusion.box;
	//calculate_tris_from_square(e->occlusion.box, e, render);
	drawline(sdl, (t_point){s.max.x, s.max.y}, (t_point){s.min.x, s.max.y}, clr);
	drawline(sdl, (t_point){s.min.x, s.max.y}, (t_point){s.min.x, s.min.y}, clr);
	drawline(sdl, (t_point){s.min.x, s.min.y}, (t_point){s.max.x, s.min.y}, clr);
	drawline(sdl, (t_point){s.max.x, s.min.y}, (t_point){s.max.x, s.max.y}, clr);
}
