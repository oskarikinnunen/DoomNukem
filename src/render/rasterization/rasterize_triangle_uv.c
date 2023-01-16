#include "doomnukem.h"

static t_vector3 texcoord_to_loc(t_vector3 *v, t_vector2 *t, t_vector2 p)
{
	float       i;
	float       s;
	float       delta;

	t_vector3   r;
	i = 1.0f / ((t[1].x - t[0].x) * (t[2].y - t[0].y) - (t[1].y - t[0].y) * (t[2].x - t[0].x));
	s = i * ((t[2].y - t[0].y) * (p.x - t[0].x) - (t[2].x - t[0].x) * (p.y - t[0].y));
	delta = i * (-(t[1].y - t[0].y) * (p.x - t[0].x) + (t[1].x - t[0].x) * (p.y - t[0].y));
	r.x = v[0].x + s * (v[1].x - v[0].x) + delta * (v[2].x - v[0].x);
	r.y = v[0].y + s * (v[1].y - v[0].y) + delta * (v[2].y - v[0].y);
	r.z = v[0].z + s * (v[1].z - v[0].z) + delta * (v[2].z - v[0].z);
	return r;
}

//	abc = tri to check against
//	w = distance of ray hit
// disable inverse normal hitting by removing fabs

static bool intersect_triangle(t_ray r, t_texture *t, t_vector3 a, t_vector3 b, t_vector3 c)
{
	t_vector3   e1, e2, n;

	e1 = vector3_sub(b, a);
	e2 = vector3_sub(c, a);
	n = vector3_crossproduct(e1, e2);

	float det, invdet;
	det = -vector3_dot(r.dir, n);
	invdet = 1.0f/det;
	t_vector3 ao = vector3_sub(r.origin, a);
	t_vector3 dao = vector3_crossproduct(ao, r.dir);
	t->u = vector3_dot(e2, dao) * invdet;
	t->v = -vector3_dot(e1, dao) * invdet;
	t->w = vector3_dot(ao, n) * invdet;
	return(fabs(det) >= 1e-6 && t->w >= -0.01f && t->u >= 0.0f && t->v >= 0.0f && (t->u + t->v) <= 1.0f);
}

static void sample_img(t_lighting l, int x, int y, t_triangle_polygon t)
{
	t_ray		ray;
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
		if (dist <= l.pointlight->radius /*&& l.drawnbuff[x + l.lightmap->size.x * y] == false*/)
		{
			dist = 1.0f - (dist / l.pointlight->radius);
			//l.drawnbuff[x + l.lightmap->size.x * y] = true;
			l.lightmap->data[x + l.lightmap->size.x * y] = ft_clamp((dist * 255) + l.lightmap->data[x + l.lightmap->size.x * y], 0, 255);
		}
		return;
	}
	ray.dir = vector3_sub(l.pointlight->origin, ray.origin);
	bool ol = false;
	for (int o = 0; o < l.entities_count; o++)
	{
		/*if (l.entities[o]->id == l.entity_id)
			continue ;*/
		if (l.entities[o]->hidden
			|| (l.pointlight->ignoreself && l.entities[o]->id == l.entity_id))
			continue;
		for (int p = 0; p < l.entities[o]->obj->face_count; p++)
		{
			if (intersect_triangle(ray, &temp_t, l.triangles[o][p].p3[0], l.triangles[o][p].p3[1], l.triangles[o][p].p3[2]) == true)
			{
				if (temp_t.w < 1.0f && temp_t.w > 0.01f)
				{
					ol = true;
						break;
				}
			}
		}
		if (ol == true)
			break;
	}
	if (ol == false /*&& l.drawnbuff[x + l.lightmap->size.x * y] == false*/)
	{
		float dist = vector3_dist(ray.origin, l.pointlight->origin);
		if (dist <= l.pointlight->radius)
		{
			dist = 1.0f - (dist / l.pointlight->radius);
			//l.drawnbuff[x + l.lightmap->size.x * y] = true;
			l.lightmap->data[x + l.lightmap->size.x * y] = ft_clamp((dist * 255) + l.lightmap->data[x + l.lightmap->size.x * y], 0, 255);
		}
	}
}

static void fill_point_tri_bot(t_lighting l, t_triangle_polygon triangle)
{
	t_point			*p;
	float			step[2];
	int				x;
	int				y;
	float			delta;

	p = triangle.p2;
	delta = 1.0f / ((float)(p[1].y - p[0].y));
	step[0] = (p[0].x - p[1].x) * delta;
	step[1] = (p[0].x - p[2].x) * delta;
	y = p[1].y;
	while (y >= p[0].y)
	{
		x = p[1].x + (step[0] * (float)(p[1].y - y));
		int ax =  p[2].x + (step[1] * (float)(p[1].y - y));
		while(x <= ax)
		{
			sample_img(l, x, y, triangle);
			x++;
		}
		y--;
	}
}


static void fill_point_tri_top(t_lighting l, t_triangle_polygon triangle)
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
}

/*
creates two triangles from the given triangle one flat top and one flat bottom.
both triangles are then assigned to t_point p[3] array and passed onto fill_tri_bot/top functions.
p[0] is always the pointy head of the triangle p[1] and p[2] are flat points where, p[1] x is smaller than p[2]
*/

void	render_triangle_uv(t_lighting l, t_triangle_polygon triangle)
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

	p2 = triangle.p2;
	p3 = triangle.p3;
	sort_polygon_tri(triangle.p2, triangle.uv, triangle.p3);
	lerp = ((float)p2[1].y - (float)p2[2].y) / ((float)p2[0].y - (float)p2[2].y);
	p2_split.x = p2[2].x + (lerp * ((float)p2[0].x - (float)p2[2].x));
	p2_split.y = p2[1].y;
	uv_split.x = ft_flerp(triangle.uv[2].x, triangle.uv[0].x, lerp);
	uv_split.y = ft_flerp(triangle.uv[2].y, triangle.uv[0].y, lerp);
	p3_split = vector3_lerp(p3[2], p3[0], lerp);

	if (p2_split.x < p2[1].x)
	{
		ft_swap(&p2[1], &p2_split, sizeof(t_point));
		ft_swap(&triangle.uv[1], &uv_split, sizeof(t_vector2));
		ft_swap(&p3[1], &p3_split, sizeof(t_vector3));
	}
	p2_temp = p2[2];
	uv_temp = triangle.uv[2];
	p3_temp = p3[2];
	p2[2] = p2_split;
	triangle.uv[2] = uv_split;
	p3[2] = p3_split;
	if (p2[0].y != p2[1].y)
		fill_point_tri_top(l, triangle);
	p2[0] = p2_temp;
	triangle.uv[0] = uv_temp;
	p3[0] = p3_temp;
	if (p2[0].y != p2[1].y)
		fill_point_tri_bot(l, triangle);
}
