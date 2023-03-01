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

inline static void scanline(int ax, int bx, int y, t_vector2 *p, t_texture *t, t_sdlcontext *sdl)
{
	t_texture	tex;
	t_vector2	bary;
	float		dist;

	//printf("ax %d bx %d\n", ax, bx);
	bary = barycentric_coordinates(p, (t_vector2){ax, y});
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
		bary = barycentric_coordinates(p, (t_vector2){ax, y});
	}
	render_bitmask_row(ax, bx, dist * 1000.0f, tex.w * 1000.0f, y, sdl);
}
if (xsample < 0 || ysample < 0 || ysample * render->map.size.x + xsample > render->map.size.x * render->map.size.y)
{
	printf("xsample ysample %d %d\n%d %d %d %d\n%f %f\n", xsample, ysample, render->img->size.x, render->img->size.y, render->map.size.x, render->map.size.y, t.u, t.v);
	exit(0);
}
*/
inline static uint32_t sample_img(t_render *render, uint32_t xsample, uint32_t ysample)
{
	return(render->map.data[ysample * render->map.size.x + xsample]);
}

inline static void scanline(int ax, int bx, int y, t_vector2*p, t_texture *t, t_sdlcontext *sdl)
{
	t_texture	tex;
	t_vector2	bary;
	t_vector2	left;
	t_vector2	right;
	float		dist;
	float		steps;

	left = barycentric_coordinates(p, vector2_add_xy((t_vector2){ax, y}, 0.5f));
	right = barycentric_coordinates(p, vector2_add_xy((t_vector2){bx, y}, 0.5f));
	bary = left;
	dist = ft_flerp(t[0].w, t[1].w, bary.x) + ((t[2].w - t[0].w) * bary.y);
	int start = ax;
	steps = bx - start;
	while(ax < bx)
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
				sample_img(&sdl->render, tex.u, tex.v);
		}
		ax++;
		bary = vector2_lerp(left, right, (float)(ax - start) / steps);
	}
	render_bitmask_row(start, bx, dist, tex.w, y, sdl);
}

static void render_flat_top_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2			*p;
	t_texture		*t;
	int				y;
	float			delta;
	float			w1;
	float			w2;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	left.step = (p[0].x - p[1].x) / (p[0].y - p[1].y);
	right.step = (p[0].x - p[2].x) / (p[0].y - p[2].y);
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

static void render_flat_bot_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2			*p;
	t_texture		*t;
	int				y;
	float			delta;
	float			w1;
	float			w2;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	left.step = (p[1].x - p[0].x) / (p[1].y - p[0].y);
	right.step = (p[2].x - p[0].x) / (p[2].y - p[0].y);
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
	t_vector2				p_split;
	t_texture			t_split;
	t_texture			t_temp;
	t_vector2				p_temp;
	t_vector2				*p;
	float				lerp;

	
	triangle = render->screenspace_ptris[index];
	if (sdl->ps1_tri_div > 1)
		triangle = ps1(triangle, sdl->ps1_tri_div);
	p = triangle.p;
	sort_point_uv_tri(triangle.p, triangle.t);
	if (p[1].y == p[2].y)
	{
		if (p[1].x > p[2].x)
		{
			ft_swap(&p[1], &p[2], sizeof(t_vector2));
			ft_swap(&triangle.t[1], &triangle.t[2], sizeof(t_texture));
		}
		render_flat_top_tri(sdl, triangle);
	}
	else if (p[0].y == p[1].y)
	{
		ft_swap(&p[0], &p[2], sizeof(t_vector2));
		ft_swap(&triangle.t[0], &triangle.t[2], sizeof(t_texture));
		if (p[1].x > p[2].x)
		{
			ft_swap(&p[1], &p[2], sizeof(t_vector2));
			ft_swap(&triangle.t[1], &triangle.t[2], sizeof(t_texture));
		}
		render_flat_bot_tri(sdl, triangle);
	}
	else
	{
		float alphasplit = (p[1].y - p[2].y) / (p[0].y - p[2].y);

		p_split.x = p[2].x + (alphasplit * (p[0].x - p[2].x));
		p_split.y = p[1].y;
		t_split.u = ft_flerp(triangle.t[2].u, triangle.t[0].u, alphasplit);
		t_split.v = ft_flerp(triangle.t[2].v, triangle.t[0].v, alphasplit);
		t_split.w = ft_flerp(triangle.t[2].w, triangle.t[0].w, alphasplit);
		if (p_split.x < p[1].x)
		{
			ft_swap(&p[1], &p_split, sizeof(t_vector2));
			ft_swap(&triangle.t[1], &t_split, sizeof(t_texture));
		}
		p_temp = p[2];
		t_temp = triangle.t[2];
		p[2] = p_split;
		triangle.t[2] = t_split;
		if (p[0].y != p[1].y && p[1].x != p[2].x)
			render_flat_top_tri(sdl, triangle);
		p[0] = p_temp;
		triangle.t[0] = t_temp;
		if (p[0].y != p[1].y && p[1].x != p[2].x)
			render_flat_bot_tri(sdl, triangle);
	}
}

