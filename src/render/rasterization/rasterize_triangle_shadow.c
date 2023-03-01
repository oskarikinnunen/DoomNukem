#include "doomnukem.h"

static void sample_img(t_lighting *lighting, int x, int y, t_triangle_polygon poly)
{
    t_vector3       loc;
    t_quaternion    q;
	uint32_t		clr;
	uint32_t		light_amount;

	clr = lighting->img[(y % (lighting->map->img_size.y)) * lighting->map->img_size.x + (x % (lighting->map->img_size.x))];
	loc = texcoord_to_loc(poly.p3, poly.p2, vector2_add_xy((t_vector2){x, y}, 0.5f));
	light_amount = get_lighting_for_pixel(&lighting->world->entitycache, loc);
	clr = update_pixel_brightness(ft_clamp(light_amount, 0, 255), clr);
	lighting->map->data[y * (lighting->map->size.x) + x] = clr;
}

inline static void scanline(int ax, int bx, int y, t_lighting *lighting, t_triangle_polygon poly)
{
	t_vector2	tex;
	t_vector2	bary;
	t_vector2	left;
	t_vector2	right;
	float		steps;
	t_vector2	*p;
	t_vector2	*t;

	p = poly.p2;
	t = poly.uv;
	t_vector2 temp[3];
	temp[0].x = 0;
	temp[0].y = 0;
	temp[1].x = lighting->map->size.x;
	temp[1].y = 0;
	temp[2].x = lighting->map->size.x;
	temp[2].y = lighting->map->size.x;
	left = barycentric_coordinates(p, vector2_add_xy((t_vector2){ax, y}, 0.5f));
	right = barycentric_coordinates(p, vector2_add_xy((t_vector2){bx, y}, 0.5f));
	bary = left;
	int start = ax;
	steps = bx - start;
	while(ax < bx)
	{
		tex.x = ft_flerp(t[0].x, t[1].x, bary.x) + ((t[2].x - t[0].x) * bary.y);
		tex.y = ft_flerp(t[0].y, t[1].y, bary.x) + ((t[2].y - t[0].y) * bary.y);
		tex.x = ceilf(tex.x * (float)lighting->map->img_size.x);
		tex.y = ceilf(tex.y * (float)lighting->map->img_size.y);
		//tex = vector2_add_xy(tex, 0.5f);
		lighting->map->data[(int)(tex.y * (float)(lighting->map->size.x) + tex.x)] = CLR_RED;
		ax++;
		bary = vector2_lerp(left, right, (float)(ax - start) / steps);
		//bary = barycentric_coordinates(p, vector2_add_xy((t_vector2){ax, y}, 0.0f));
	}
}


static void fill_point_tri_top(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2		*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p2;
	left.step = (p[0].x - p[1].x) / (p[0].y - p[1].y);
	right.step = (p[0].x - p[2].x) / (p[0].y - p[2].y);
	y = ceilf(p[2].y - 0.5f);
	endy = ceilf(p[0].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[1].y) + p[1].x;
		right.location = right.step * ((float)y + 0.5f - p[2].y) + p[2].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, lighting, triangle);
		y++;
	}
}

static void fill_point_tri_bot(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2		*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p2;
	left.step = (p[1].x - p[0].x) / (p[1].y - p[0].y);
	right.step = (p[2].x - p[0].x) / (p[2].y - p[0].y);
	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[2].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		right.location = right.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, lighting, triangle);
		y++;
	}
}

void	rasterize_light(t_triangle_polygon triangle, t_lighting *lighting)
{
	t_vector2	p2_split;
	t_vector3	p3_split;
	t_vector2	uv_split;
	t_vector2	p2_temp;
	t_vector3	p3_temp;
	t_vector2	uv_temp;
	t_vector2	*p2;
	t_vector3	*p3;
	t_vector2	*uv;

	p2 = triangle.p2;
	p3 = triangle.p3;
	uv = triangle.uv;
	sort_polygon_tri(triangle.p2, triangle.uv, triangle.p3);
	if (p2[1].y == p2[2].y)
	{
		printf("1\n");
		if (p2[1].x > p2[2].x)
		{
			ft_swap(&p2[1], &p2[2], sizeof(t_vector2));
			ft_swap(&p3[1], &p3[2], sizeof(t_vector3));
			ft_swap(&uv[1], &uv[2], sizeof(t_vector2));
		}
		fill_point_tri_top(lighting, triangle);
	}
	else if (p2[0].y == p2[1].y)
	{
		printf("2\n");
		ft_swap(&p2[0], &p2[2], sizeof(t_vector2));
		ft_swap(&p3[0], &p3[2], sizeof(t_vector3));
		ft_swap(&uv[0], &uv[2], sizeof(t_vector2));
		if (p2[1].x > p2[2].x)
		{
			ft_swap(&p2[1], &p2[2], sizeof(t_vector2));
			ft_swap(&p3[1], &p3[2], sizeof(t_vector3));
			ft_swap(&uv[1], &uv[2], sizeof(t_vector2));
		}
		fill_point_tri_bot(lighting, triangle);
	}
	else
	{
		printf("3\n");
		float delta = (p2[1].y - p2[2].y) / (p2[0].y - p2[2].y);

		p2_split.x = ft_flerp(p2[2].x, p2[0].x, delta);
		p2_split.y = p2[1].y;
		p3_split = vector3_lerp(p3[2], p3[0], delta);
		uv_split = vector2_lerp(uv[2], uv[0], delta);
		if (p2_split.x < p2[1].x)
		{
			ft_swap(&p2[1], &p2_split, sizeof(t_vector2));
			ft_swap(&p3[1], &p3_split, sizeof(t_vector3));
			ft_swap(&uv[1], &uv_split, sizeof(t_vector2));
		}
		p2_temp = p2[2];
		p3_temp = p3[2];
		uv_temp = uv[2];
		p2[2] = p2_split;
		p3[2] = p3_split;
		uv[2] = uv_split;
		if (p2[0].y != p2[1].y && p2[1].x != p2[2].x)
			fill_point_tri_top(lighting, triangle);
		p2[0] = p2_temp;
		p3[0] = p3_temp;
		uv[0] = uv_temp;
		if (p2[0].y != p2[1].y && p2[1].x != p2[2].x)
			fill_point_tri_bot(lighting, triangle);
	}
}

