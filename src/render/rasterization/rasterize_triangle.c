#include "doomnukem.h"

#define FOG 0.0025f

/*
inline static uint32_t sample_img(t_render *render, t_vector3 t)
{
	uint32_t	xsample;
	uint32_t	ysample;

	xsample = t.x;
	ysample = t.y;
	return(render->map.data[ysample * render->map.size.x + xsample]);
}

inline static void scanline(int ax, int bx, int y, t_vector2 *p, t_vector3 *t, t_sdlcontext *sdl)
{
	t_vector3	tex;
	t_vector2	bary;
	float		dist;

	//printf("ax %d bx %d\n", ax, bx);
	bary = barycentric_coordinates(p, (t_vector2){ax, y});
	dist = ft_flerp(t[0].z, t[1].z, bary.x) + ((t[2].z - t[0].z) * bary.y);
	while(ax <= bx)
	{
		tex.z = ft_flerp(t[0].z, t[1].z, bary.x) + ((t[2].z - t[0].z) * bary.y);
		if (tex.z > sdl->zbuffer[ax + y * sdl->window_w])
		{
			tex.x = ft_flerp(t[0].x, t[1].x, bary.x) + ((t[2].x - t[0].x) * bary.y);
			tex.y = ft_flerp(t[0].y, t[1].y, bary.x) + ((t[2].y - t[0].y) * bary.y);
			tex.x = tex.x / tex.z;
			tex.y = tex.y / tex.z;
			sdl->zbuffer[ax + y * sdl->window_w] = tex.z;
			((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] =
				sample_img(&sdl->render, tex);
		}
		ax++;
		bary = barycentric_coordinates(p, (t_vector2){ax, y});
	}
	render_bitmask_row(ax, bx, dist * 1000.0f, tex.z * 1000.0f, y, sdl);
}
if (xsample < 0 || ysample < 0 || ysample * render->map.size.x + xsample > render->map.size.x * render->map.size.y)
{
	printf("xsample ysample %d %d\n%d %d %d %d\n%f %f\n", xsample, ysample, render->img->size.x, render->img->size.y, render->map.size.x, render->map.size.y, t.x, t.y);
	exit(0);
}
*/

inline static uint32_t sample_img(t_render *render, uint32_t xsample, uint32_t ysample)
{
	return(render->map.data[ysample * render->map.size.x + xsample]);
}

inline static void scanline(int ax, int bx, int y, t_vector2*p, t_vector3 *t, t_sdlcontext *sdl)
{
	t_vector3	tex;
	t_vector2	bary;
	t_vector2	left;
	t_vector2	right;
	float		dist;
	float		steps;

	left = barycentric_coordinates(p, vector2_add_xy((t_vector2){ax, y}, 0.5f));
	right = barycentric_coordinates(p, vector2_add_xy((t_vector2){bx, y}, 0.5f));
	bary = left;
	dist = ft_flerp(t[0].z, t[1].z, bary.x) + ((t[2].z - t[0].z) * bary.y);
	int start = ax;
	steps = bx - start;
	while(ax < bx)
	{
		tex.z = ft_flerp(t[0].z, t[1].z, bary.x) + ((t[2].z - t[0].z) * bary.y);
		if (tex.z > sdl->zbuffer[ax + y * sdl->window_w])
		{
			tex.x = ft_flerp(t[0].x, t[1].x, bary.x) + ((t[2].x - t[0].x) * bary.y);
			tex.y = ft_flerp(t[0].y, t[1].y, bary.x) + ((t[2].y - t[0].y) * bary.y);
			tex.x = (tex.x / tex.z);
			tex.y = (tex.y / tex.z);
			sdl->zbuffer[ax + y * sdl->window_w] = tex.z;
			((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] =
				sample_img(&sdl->render, tex.x, tex.y);
		}
		ax++;
		bary = vector2_lerp(left, right, (float)(ax - start) / steps);
	}
	render_bitmask_row(start, bx, 1.0f / dist, 1.0f / tex.z, y, sdl);
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
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, p, t, sdl);
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
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, p, t, sdl);
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

