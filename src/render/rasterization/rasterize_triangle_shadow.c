#include "doomnukem.h"

static void sample_img(t_lighting *lighting, int x, int y, t_point_triangle poly)
{
    t_vector3       loc;
	uint32_t		light_amount;

	if (lighting->overdraw[y * lighting->map->size.x + x])
		return;
	else
		lighting->overdraw[y * lighting->map->size.x + x] = true;
	loc = texcoord_to_loc(poly.t, poly.p, vector2_add_xy((t_vector2){x, y}, 0.0f));
	light_amount = lighting->map->lightmap[y * (lighting->map->size.x) + x];
	light_amount = get_lighting_for_pixel(lighting, light_amount, loc);
	lighting->map->lightmap[y * (lighting->map->size.x) + x] = light_amount;
}

static void sample_pix(t_lighting *lighting, int ax, int y)
{
	ax = ft_clamp(ax, 0, lighting->map->size.x - 1);
	y = ft_clamp(y, 0, lighting->map->size.y - 1);
	lighting->map->lightmap[y * lighting->map->size.x + ax] = 255;
}

//>= 1.0f == counterclockwise, <= -1.0f == clockwise and 0.0f is collinear
static float clockwise(t_vector2 *p)
{
	return ((p[1].x - p[0].x) * (p[2].y - p[0].y) - (p[2].x - p[0].x) * (p[1].y - p[0].y));
}

static t_vector3	normal_calc(t_vector3 p[3])
{
	t_vector3 normal, line1, line2;

	line1 = vector3_sub(p[1], p[0]);
	line2 = vector3_sub(p[2], p[0]);
	normal = vector3_crossproduct(line1, line2);
	normal = vector3_normalise(normal);

	return (normal);
}

void	rasterize_light(t_point_triangle triangle, t_lighting *lighting)
{
	t_vector2 min;
	t_vector2 max;

	lighting->triangle_normal = normal_calc(triangle.t);
	sort_point_uv_tri(triangle.p, triangle.t);
	if (clockwise(triangle.p) >= -1.0f)
	{
		ft_swap(&triangle.p[0], &triangle.p[2], sizeof(t_vector2));
		ft_swap(&triangle.t[0], &triangle.t[2], sizeof(t_vector3));
	}
	min = triangle.p[0];
	max = triangle.p[0];
	for (int i = 1; i < 3; i++)
	{
		if (max.x < triangle.p[i].x)
			max.x = triangle.p[i].x;
		if (max.y < triangle.p[i].y)
			max.y = triangle.p[i].y;
		if (min.x > triangle.p[i].x)
			min.x = triangle.p[i].x;
		if (min.y > triangle.p[i].y)
			min.y = triangle.p[i].y;
	}
	min.x = floorf(min.x);
	min.y = floorf(min.y);
	max.x = ceilf(max.x);
	max.y = ceilf(max.y);
	t_vector2 offset;
	offset = vector2_sub(barycentric_coordinates(triangle.p, (t_vector2){0, 0}), barycentric_coordinates(triangle.p, (t_vector2){1, 1}));
	offset = vector2_abs(offset);
	offset = vector2_negative(offset);
	for (int y = min.y; y < max.y; y++)
	{
		for (int x = min.x; x < max.x; x++)
		{
			t_vector2 bary = barycentric_coordinates(triangle.p, (t_vector2){x, y});
			if (bary.x >= offset.x && bary.y >= offset.y && bary.x + bary.y <= 1.0f -(offset.x + offset.y))
			{
				sample_img(lighting, ft_clamp(x + 0.5f, 0, lighting->map->size.x - 1), ft_clamp(y + 0.5f, 0, lighting->map->size.y - 1), triangle);
			}
		}
	}
}

