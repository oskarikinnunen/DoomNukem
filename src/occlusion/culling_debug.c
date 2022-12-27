/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 14:59:10 by vlaine            #+#    #+#             */
/*   Updated: 2022/12/27 20:14:21 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void get_min_max_from_triangles(t_vector2 *min, t_vector2 *max, t_point_triangle *t, int count)
{
	t_vector2	v;
	int			i;

	max->x = -1000000000;
	max->y = -1000000000;
	min->x = 1000000000;
	min->y = 1000000000;
	i = 0;
	while (i < count)
	{
		for (int j = 0; j < 3; j++)
		{
			v = (t_vector2){t[i].p[j].x, t[i].p[j].y};
			if (v.x < min->x)
				min->x = v.x;
			if (v.x > max->x)
				max->x = v.x;
			if (v.y < min->y)
				min->y = v.y;
			if (v.y > max->y)
				max->y = v.y;
		}
		i++;
	}
}

int calculate_tris_from_square(t_square s, t_entity *ent, t_render *render)
{
	t_point_triangle *t;

	if (ent->obj->bounds.type == bt_box)
	{
		t = render->screenspace_ptris;
		t[0].p[0] = (t_point){s.max.x, s.max.y};
		t[0].p[1] = (t_point){s.max.x, s.min.y};
		t[0].p[2] = (t_point){s.min.x, s.max.y};
		t[1].p[0] = t[0].p[1];
		t[1].p[1] = t[0].p[2];
		t[1].p[2] = (t_point){s.min.x, s.min.y};
		render->screenspace_ptri_count = 2;
		return(2);
	}
	return(render->screenspace_ptri_count);
}

void draw_wireframe(t_sdlcontext sdl, t_render *render, t_entity *e, uint32_t clr)
{
	t_square	s;
	int			i;

	if (strcmp(e->obj->name, "bench.obj") == 0 || clr != CLR_BLUE)
	{
		return;
	}
	render->worldspace_ptri_count = 0;
	render->screenspace_ptri_count = 0;
	calculate_triangles(sdl, render, e);
	get_min_max_from_triangles(&s.min, &s.max, render->screenspace_ptris, render->screenspace_ptri_count);
	calculate_tris_from_square(s, e, render);
	i = 0;
	while (i < render->screenspace_ptri_count)
	{
		drawline(sdl, render->screenspace_ptris[i].p[0], render->screenspace_ptris[i].p[1], clr);
		drawline(sdl, render->screenspace_ptris[i].p[2], render->screenspace_ptris[i].p[1], clr);
		drawline(sdl, render->screenspace_ptris[i].p[0], render->screenspace_ptris[i].p[2], clr);
		i++;
	}
}

void draw_edges(t_sdlcontext sdl, t_render *render, t_entity *e, uint32_t clr)
{
	t_occlusion o;
	int			i;

	o = e->occlusion;
	i = 0;
	while (i < o.occluder_count)
	{
		drawline(sdl, (t_point){o.occluder[i].vector[0].x, o.occluder[i].vector[0].y}, (t_point){o.occluder[i].vector[1].x, o.occluder[i].vector[1].y}, clr);
		i++;
	}
}