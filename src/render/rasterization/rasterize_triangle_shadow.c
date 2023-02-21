#include "doomnukem.h"

static void sample_img(t_lighting *lighting, int x, int y, t_triangle_polygon poly)
{
    t_vector3       loc;
    t_quaternion    q;
	uint32_t		clr;

	clr = lighting->img[(y % (lighting->map->img_size.y)) * lighting->map->img_size.x + (x % (lighting->map->img_size.x))];
    loc = texcoord_to_loc(poly.p3, poly.uv, (t_vector2){x, y});
	clr = get_lighting_for_pixel(&lighting->world->entitycache, clr, loc);
	lighting->map->data[y * (lighting->map->size.x) + x] = clr;
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
