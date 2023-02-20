#include "doomnukem.h"
/*
	int e = 0;
	float temp = dir.x;
	if (fabsf(dir.y) > fabsf(temp))
	{
		temp = dir.y;
		e = 1;
	}
	if (fabsf(dir.z) > fabsf(temp))
	{
		temp = dir.z;
		e = 4;
	}
	if (temp < 0.0f)
		e += 3; */
			/*
	int j = 0;
	int index = 0;
	while (j < 6)
	{
		if (vector3_dot(dir, light->cubemap.cameras[j].lookdir) >= 0.70710f)
			break;
		j++;
	}
	j = j; */

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

uint32_t get_lighting_for_pixel(t_entitycache *cache, uint32_t clr, t_vector3 location)
{
	uint8_t			light_amount;
	int				i;
	int				found;
	t_entity		*ent;
	t_quaternion	q;

	i = 0;
	found = 0;
	light_amount = 0;
	while (found < cache->existing_entitycount
		&& i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status != es_free)
		{
			if	(ent->status == es_active && ent->component.type == COMP_LIGHT)
			{
                t_light *light = ent->component.data;
				t_vector3 dir;

				dir = vector3_add(ent->transform.position, light->origin);
				dir = vector3_sub(location, dir);
				dir = vector3_normalise(dir);
                int j = 0;
                float temp;
				temp = ft_maxf(fabsf(dir.x), ft_maxf(fabsf(dir.y), fabsf(dir.z)));
				if (temp == fabsf(dir.x))
				{
					if (dir.x > 0.0f)
						j = 0;
					else
						j = 2;
				}
				if (temp == fabsf(dir.y))
				{
					if (dir.y > 0.0f)
						j = 3;
					else
						j = 1;
				}
				if (temp == fabsf(dir.z))
				{
					if (dir.z > 0.0f)
						j = 4;
					else
						j = 5;
				}

                t_point     p;
                t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
                q = vector3_to_quaternion(location);
                q = quaternion_mul_matrix(light->cubemap.cameras[j].matview, q);
				float dist = vector3_fdist_to_plane(q.v, (t_vector3){.z = 1.0f}, (t_vector3){.z = 0.1f});
				if (dist < 0.0f && 0)
				{
					found++;
					i++;
					continue;
				}
                q = quaternion_mul_matrix(light->cubemap.cameras[j].matproj, q);
                q.v = vector3_div(q.v, q.w);
                q.v = vector3_negative(q.v);
                q.v = vector3_add(q.v, voffsetview);
                p.x = q.v.x * 640.0f;
                p.y = q.v.y * 640.0f;

                p.x = ft_clamp(p.x, 0, 1279);
                p.y = ft_clamp(p.y, 0, 1279);
	
				/*
                clr = INT_MAX;
				if (j == 0)
					clr = CLR_BLUE;
				if (j == 1)
					clr = CLR_GREEN;
				if (j == 2)
					clr = CLR_RED;
				if (j == 3)
					clr = CLR_TURQ;
				if (j == 4)
					clr = CLR_PRPL;
				if (j == 5)
					clr = CLR_DARKGRAY;
				*/
                int test = p.y * 1280 + p.x;
                if (1.0f / q.w >= light->cubemap.shadowmaps[j][test] * 0.98f && q.w < light->radius)
                {
					//uint8_t	light_amount = q.w;
					float dist = 1.0f - (q.w / light->radius);
					light_amount = ft_clamp(dist * 255 + light_amount, 0, 255);
                    //lighting->map->data[y * (lighting->map->size.x) + x] = clr;
					 //lighting->img[(y % (lighting->map->img_size.y)) * lighting->map->img_size.x + (x % (lighting->map->img_size.x))];
                }
                //lighting->map->data[y * (lighting->map->size.x) + x] = clr;
			}
			found++;
		}
		i++;
	}
	Uint32 alpha = clr & 0xFF000000;
	Uint32 red = ((clr & 0x00FF0000) * light_amount) >> 8;
	Uint32 green = ((clr & 0x0000FF00) * light_amount) >> 8;
	Uint32 blue = ((clr & 0x000000FF) * light_amount) >> 8;
	clr = alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF);
	return(clr);
}

static void sample_img(t_lighting *lighting, int x, int y, t_triangle_polygon poly)
{
    t_vector3       loc;
    t_quaternion    q;
	uint32_t		clr;

    loc = texcoord_to_loc(poly.p3, poly.uv, (t_vector2){(float)x, (float)y});
	clr = lighting->img[(y % (lighting->map->img_size.y)) * lighting->map->img_size.x + (x % (lighting->map->img_size.x))];
	lighting->map->data[y * (lighting->map->size.x) + x] = get_lighting_for_pixel(&lighting->world->entitycache, clr, loc);
}

static void fill_point_tri_bot(t_lighting *l, t_triangle_polygon triangle)
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

static void fill_point_tri_top(t_lighting *l, t_triangle_polygon triangle)
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


void	rasterize_light(t_triangle_polygon triangle, t_lighting *lighting)
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
		fill_point_tri_top(lighting, triangle);
	p2[0] = p2_temp;
	triangle.uv[0] = uv_temp;
	p3[0] = p3_temp;
	if (p2[0].y != p2[1].y)
		fill_point_tri_bot(lighting, triangle);
}