/*
static void fill_point_tri_top(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2		*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p2;
	left.step = (p[0].x - p[1].x) / (p[0].y - p[1].y);
	right.step = (p[0].x - p[2].x) / (p[0].y - p[2].y);
	y = ceilf(p[2].y - 0.5f);
	endy = ceilf(p[0].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[1].y) + p[1].x;
		right.location = right.step * ((float)y + 0.5f - p[2].y) + p[2].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, lighting, triangle);
		y++;
	}
}

static void fill_point_tri_bot(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2		*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p2;
	left.step = (p[1].x - p[0].x) / (p[1].y - p[0].y);
	right.step = (p[2].x - p[0].x) / (p[2].y - p[0].y);
	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[2].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		right.location = right.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, lighting, triangle);
		y++;
	}
}
static void fill_point_tri_top(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2		*p;
	t_vector2		bary;

	p = triangle.p2;
	int minx = floorf(p[1].x);
	if (p[0].x < minx)
		minx = floorf(p[0].x);
	int maxx = ceilf(p[2].x);
	if (p[0].x > maxx)
		maxx = ceilf(p[0].x);
	int y = floorf(p[1].y);
	while (y <= ceilf(p[0].y))
	{
		for (int x = minx; x <= maxx; x++)
		{
			bary = barycentric_coordinates(p, (t_vector2){x, y});
			if (bary.x >= -0.05f && bary.y >= -0.05f && bary.x + bary.y <= 1.05f)
				lighting->map->data[ft_clamp(y * lighting->map->size.x + x, 0, lighting->map->size.x * (lighting->map->size.y) - 1)] = CLR_BLUE;
		}
		y++;
	}
}

static void fill_point_tri_bot(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2		*p;
	t_vector2		bary;

	p = triangle.p2;
	int minx = floorf(p[1].x);
	if (p[0].x < minx)
		minx = floorf(p[0].x);
	int maxx = ceilf(p[2].x);
	if (p[0].x > maxx)
		maxx = ceilf(p[0].x);
	int y = floorf(p[0].y);
	while (y <= ceilf(p[1].y))
	{
		for (int x = minx; x <= maxx; x++)
		{
			bary = barycentric_coordinates(p, (t_vector2){x, y});
			if (bary.x >= -0.05f && bary.y >= -0.05f && bary.x + bary.y <= 1.05f)
				lighting->map->data[ft_clamp(y * lighting->map->size.x + x, 0, lighting->map->size.x * (lighting->map->size.y) - 1)] = CLR_RED;
		}
		y++;
	}
}

static void fill_point_tri_top(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2		*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p2;
	left.step = (p[0].x - p[1].x) / (p[0].y - p[1].y);
	right.step = (p[0].x - p[2].x) / (p[0].y - p[2].y);
	y = ceilf(p[2].y - 0.5f);
	endy = ceilf(p[0].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[1].y) + p[1].x;
		right.location = right.step * ((float)y + 0.5f - p[2].y) + p[2].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, lighting, triangle);
		y++;
	}
}

static void fill_point_tri_bot(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2		*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p2;
	left.step = (p[1].x - p[0].x) / (p[1].y - p[0].y);
	right.step = (p[2].x - p[0].x) / (p[2].y - p[0].y);
	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[2].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		right.location = right.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, lighting, triangle);
		y++;
	}
}

static void fill_point_tri_top(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2		*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p2;
	left.step = (p[0].x - p[1].x) / (p[0].y - p[1].y);
	right.step = (p[0].x - p[2].x) / (p[0].y - p[2].y);
	delta = 1.0f/(p[0].y - p[1].y);
	left = make_slope(p[1].x, p[0].x, delta);
	right = make_slope(p[2].x, p[0].x, delta);
	y = ceilf(p[1].y - 0.5f);
	endy = ceilf(p[0].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[1].y) + p[1].x;
		right.location = right.step * ((float)y + 0.5f - p[2].y) + p[2].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, lighting, triangle);
		y++;
	}
}

static void fill_point_tri_bot(t_lighting *lighting, t_triangle_polygon triangle)
{
	t_vector2		*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p2;
	left.step = (p[1].x - p[0].x) / (p[1].y - p[0].y);
	right.step = (p[2].x - p[0].x) / (p[2].y - p[0].y);
	delta = 1.0f/(p[1].y - p[0].y);
	left = make_slope(p[0].x, p[1].x, delta);
	right = make_slope(p[0].x, p[2].x, delta);
	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[1].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		right.location = right.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, lighting, triangle);
		y++;
	}
}

*/