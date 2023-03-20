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
				sample_img(lighting, ft_clamp(x, 0, lighting->map->size.x - 1), ft_clamp(y, 0, lighting->map->size.y - 1), triangle);
			}
		}
	}
}
