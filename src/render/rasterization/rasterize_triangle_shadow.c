#include "doomnukem.h"

static void sample_img(t_lighting *lighting, int x, int y, t_point_triangle poly)
{
    t_vector3       loc;
    t_quaternion    q;
	uint32_t		clr;
	uint32_t		light_amount;

	clr = lighting->img[(y % (lighting->map->img_size.y)) * lighting->map->img_size.x + (x % (lighting->map->img_size.x))];
	loc = texcoord_to_loc(poly.t, poly.p, vector2_add_xy((t_vector2){x, y}, 0.5f)); //adding
	light_amount = get_lighting_for_pixel(&lighting->world->entitycache, loc);
	clr = update_pixel_brightness(ft_clamp(light_amount, 0, 255), clr);
	lighting->map->data[y * (lighting->map->size.x) + x] = clr;
}

inline static void scanline(int ax, int bx, int y, t_lighting *lighting, t_point_triangle poly)
{
	while(ax < bx)
	{
		lighting->map->data[y * lighting->map->size.x + ax] = CLR_RED;
		//sample_img(lighting, ax, y, poly);
		ax++;
	}
}

static void fill_point_tri_top(t_lighting *lighting, t_point_triangle triangle)
{
	t_vector2		*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
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

static void fill_point_tri_bot(t_lighting *lighting, t_point_triangle triangle)
{
	t_vector2		*p;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
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


void	rasterize_light(t_point_triangle triangle, t_lighting *lighting)
{
	t_point_triangle	tris[2];
	int	res;

	res = triangle_to_flat(triangle, tris);
	if (res == 0)
		fill_point_tri_top(lighting, tris[0]);
	else if (res == 1)
		fill_point_tri_bot(lighting, tris[0]);
	else
	{
		fill_point_tri_top(lighting, tris[0]);
		fill_point_tri_bot(lighting, tris[1]);
	}
}

/*
static void fill_point_tri_top(t_lighting *lighting, t_point_triangle triangle)
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

static void fill_point_tri_bot(t_lighting *lighting, t_point_triangle triangle)
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
static void fill_point_tri_top(t_lighting *lighting, t_point_triangle triangle)
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

static void fill_point_tri_bot(t_lighting *lighting, t_point_triangle triangle)
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

static void fill_point_tri_top(t_lighting *lighting, t_point_triangle triangle)
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

static void fill_point_tri_bot(t_lighting *lighting, t_point_triangle triangle)
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

static void fill_point_tri_top(t_lighting *lighting, t_point_triangle triangle)
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

static void fill_point_tri_bot(t_lighting *lighting, t_point_triangle triangle)
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


static void fill_point_tri_top(t_lighting *lighting, t_point_triangle triangle)
{
	t_vector2		*p;
	t_vector3		*t;
	int				y;
	float			delta;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	delta = p[0].y - p[1].y;
	t_vector2 dv0 = vector2_div(vector2_sub(p[0], p[1]), delta);
	t_vector2 dv1 = vector2_div(vector2_sub(p[0], p[2]), delta);
	t_vector3 tdv0 = vector3_div(vector3_sub(t[0], t[1]), delta);
	t_vector3 tdv1 = vector3_div(vector3_sub(t[0], t[2]), delta);

	t_vector2 itedge0 = p[1];
	t_vector2 itedge1 = p[2];
	t_vector3 titedge0 = t[1];
	t_vector3 titedge1 = t[2];

	y = ceilf(p[1].y - 0.5f);
	endy = ceilf(p[0].y - 0.5f);

	itedge0 = vector2_add(itedge0, vector2_mul(dv0, (float)y + 0.5f - p[1].y));
	itedge1 = vector2_add(itedge1, vector2_mul(dv1, (float)y + 0.5f - p[1].y));
	titedge0 = vector3_add(titedge0, vector3_mul(tdv0, (float)y + 0.5f - p[1].y));
	titedge1 = vector3_add(titedge1, vector3_mul(tdv1, (float)y + 0.5f - p[1].y));

	while (y < endy)
	{
		int ax, bx;
		ax = ceilf(itedge0.x - 0.5f);
		bx = ceilf(itedge1.x - 0.5f);
		
		t_vector3 dtcline = vector3_div(vector3_sub(titedge1, titedge0), itedge1.x - itedge0.x);

		t_vector3 itcline = vector3_add(titedge0, vector3_mul(dtcline, (float)ax + 0.5f - titedge0.x));

		for (int x = ax; x < bx; x++)
		{
			int tempx = ft_clampf(itcline.x, 0.0f, (float)lighting->map->size.x - 1.0f);
			int tempy = ft_clampf(itcline.y, 0.0f, (float)lighting->map->size.y - 1.0f);
			lighting->map->data[y * lighting->map->size.x + x] = CLR_RED;
			lighting->map->data[tempy * lighting->map->size.x + tempx] = CLR_RED;
			itcline = vector3_add(itcline, dtcline);
		}
		itedge0 = vector2_add(itedge0, dv0);
		itedge1 = vector2_add(itedge1, dv1);
		titedge0 = vector3_add(titedge0, tdv0);
		titedge1 = vector3_add(titedge1, tdv1);
		y++;
	}
}

static void fill_point_tri_bot(t_lighting *lighting, t_point_triangle triangle)
{
	t_vector2		*p;
	t_vector3		*t;
	int				y;
	float			delta;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	delta = p[2].y - p[0].y;
	t_vector2 dv0 = vector2_div(vector2_sub(p[1], p[0]), delta);
	t_vector2 dv1 = vector2_div(vector2_sub(p[2], p[0]), delta);
	t_vector3 tdv0 = vector3_div(vector3_sub(t[1], t[0]), delta);
	t_vector3 tdv1 = vector3_div(vector3_sub(t[2], t[0]), delta);

	t_vector2 itedge0 = p[0];
	t_vector2 itedge1 = p[0];
	t_vector3 titedge0 = t[0];
	t_vector3 titedge1 = t[0];

	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[2].y - 0.5f);

	itedge0 = vector2_add(itedge0, vector2_mul(dv0, (float)y + 0.5f - p[0].y));
	itedge1 = vector2_add(itedge1, vector2_mul(dv1, (float)y + 0.5f - p[0].y));
	titedge0 = vector3_add(titedge0, vector3_mul(tdv0, (float)y + 0.5f - p[0].y));
	titedge1 = vector3_add(titedge1, vector3_mul(tdv1, (float)y + 0.5f - p[0].y));

	while (y < endy)
	{
		int ax, bx;
		ax = ceilf(itedge0.x - 0.5f);
		bx = ceilf(itedge1.x - 0.5f);
		
		t_vector3 dtcline = vector3_div(vector3_sub(titedge1, titedge0), itedge1.x - itedge0.x);

		t_vector3 itcline = vector3_add(titedge0, vector3_mul(dtcline, (float)ax + 0.5f - titedge0.x));

		for (int x = ax; x < bx; x++)
		{
			int tempx = ft_clampf(itcline.x, 0.0f, (float)lighting->map->size.x - 1.0f);
			int tempy = ft_clampf(itcline.y, 0.0f, (float)lighting->map->size.y - 1.0f);
			lighting->map->data[y * lighting->map->size.x + x] = CLR_RED;
			lighting->map->data[tempy * lighting->map->size.x + tempx] = CLR_RED;
			itcline = vector3_add(itcline, dtcline);
		}
		itedge0 = vector2_add(itedge0, dv0);
		itedge1 = vector2_add(itedge1, dv1);
		titedge0 = vector3_add(titedge0, tdv0);
		titedge1 = vector3_add(titedge1, tdv1);
		y++;
	}
}

*/