/*
void	rasterize_light(t_point_triangle triangle, t_lighting *lighting)
{
	t_vector2 min;
	t_vector2 max;

	//printf("%f\n", clockwise(triangle.p));
	t_vector2 center;
	center = vector2_div(vector2_add(vector2_add(triangle.p[0], triangle.p[1]), triangle.p[2]), 3.0f);
	t_vector2 *p = triangle.p;
	t_vector3 *t = triangle.t;
	int			s_x;
	int			s_j;
	t_vector2	temp_p;
	t_vector3	temp_t;

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (!less(p[s_j], p[s_j + 1], center))
			{
				temp_p = p[s_j];
				p[s_j] = p[s_j + 1];
				p[s_j + 1] = temp_p;

				temp_t = t[s_j];
				t[s_j] = t[s_j + 1];
				t[s_j + 1] = temp_t;
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
	min = triangle.p[0];
	max = triangle.p[0];
	for (int i = 1; i < 3; i++)
	{
		if (max.x < triangle.p[i].x)
			max.x = triangle.p[i].x;
		if (max.y < triangle.p[i].y)
			max.y = triangle.p[i].y;
		if (min.x > triangle.p[i].x)
			min.x = triangle.p[i].x;
		if (min.y > triangle.p[i].y)
			min.y = triangle.p[i].y;
	}
	//print_vector2(min);
	//print_vector2(max);
	printf("\n");
	min.x = floorf(min.x - 0.5f);
	min.y = floorf(min.y - 0.5f);
	max.x = ceilf(max.x + 0.5f);
	max.y = ceilf(max.y + 0.5f);
	t_vector2 offset;
	offset = vector2_sub(barycentric_coordinates(triangle.p, (t_vector2){0, 0}), barycentric_coordinates(triangle.p, (t_vector2){1, 1}));
	offset = vector2_abs(offset);
	offset = vector2_negative(offset);
	bool test = false;
	for (int y = min.y; y < max.y; y++)
	{
		for (int x = min.x; x < max.x; x++)
		{
			t_vector2 bary = barycentric_coordinates(triangle.p, (t_vector2){x, y});
			bary = vector2_abs(bary);
			if (bary.x >= offset.x && bary.y >= offset.y && bary.x + bary.y <= 1.0f -(offset.x + offset.y))
			{
				test = true;
				sample_pix(lighting, x, y);
			}
		}
	}
	if (test == false)
	{
		printf("%s %s\n\n", lighting->entity->object_name.str, lighting->entity->obj->name);
		for (int i = 0; i < 3; i++)
		{
			print_vector2(triangle.p[i]);
		}
	}
}
void	rasterize_light(t_point_triangle triangle, t_lighting *lighting)
{
	t_vector2 min;
	t_vector2 max;

	//printf("%f\n", clockwise(triangle.p));
	sort_point_uv_tri(triangle.p, triangle.t);
	if (clockwise(triangle.p) <= -1.09f)
	{
		ft_swap(&triangle.p[1], &triangle.p[2], sizeof(t_vector2));
		ft_swap(&triangle.t[1], &triangle.t[2], sizeof(t_vector3));
	}
	if (clockwise(triangle.p) == 0.0f)
	{
		for (int y = 0; y < lighting->map->size.y; y++)
		{
			for (int x = 0; x < lighting->map->size.x; x++)
			{
				lighting->map->lightmap[y * lighting->map->size.x + x] = 101;
			}
		}
		return;
	}
	min = triangle.p[0];
	max = triangle.p[0];
	for (int i = 0; i < 3; i++)
	{
		if (max.x < triangle.p[i].x)
			max.x = triangle.p[i].x;
		if (max.y < triangle.p[i].y)
			max.y = triangle.p[i].y;
		if (min.x > triangle.p[i].x)
			min.x = triangle.p[i].x;
		if (min.y > triangle.p[i].y)
			min.y = triangle.p[i].y;
	}
	//print_vector2(min);
	//print_vector2(max);
	printf("\n");
	min.x = floorf(min.x - 0.5f);
	min.y = floorf(min.y - 0.5f);
	max.x = ceilf(max.x + 0.5f);
	max.y = ceilf(max.y + 0.5f);
	t_vector2 offset;
	offset = vector2_sub(barycentric_coordinates(triangle.p, (t_vector2){0, 0}), barycentric_coordinates(triangle.p, (t_vector2){1, 1}));
	offset = vector2_abs(offset);
	offset = vector2_negative(offset);
	bool test = false;
	for (int y = min.y; y < max.y; y++)
	{
		for (int x = min.x; x < max.x; x++)
		{
			t_vector2 bary = barycentric_coordinates(triangle.p, (t_vector2){x, y});
			bary = vector2_abs(bary);
			if (bary.x >= offset.x && bary.y >= offset.y && bary.x + bary.y <= 1.0f -(offset.x + offset.y))
			{
				test = true;
				sample_pix(lighting, x, y);
			}
		}
	}
	if (test == false)
	{
		printf("%s %s\n\n", lighting->entity->object_name.str, lighting->entity->obj->name);
		for (int i = 0; i < 3; i++)
		{
			print_vector2(triangle.p[i]);
		}
	}
}
t_vector2 bary = barycentric_coordinates(triangle.p, (t_vector2){x, y});
			bary = vector2_abs(bary);
			if (bary.x >= offset.x && bary.y >= offset.y && bary.x + bary.y <= 1.0f -(offset.x + offset.y))
			{
				sample_pix(lighting, x, y);
			}
static void fill_point_tri_top(t_lighting *lighting, t_point_triangle triangle)
{
	t_vector2		*p;
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	steps = p[0].y - p[1].y;
	left = make_slope(p[1].x, p[0].x, steps);
	right = make_slope(p[2].x, p[0].x, steps);
	y = ceilf(p[2].y - 0.5f);
	endy = ceilf(p[0].y + 0.5f) + 1;
	//left.location = left.step * ((float)y + 0.5f - p[1].y) + left.location;
	//right.location = right.step * ((float)y + 0.5f - p[1].y) + right.location;
	while (y <= endy)
	{	
		left.location = left.step * ((float)y + 0.5f - p[1].y) + p[1].x;
		right.location = right.step * ((float)y + 0.5f - p[2].y) + p[2].x;
		int tempax = floorf(left.location - 0.5f);
		int tempbx = ceilf(right.location + 0.5f);

		scanline(tempax, tempbx, y, lighting, triangle);
		y++;
	}
}

static void fill_point_tri_bot(t_lighting *lighting, t_point_triangle triangle)
{
	t_vector2		*p;
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;		
	steps = p[1].y - p[0].y;
	left = make_slope(p[0].x, p[1].x, steps);
	right = make_slope(p[0].x, p[2].x, steps);
	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[2].y + 0.5f) + 1;
//	left.location = left.step * ((float)y + 0.5f - p[0].y) + left.location;
	//right.location = right.step * ((float)y + 0.5f - p[0].y) + right.location;
	while (y <= endy)
	{	
		left.location = left.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		right.location = right.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		int tempax = floorf(left.location - 0.5f);
		int tempbx = ceilf(right.location + 0.5f);

		scanline(tempax, tempbx, y, lighting, triangle);
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


inline static void scanline(int ax, int bx, int y, t_lighting *lighting, t_point_triangle poly)
{
	t_vector3	tex;
	t_vector2	bary;
	t_vector2	left;
	t_vector2	right;
	float		dist;
	float		steps;

	t_vector2 *p = poly.p;
	t_vector3 *t = poly.t;
	left = barycentric_coordinates(p, vector2_add_xy((t_vector2){ax, y}, 0.5f));
	right = barycentric_coordinates(p, vector2_add_xy((t_vector2){bx, y}, 0.5f));
	bary = left;
	//dist = ft_flerp(t[0].z, t[1].z, bary.x) + ((t[2].z - t[0].z) * bary.y);
	int start = ax;
	steps = bx - start;
	while(ax < bx)
	{
		int tempx = ft_flerp(t[0].x, t[1].x, bary.x) + ((t[2].x - t[0].x) * bary.y);
		int tempy = ft_flerp(t[0].y, t[1].y, bary.x) + ((t[2].y - t[0].y) * bary.y);
		lighting->map->data[y * (lighting->map->size.x) + ax] = CLR_RED;
		lighting->map->data[tempy * (lighting->map->size.x) + tempx] = CLR_RED;
		ax++;
		bary = vector2_lerp(left, right, (float)(ax - start) / steps);
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
*/