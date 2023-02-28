#include "doomnukem.h"

#define FOG 0.0025f

/*
inline static uint32_t sample_img(t_render *render, t_texture t)
{
	uint32_t	xsample;
	uint32_t	ysample;

	xsample = t.u;
	ysample = t.v;
	return(render->map.data[ysample * render->map.size.x + xsample]);
}

inline static void scanline(int ax, int bx, int y, t_point *p, t_texture *t, t_sdlcontext *sdl)
{
	t_texture	tex;
	t_vector2	bary;
	float		dist;

	//printf("ax %d bx %d\n", ax, bx);
	bary = barycentric_coordinates(p, (t_point){ax, y});
	dist = ft_flerp(t[0].w, t[1].w, bary.x) + ((t[2].w - t[0].w) * bary.y);
	while(ax <= bx)
	{
		tex.w = ft_flerp(t[0].w, t[1].w, bary.x) + ((t[2].w - t[0].w) * bary.y);
		if (tex.w > sdl->zbuffer[ax + y * sdl->window_w])
		{
			tex.u = ft_flerp(t[0].u, t[1].u, bary.x) + ((t[2].u - t[0].u) * bary.y);
			tex.v = ft_flerp(t[0].v, t[1].v, bary.x) + ((t[2].v - t[0].v) * bary.y);
			tex.u = tex.u / tex.w;
			tex.v = tex.v / tex.w;
			sdl->zbuffer[ax + y * sdl->window_w] = tex.w;
			((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] =
				sample_img(&sdl->render, tex);
		}
		ax++;
		bary = barycentric_coordinates(p, (t_point){ax, y});
	}
	render_bitmask_row(ax, bx, dist * 1000.0f, tex.w * 1000.0f, y, sdl);
}
if (xsample < 0 || ysample < 0 || ysample * render->map.size.x + xsample > render->map.size.x * render->map.size.y)
{
	printf("xsample ysample %d %d\n%d %d %d %d\n%f %f\n", xsample, ysample, render->img->size.x, render->img->size.y, render->map.size.x, render->map.size.y, t.u, t.v);
	exit(0);
}
*/
inline static uint32_t sample_img(t_render *render, t_texture t)
{
	uint32_t	xsample;
	uint32_t	ysample;

	xsample = t.u * render->img->size.x;
	ysample = t.v * render->img->size.y;
	xsample = ft_clamp(xsample, 0, render->map.size.x - 1);
	ysample = ft_clamp(ysample, 0, render->map.size.y - 1);
	return(render->map.data[ysample * render->map.size.x + xsample]);
}

inline static void scanline(int ax, int bx, int y, t_point *p, t_texture *t, t_sdlcontext *sdl)
{
	t_texture	tex;
	t_vector2	bary;
	t_vector2	left;
	t_vector2	right;
	float		dist;
	float		steps;

	left = barycentric_coordinates(p, (t_point){ax + 1, y});
	right = barycentric_coordinates(p, (t_point){bx, y});
	bary = left;
	dist = ft_flerp(t[0].w, t[1].w, bary.x) + ((t[2].w - t[0].w) * bary.y);
	int start = ax;
	steps = bx - start;
	while(ax <= bx)
	{
		tex.w = ft_flerp(t[0].w, t[1].w, bary.x) + ((t[2].w - t[0].w) * bary.y);
		if (tex.w > sdl->zbuffer[ax + y * sdl->window_w])
		{
			tex.u = ft_flerp(t[0].u, t[1].u, bary.x) + ((t[2].u - t[0].u) * bary.y);
			tex.v = ft_flerp(t[0].v, t[1].v, bary.x) + ((t[2].v - t[0].v) * bary.y);
			tex.u = (tex.u / tex.w);
			tex.v = (tex.v / tex.w);
			sdl->zbuffer[ax + y * sdl->window_w] = tex.w;
			((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] =
				sample_img(&sdl->render, tex);
		}
		ax++;
		bary = vector2_lerp(left, right, (float)(ax - start) / steps);
	}
	render_bitmask_row(start, bx, dist, tex.w, y, sdl);
}

static void fill_point_tri_bot(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_point			*p;
	t_texture		*t;
	int				y;
	float			delta;
	float			w1;
	float			w2;
	t_step			left;
	t_step			right;

	p = triangle.p;
	t = triangle.t;
	delta = 1.0f / (float)(p[1].y - p[0].y);
	left = make_slope(p[0].x, p[1].x, delta);
	right = make_slope(p[0].x, p[2].x, delta);
	y = p[0].y;
	while (y < p[1].y)
	{
		scanline(left.location, right.location, y, p, t, sdl);
		left.location += left.step;
		right.location += right.step;
		y++;
	}
}

static void fill_point_tri_top(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_point			*p;
	t_texture		*t;
	int				y;
	float			delta;
	float			w1;
	float			w2;
	t_step			left;
	t_step			right;

	p = triangle.p;
	t = triangle.t;
	delta = 1.0f / (float)(p[0].y - p[1].y);
	left = make_slope(p[1].x, p[0].x, delta);
	right = make_slope(p[2].x, p[0].x, delta);
	y = p[1].y;
	while (y < p[0].y)
	{
		scanline(left.location, right.location, y, p, t, sdl);
		left.location += left.step;
		right.location += right.step;
		y++;
	}
}

/*
creates two triangles from the given triangle one flat top and one flat bottom.
both triangles are then assigned to t_point p[3] array and passed onto fill_tri_bot/top functions.
p[0] is always the pointy head of the triangle p[1] and p[2] are flat points where, p[1] x is smaller than p[2]
*/

void	render_triangle_lit(t_sdlcontext *sdl, t_render *render, int index)
{
	t_point_triangle	triangle;
	t_point				p_split;
	t_texture			t_split;
	t_texture			t_temp;
	t_point				p_temp;
	t_point				*p;
	float				lerp;

	triangle = render->screenspace_ptris[index];
	if (sdl->ps1_tri_div > 1)
		triangle = ps1(triangle, sdl->ps1_tri_div);
	p = triangle.p;
	sort_point_uv_tri(triangle.p, triangle.t);
	lerp = ((float)p[1].y - (float)p[2].y) / ((float)p[0].y - (float)p[2].y);
	p_split.x = p[2].x + (lerp * ((float)p[0].x - (float)p[2].x));
	p_split.y = p[1].y;
	t_split.u = ft_flerp(triangle.t[2].u, triangle.t[0].u, lerp);
	t_split.v = ft_flerp(triangle.t[2].v, triangle.t[0].v, lerp);
	t_split.w = ft_flerp(triangle.t[2].w, triangle.t[0].w, lerp);
	if (p_split.x < p[1].x)
	{
		ft_swap(&p[1], &p_split, sizeof(t_point));
		ft_swap(&triangle.t[1], &t_split, sizeof(t_texture));
	}
	p_temp = p[2];
	t_temp = triangle.t[2];
	p[2] = p_split;
	triangle.t[2] = t_split;
	if (p[0].y != p[1].y && p[1].x != p[2].x)
		fill_point_tri_top(sdl, triangle);
	p[0] = p_temp;
	triangle.t[0] = t_temp;
	if (p[0].y != p[1].y && p[1].x != p[2].x)
		fill_point_tri_bot(sdl, triangle);
}
