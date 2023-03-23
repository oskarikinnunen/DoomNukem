#include "doomnukem.h"

uint32_t	update_pixel_brightness(uint32_t light, uint32_t clr)
{
   // light = CLR_BLUE;
    uint32_t alpha = clr & 0xFF000000;
    uint32_t red = ((clr & 0x00FF0000) * (uint8_t)((light & 0xFF0000) >> 16)) >> 8;
    uint32_t green = ((clr & 0x0000FF00) * (uint8_t)((light & 0xFF00) >> 8)) >> 8;
    uint32_t blue = ((clr & 0x000000FF) * (uint8_t)((light & 0xFF))) >> 8;

    clr = alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF);
    return(clr);
}

inline t_step	make_slope(float start, float end, float steps)
{
	t_step temp;

	temp.location = start;
	temp.step = (end - start) / steps;
	return(temp);
}

inline t_stepv3 make_uv_slopev3(int start, int end, int y, t_screen_triangle triangle)
{
	t_stepv3	slope;
	t_vector2	left;
	t_vector2	right;

	left = barycentric_coordinates(triangle.p, (t_vector2){start, y});
	right = barycentric_coordinates(triangle.p, (t_vector2){end, y});
	slope.location = get_vector3_from_barycentric(triangle.t, left);
	slope.step = get_vector3_from_barycentric(triangle.t, right);
	slope.step = vector3_div(vector3_sub(slope.step, slope.location), end - start);
	return(slope);
}

t_vector3 texcoord_to_loc(t_vector3 ws[3], t_vector2 uv[3], t_vector2 p)
{
	float       i;
	float       s;
	float       delta;

	//t_vector2 bary = barycentric_coordinates(poly.p, (t_vector2){x, y});
	//loc = vector3_add(vector3_lerp(poly.t[0], poly.t[1], bary.x), vector3_mul(vector3_sub(poly.t[2], poly.t[0]), bary.y));
	t_vector3   r;
	i = 1.0f / ((uv[1].x - uv[0].x) * (uv[2].y - uv[0].y) - (uv[1].y - uv[0].y) * (uv[2].x - uv[0].x));
	s = i * ((uv[2].y - uv[0].y) * (p.x - uv[0].x) - (uv[2].x - uv[0].x) * (p.y - uv[0].y));
	delta = i * (-(uv[1].y - uv[0].y) * (p.x - uv[0].x) + (uv[1].x - uv[0].x) * (p.y - uv[0].y));
	r.x = ws[0].x + s * (ws[1].x - ws[0].x) + delta * (ws[2].x - ws[0].x);
	r.y = ws[0].y + s * (ws[1].y - ws[0].y) + delta * (ws[2].y - ws[0].y);
	r.z = ws[0].z + s * (ws[1].z - ws[0].z) + delta * (ws[2].z - ws[0].z);
	return r;
}

uint32_t	get_light_amount(float delta, uint32_t clr, uint32_t light_amount)
{
	uint8_t	channel1;
	uint8_t	channel2;
	uint8_t	channel3;
	uint8_t	channel4;

	channel1 = ft_clamp((clr & 0xFF) * delta + (light_amount & 0xFF), 0, 255);
	channel2 = ft_clamp(((clr >> 8) & 0xFF) * \
	delta + ((light_amount >> 8) & 0xFF), 0, 255);
	channel3 = ft_clamp(((clr >> 16) & 0xFF) * \
	delta + ((light_amount >> 16) & 0xFF), 0, 255);
	channel4 = ft_clamp(((clr >> 24) & 0xFF) + \
	((light_amount >> 24) & 0xFF), 0, 255);
	light_amount = \
	channel1 | (channel2 << 8) | (channel3 << 16) | (channel4 << 24);
	return (light_amount);
}
