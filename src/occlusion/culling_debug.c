/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 14:59:10 by vlaine            #+#    #+#             */
/*   Updated: 2022/12/26 18:53:05 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void get_min_max_from_triangles(t_vector2 *min, t_vector2 *max, t_triangle *t, int count)
{
	t_vector3	v;
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
			v = t[i].p[j].v;
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
	t_triangle *t;

	if (ent->obj->bounds.type == bt_box)
	{
		t = render->occ_draw_tris;
		t[0].p[0].v = (t_vector3){.x = s.max.x, .y = s.max.y};
		t[0].p[1].v = (t_vector3){.x = s.max.x, .y = s.min.y};
		t[0].p[2].v = (t_vector3){.x = s.min.x, .y = s.max.y};
		t[1].p[0].v = t[0].p[1].v;
		t[1].p[1].v = t[0].p[2].v;
		t[1].p[2].v = (t_vector3){.x = s.min.x, .y = s.min.y};
		render->occ_tri_count = 2;
		return(2);
	}
	return(render->occ_tri_count);
}

void draw_wireframe(t_sdlcontext sdl, t_render *render, t_entity *e, uint32_t clr)
{
	t_square	s;
	int			i;

	render->occ_tri_count = 0;
	render->occ_calc_tri_count = 0;
	calculate_triangles(sdl, render, e);
	get_min_max_from_triangles(&s.min, &s.max, render->occ_draw_tris, render->occ_tri_count);
	calculate_tris_from_square(s, e, render);
	i = 0;
	while (i < render->occ_tri_count)
	{
		drawline(sdl, (t_point){render->occ_draw_tris[i].p[0].v.x, render->occ_draw_tris[i].p[0].v.y}, (t_point){render->occ_draw_tris[i].p[1].v.x, render->occ_draw_tris[i].p[1].v.y}, clr);
		drawline(sdl, (t_point){render->occ_draw_tris[i].p[2].v.x, render->occ_draw_tris[i].p[2].v.y}, (t_point){render->occ_draw_tris[i].p[1].v.x, render->occ_draw_tris[i].p[1].v.y}, clr);
		drawline(sdl, (t_point){render->occ_draw_tris[i].p[0].v.x, render->occ_draw_tris[i].p[0].v.y}, (t_point){render->occ_draw_tris[i].p[2].v.x, render->occ_draw_tris[i].p[2].v.y}, clr);
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