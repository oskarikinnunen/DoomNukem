#include "doomnukem.h"

static void sample_img(t_lighting *lighting, int x, int y, t_triangle_polygon poly)
{
    t_vector3       loc;
    t_quaternion    q;
	uint32_t		clr;
	uint32_t		light_amount;

	clr = lighting->img[(y % (lighting->map->img_size.y)) * lighting->map->img_size.x + (x % (lighting->map->img_size.x))];
	loc = texcoord_to_loc(poly.p3, poly.uv, (t_vector2){x, y});
	light_amount = get_lighting_for_pixel(&lighting->world->entitycache, loc);
	clr = update_pixel_brightness(ft_clamp(light_amount, 0, 255), clr);
	lighting->map->data[y * lighting->map->size.x + x] = clr;
}

inline static void scanline(int ax, int bx, int y, t_lighting *lighting, t_triangle_polygon poly)
{
	t_vector2	*p;
	t_vector2	bary;
	t_vector2	left;
	t_vector2	right;
	float		steps;

	p = poly.p2;
	ax = ft_clamp(ax, 0, lighting->map->size.x - 1);
	bx = ft_clamp(bx, 0, lighting->map->size.x - 1);
	y = ft_clamp(y, 0, lighting->map->size.y - 1);
	left = barycentric_coordinates(p, (t_vector2){ax, y});
	right = barycentric_coordinates(p, (t_vector2){bx, y});
	bary = left;
	int start = ax;
	steps = bx - start;
	while(ax <= bx)
	{
		sample_img(lighting, ax, y, poly);
		ax++;
		bary = vector2_lerp(left, right, (float)(ax - start) / steps);
	}
}

static void fill_point_tri_bot(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2			*p;
	t_texture		*t;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;

	p = triangle.p2;
	delta = 1.0f / (float)(p[1].y - p[0].y);
	left = make_slope(p[0].x, p[1].x, delta);
	right = make_slope(p[0].x, p[2].x, delta);
	y = p[0].y;
	while (y <= p[1].y)
	{
		scanline(left.location, right.location, y, lighting, triangle);
		left.location += left.step;
		right.location += right.step;
		y++;
	}
}

static void fill_point_tri_top(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2			*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;

	p = triangle.p2;
	delta = 1.0f / (float)(p[0].y - p[1].y);
	left = make_slope(p[1].x, p[0].x, delta);
	right = make_slope(p[2].x, p[0].x, delta);
	y = p[1].y;
	while (y <= p[0].y)
	{
		scanline(left.location, right.location, y, lighting, triangle);
		left.location += left.step;
		right.location += right.step;
		y++;
	}
}


//w1 = ((float)p[0].x * (float)(p[2].y - p[0].y) + (float)(y - p[0].y) * (float)(p[2].x - p[0].x) - (float)x * (float)(p[2].y - p[0].y)) / (float)((float)(p[1].y - p[0].y) * (float)(p[2].x - p[0].x) - (float)(p[1].x - p[0].x) * (float)(p[2].y - p[0].y));
//w2 = (float)(y - p[0].y - w1 * (float)(p[1].y - p[0].y)) / (float)(p[2].y - p[0].y);
static void	rasterize_triangle_bounds(t_triangle_polygon triangle, t_square bounds, t_lighting *lighting)
{
	t_vector2	*p;
	int x;
	int y;
	float w1, w2;
	t_vector2	temp;

	p = triangle.p2;
	y = bounds.min.y;
	while (y < bounds.max.y)
	{
		x = bounds.min.x;
		while (x < bounds.max.x)
		{
			lighting->map->data[y * (lighting->map->size.x) + x] = CLR_RED;
			x++;
		}
		y++;
	}
}

void	rasterize_light(t_triangle_polygon triangle, t_lighting *lighting)
{
	t_vector2			p2_split;
	t_vector2		uv_split;
	t_vector3		p3_split;
	t_vector2			p2_temp;
	t_vector2		uv_temp;
	t_vector3		p3_temp;
	t_vector2			*p2;
	t_vector3		*p3;
	float			lerp;

	p2 = triangle.p2;
	p3 = triangle.p3;
	sort_polygon_tri(triangle.p2, triangle.uv, triangle.p3);
	lerp = (float)(p2[1].y - p2[2].y) / (float)(p2[0].y - p2[2].y);
	p2_split.x = p2[2].x + (lerp * (float)(p2[0].x - p2[2].x));
	p2_split.y = p2[1].y;
	//p2_split.x = ft_flerp(p2[2].x, p2[0].x, lerp);
	//p2_split.y = ft_flerp(p2[2].y, p2[0].y, lerp);
	uv_split = vector2_lerp(triangle.uv[2], triangle.uv[0], lerp);
	p3_split = vector3_lerp(p3[2], p3[0], lerp);
	if (p2_split.x < p2[1].x)
	{
		ft_swap(&p2[1], &p2_split, sizeof(t_vector2));
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

/*
void	rasterize_light(t_triangle_polygon triangle, t_lighting *lighting)
{
	t_vector2			p2_split;
	t_vector2		uv_split;
	t_vector3		p3_split;
	t_vector2			p2_temp;
	t_vector2		uv_temp;
	t_vector3		p3_temp;
	t_vector2			*p2;
	t_vector3		*p3;
	float			lerp;

	p2 = triangle.p2;
	p3 = triangle.p3;
	sort_polygon_tri(triangle.p2, triangle.uv, triangle.p3);
	lerp = ((float)p2[1].y - (float)p2[2].y) / ((float)p2[0].y - (float)p2[2].y);
	//p2_split.x = p2[2].x + (lerp * ((float)p2[0].x - (float)p2[2].x));
	//p2_split.y = p2[1].y;
	p2_split.x = ft_flerp(p2[2].x, p2[0].x, lerp);
	p2_split.y = ft_flerp(p2[2].y, p2[0].y, lerp);
	uv_split = vector2_lerp(triangle.uv[2], triangle.uv[0], lerp);
	p3_split = vector3_lerp(p3[2], p3[0], lerp);
	if (p2_split.x < p2[1].x)
	{
		ft_swap(&p2[1], &p2_split, sizeof(t_vector2));
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
*/