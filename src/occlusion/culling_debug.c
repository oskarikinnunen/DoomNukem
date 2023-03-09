/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 14:59:10 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/07 13:06:52 by raho             ###   ########.fr       */
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

void draw_wireframe(t_sdlcontext sdl, t_square s, uint32_t clr)
{

//	if (e->obj->bounds.type != bt_box)
	//	return;
	drawline(sdl, (t_point){s.max.x, s.max.y}, (t_point){s.min.x, s.max.y}, clr);
	drawline(sdl, (t_point){s.min.x, s.max.y}, (t_point){s.min.x, s.min.y}, clr);
	drawline(sdl, (t_point){s.min.x, s.min.y}, (t_point){s.max.x, s.min.y}, clr);
	drawline(sdl, (t_point){s.max.x, s.min.y}, (t_point){s.max.x, s.max.y}, clr);
}

void bitmask_to_pixels(t_sdlcontext *sdl)
{
	if (sdl->render.occlusion.draw_occlusion == false)
		return;
	uint32_t	clr;

	float max_w = sdl->bitmask.max_dist;
	for (int y = 0; y < sdl->window_h; y++)
	{
		for (int x = 0; x < sdl->window_w; x++)
		{
			if (sdl->zbuffer[y * sdl->window_w + x] > 1.0f)
				continue;
			float w = sdl->bitmask.tile[(y / 8) * sdl->bitmask.tile_chunks.x + (x / 8)].max0;
			if (sdl->bitmask.tile[(y / 8) * sdl->bitmask.tile_chunks.x + (x / 8)].max0 - 500.0f > max_w)
			{
				w = sdl->bitmask.tile[(y / 8) * sdl->bitmask.tile_chunks.x + (x / 8)].max1;
				if ((sdl->bitmask.tile[(y / 8) * sdl->bitmask.tile_chunks.x + (x / 8)].mask >> ((((y % 8)) * 8) + (7 - (x % 8))) & 1) == 0)
					continue;
			}
			clr = INT_MAX;
			w = (max_w - w) / max_w;
			w = ft_flerp(0.2f, 0.8f, w);
			uint8_t p = ft_clamp(w * 255.0f, 0, 255);
			Uint32 alpha = clr & 0xFF000000;
			Uint32 red = ((clr & 0x00FF0000) * p) >> 8;
			Uint32 green = ((clr & 0x0000FF00) * p) >> 8;
			Uint32 blue = ((clr & 0x000000FF) * p) >> 8;
			clr = alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF);
			((uint32_t *)sdl->surface->pixels)[sdl->window_w * y + x] = clr;
		}
	}
}
