/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize_lightpoly.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 07:34:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/23 08:07:22 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

/*

static t_vector3 texcoord_to_loc(t_vector3 ws[3], t_vector2 uv[3], t_vector2 p)
{
	float       i;
	float       s;
	float       delta;

	t_vector3   r;
	i = 1.0f / ((uv[1].x - uv[0].x) * (uv[2].y - uv[0].y) - (uv[1].y - uv[0].y) * (uv[2].x - uv[0].x));
	s = i * ((uv[2].y - uv[0].y) * (p.x - uv[0].x) - (uv[2].x - uv[0].x) * (p.y - uv[0].y));
	delta = i * (-(uv[1].y - uv[0].y) * (p.x - uv[0].x) + (uv[1].x - uv[0].x) * (p.y - uv[0].y));
	r.x = ws[0].x + s * (ws[1].x - ws[0].x) + delta * (ws[2].x - ws[0].x);
	r.y = ws[0].y + s * (ws[1].y - ws[0].y) + delta * (ws[2].y - ws[0].y);
	r.z = ws[0].z + s * (ws[1].z - ws[0].z) + delta * (ws[2].z - ws[0].z);
	return r;
}

static void sample_img(t_point xy, t_lightpoly poly)
{
	ray.origin = texcoord_to_loc(poly.world_coord, poly.uv_coord,
								(t_vector2){xy.x/(float)poly.lmap->size.x, xy.y/(float)poly.lmap->size.y});
	float dist = vector3_dist(ray.origin, poly.light->origin);
	if (dist <= poly.light->radius)
	{
		dist = 1.0f - (dist / poly.light->radius);
		poly.lmap->data[x + poly.lmap->size.x * y] = ft_clamp((dist * 255) + poly.lmap->data[x + poly.lmap->size.x * y], 0, 255);
	}
	*/
	/*t_ray		ray;
	t_texture	temp_t;

	if (l.lightmap->data[x + l.lightmap->size.x * y] != l.ambient_light)
	{
		//printf("light map data %i, ambient %i \n", l.lightmap->data[x + l.lightmap->size.x * y], l.ambient_light);
		return ;
	}
	ray.origin = texcoord_to_loc(t.p3, t.uv, (t_vector2){x/(float)l.lightmap->size.x, y/(float)l.lightmap->size.y});
	if (l.pointlight->shadows == false)
	{
		float dist = vector3_dist(ray.origin, l.pointlight->origin);
		if (dist <= l.pointlight->radius)
		{
			dist = 1.0f - (dist / l.pointlight->radius);
			l.lightmap->data[x + l.lightmap->size.x * y] = ft_clamp((dist * 255) + l.lightmap->data[x + l.lightmap->size.x * y], 0, 255);
		}
		return;
	}
}*/

/*static void fill_point_tri_top(t_lighting l, t_triangle_polygon triangle)
{
	t_point			*p;
	float			step[2];
	int				x;
	int				y;
	float			delta;

	p = triangle.p2;
	delta = 1.0f/((float)(p[0].y - p[1].y));
	step[0] = (p[0].x - p[1].x) * delta;
	step[1] = (p[0].x - p[2].x) * delta;
	y = p[1].y;
	while (y <= p[0].y)
	{
		x = p[1].x + (step[0] * (float)(y - p[1].y));
		int ax =  p[2].x + (step[1] * (float)(y - p[1].y));
		while(x <= ax)
		{
			sample_img(l, x, y, triangle);
			x++;
		}
		y++;
	}
}*/

typedef struct s_barycentric
{
	float	v1;
	float	v2;
	float	v3;
	float	v4;
	float	v5;
	float	b1;
	float	b2;
	float	b3;
}	t_barycentric;

