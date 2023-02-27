#include "doomnukem.h"

inline static uint32_t sample_img_dynamic(t_render *render, t_texture t)
{
	uint32_t	xsample;
	uint32_t	ysample;

	xsample = (t.u * render->img->size.x);
	ysample = (t.v * render->img->size.y);
	xsample = xsample % (render->img->size.x);
	ysample = ysample % (render->img->size.y);
	return((render->img->data[ysample * render->img->size.x + xsample]));
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
				sample_img_dynamic(&sdl->render, tex);
		}
		ax++;
		bary = vector2_lerp(left, right, (float)(ax - start) / steps);
	}
	render_bitmask_row(start, bx, dist * 1000.0f, tex.w * 1000.0f, y, sdl);// times by far
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

void	render_triangle_unlit(t_sdlcontext *sdl, t_render *render, int index)
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
