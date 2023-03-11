#include "doomnukem.h"

#define FOG 0.0025f

inline static uint32_t sample_img(t_render *render, uint32_t xsample, uint32_t ysample)
{
	//xsample = ft_clamp(xsample, 0, render->map.size.x - 1);
	//ysample = ft_clamp(ysample, 0, render->map.size.y - 1);
	ysample = ft_min(ysample, render->map.size.y - 1);
	return(render->map.data[ysample * render->map.size.x + xsample]);
}

inline static void scanline(int start, int end, int y, t_point_triangle triangle, t_sdlcontext *sdl)
{
	t_vector3	tex;
	float		dist;
	t_stepv3	slope;
	int			x;

	slope = make_uv_slopev3(start, end, y, triangle);
	x = start;
	while(x < end)
	{
		float test = x - start;
		tex.z = slope.location.z + test * slope.step.z;
		if (tex.z > sdl->zbuffer[x + y * sdl->window_w])
		{
			tex.x = slope.location.x + test * slope.step.x;
			tex.y = slope.location.y + test * slope.step.y;
			sdl->zbuffer[x + y * sdl->window_w] = tex.z;
			((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] =
				sample_img(&sdl->render, tex.x / tex.z, tex.y / tex.z);
		}
		x++;
	}
	//render_bitmask_row(start, end, 1.0f / slope.location.z, 1.0f / tex.z, y, sdl);
}


inline static void render_flat_top_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2			*p;
	t_vector3		*t;
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	steps = p[0].y - p[1].y;
	left = make_slope(p[1].x, p[0].x, steps);
	right = make_slope(p[2].x, p[0].x, steps);
	y = ceilf(p[2].y - 0.5f);
	endy = ceilf(p[0].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[1].y) + p[1].x;
		right.location = right.step * ((float)y + 0.5f - p[2].y) + p[2].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, triangle, sdl);
		y++;
	}
}

inline static void render_flat_bot_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2			*p;
	t_vector3		*t;
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	steps = p[1].y - p[0].y;
	left = make_slope(p[0].x, p[1].x, steps);
	right = make_slope(p[0].x, p[2].x, steps);
	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[2].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		right.location = right.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, triangle, sdl);
		y++;
	}
}

void	render_triangle_lit(t_sdlcontext *sdl, t_render *render, int index)
{
	t_point_triangle	triangle;
	t_point_triangle	tris[2];
	int	res;

	triangle = render->screenspace_ptris[index];
	if (sdl->ps1_tri_div > 1)
		triangle = ps1(triangle, sdl->ps1_tri_div);
	res = triangle_to_flat(triangle, tris);
	if (res == 0)
		render_flat_top_tri(sdl, tris[0]);
	else if (res == 1)
		render_flat_bot_tri(sdl, tris[0]);
	else
	{
		render_flat_top_tri(sdl, tris[0]);
		render_flat_bot_tri(sdl, tris[1]);
	}
}