static void fill_point_tri_bot(t_lighting *lighting, t_point_triangle triangle)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	t_texture		t_step[3];
	int				ax;
	int				bx;
	int				y;
	float			delta;
	float			w1;
	float			w2;
	t_texture		t_temp;
	t_texture		temp;
	t_barycentric	bary;

	p = triangle.p;
	t = triangle.t;
	delta = p[1].y - p[0].y;
	step[0] = (p[0].x - p[1].x) / delta;
	step[1] = (p[0].x - p[2].x) / delta;

	bary.b1 = p[2].y - p[0].y;
	bary.b2	= p[2].x - p[0].x;
	bary.b3 = p[1].y - p[0].y;

	bary.v1 = p[0].x * bary.b1;
	bary.v2 = bary.b3 * bary.b2 - (float)(p[1].x - p[0].x) * bary.b1;
	y = p[1].y;
	while (y >= p[0].y)
	{
		delta = p[1].y - y;
		ax = p[1].x + (step[0] * delta);
		bx = p[2].x + (step[1] * delta);
	
		float b_temp = bary.v1 + (float)(y - p[0].y) * bary.b2;
		w1 = (b_temp - ax * bary.b1) / bary.v2;
		w2 = (float)(y - p[0].y - w1 * bary.b3) / bary.b1;
		temp.w = ft_flerp(t[0].w, t[1].w, w1);
		temp.w += ((t[2].w - t[0].w) * w2);

		w1 = (b_temp - bx * bary.b1) / bary.v2;
		w2 = (float)(y - p[0].y - w1 * bary.b3) / bary.b1;
		t_temp.w = ft_flerp(t[0].w, t[1].w, w1);
		t_temp.w += ((t[2].w - t[0].w) * w2);

		//render_bitmask_row(ax, bx, 1.0f / temp.w, 1.0f / t_temp.w, y, sdl);
		delta = bx - ax;
		t_step[0].w = (t_temp.w - temp.w) / delta;
		while(ax <= bx)
		{
			int index = ax + y * lighting->resolution.x;
			if (temp.w > lighting->zbuffer[index])
			{
				lighting->zbuffer[index] = temp.w;
			}
			temp.w += t_step[0].w;
			ax++;
		}
		y--;
	}
}

static void fill_point_tri_top(t_lighting *lighting, t_point_triangle triangle)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	t_texture		t_step[3];
	int				ax;
	int				bx;
	int				y;
	float			delta;
	float			w1;
	float			w2;
	t_texture		t_temp;
	t_texture		temp;
	t_barycentric	bary;

	p = triangle.p;
	t = triangle.t;
	delta = p[0].y - p[1].y;
	step[0] = (p[0].x - p[1].x) / delta;
	step[1] = (p[0].x - p[2].x) / delta;

	bary.b1 = p[2].y - p[0].y;
	bary.b2	= p[2].x - p[0].x;
	bary.b3 = p[1].y - p[0].y;

	bary.v1 = p[0].x * bary.b1;
	bary.v2 = bary.b3 * bary.b2 - (float)(p[1].x - p[0].x) * bary.b1;

	y = p[1].y;
	while (y <= p[0].y)
	{
		delta = y - p[1].y;
		ax = p[1].x + (step[0] * delta);
		bx = p[2].x + (step[1] * delta);
	
		float b_temp = bary.v1 + (float)(y - p[0].y) * bary.b2;
		w1 = (b_temp - ax * bary.b1) / bary.v2;
		w2 = (float)(y - p[0].y - w1 * bary.b3) / bary.b1;
		temp.w = ft_flerp(t[0].w, t[1].w, w1);
		temp.w += ((t[2].w - t[0].w) * w2);

		w1 = (b_temp - bx * bary.b1) / bary.v2;
		w2 = (float)(y - p[0].y - w1 * bary.b3) / bary.b1;
		t_temp.w = ft_flerp(t[0].w, t[1].w, w1);
		t_temp.w += ((t[2].w - t[0].w) * w2);

		//render_bitmask_row(ax, bx, 1.0f / temp.w, 1.0f / t_temp.w, y, sdl);

		delta = bx - ax;
		t_step[0].w = (t_temp.w - temp.w) / delta;
		while(ax <= bx)
		{
			int index = ax + y * lighting->resolution.x;
			if (temp.w > lighting->zbuffer[index])
			{
				lighting->zbuffer[index] = temp.w;
			}
			temp.w += t_step[0].w;
			ax++;
		}
		y++;
	}
}

void	rasterize_zbuffer(t_lighting *lighting, t_point_triangle triangle)
{
	t_point				p_split;
	t_texture			t_split;
	t_texture			t_temp;
	t_point				p_temp;
	t_point				*p;
	float				lerp;

	p = triangle.p;
	sort_point_uv_tri(triangle.p, triangle.t);
	lerp = ((float)p[1].y - (float)p[2].y) / ((float)p[0].y - (float)p[2].y);
	p_split.x = p[2].x + (lerp * ((float)p[0].x - (float)p[2].x));
	p_split.y = p[1].y;
	t_split.w = ft_flerp(triangle.t[2].w, triangle.t[0].w, lerp);
	if (p_split.x < p[1].x)
	{
		ft_swap(&p[1], &p_split, sizeof(t_point));
		ft_swap(&triangle.t[1], &t_split, sizeof(t_texture));
	}
	p_temp = p[2];
	t_temp = triangle.t[2];
	p[2] = p_split;
	triangle.t[2] = t_split;
	if (p[0].y != p[1].y)
		fill_point_tri_top(lighting, triangle);
	p[0] = p_temp;
	triangle.t[0] = t_temp;
	if (p[0].y != p[1].y)
		fill_point_tri_bot(lighting, triangle);
}
