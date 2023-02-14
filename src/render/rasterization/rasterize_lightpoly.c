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
	}*/
}

static void fill_point_tri_bot(t_lightpoly poly, t_world *world)
{
	float			step[2];
	int				x;
	int				y;
	float			delta;

	delta = 1.0f / ((float)(poly.tex_coord[1].y - poly.tex_coord[0].y));
	step[0] = (poly.tex_coord[0].x - poly.tex_coord[1].x) * delta;
	step[1] = (poly.tex_coord[0].x - poly.tex_coord[2].x) * delta;
	y = poly.tex_coord[1].y;
	while (y >= poly.tex_coord[0].y)
	{
		x = poly.tex_coord[1].x + (step[0] * (float)(poly.tex_coord[1].y - y));
		int ax =  poly.tex_coord[2].x + (step[1] * (float)(poly.tex_coord[1].y - y));
		while(x <= ax)
		{
			
			//sample_img(l, x, y, triangle);
			x++;
		}
		y--;
	}
}

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


void	rasterize_lightpoly(t_lightpoly triangle, t_world *world)
{
	t_point			p2_split;
	t_vector2		uv_split;
	t_vector3		p3_split;
	t_point			p2_temp;
	t_vector2		uv_temp;
	t_vector3		p3_temp;
	t_point			*p2;
	t_vector3		*p3;
	float			lerp;

	p2 = triangle.tex_coord;
	p3 = triangle.world_coord;
	sort_polygon_tri(triangle.tex_coord, triangle.uv_coord, triangle.world_coord);
	lerp = ((float)p2[1].y - (float)p2[2].y) / ((float)p2[0].y - (float)p2[2].y);
	p2_split.x = p2[2].x + (lerp * ((float)p2[0].x - (float)p2[2].x));
	p2_split.y = p2[1].y;
	uv_split.x = ft_flerp(triangle.uv_coord[2].x, triangle.uv_coord[0].x, lerp);
	uv_split.y = ft_flerp(triangle.uv_coord[2].y, triangle.uv_coord[0].y, lerp);
	p3_split = vector3_lerp(p3[2], p3[0], lerp);
	if (p2_split.x < p2[1].x)
	{
		ft_swap(&p2[1], &p2_split, sizeof(t_point));
		ft_swap(&triangle.uv_coord[1], &uv_split, sizeof(t_vector2));
		ft_swap(&p3[1], &p3_split, sizeof(t_vector3));
	}
	p2_temp = p2[2];
	uv_temp = triangle.uv_coord[2];
	p3_temp = p3[2];
	p2[2] = p2_split;
	triangle.uv_coord[2] = uv_split;
	p3[2] = p3_split;
	if (p2[0].y != p2[1].y)
		//fill_point_tri_top(l, triangle);
	p2[0] = p2_temp;
	triangle.uv_coord[0] = uv_temp;
	p3[0] = p3_temp;
	if (p2[0].y != p2[1].y)
		fill_point_tri_bot(triangle, world);
}
