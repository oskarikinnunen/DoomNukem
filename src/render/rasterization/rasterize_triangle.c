#include "doomnukem.h"

#define FOG 0.0025f

static uint32_t sample_img(t_render *render, t_texture t)
{
	uint32_t	xsample;
	uint32_t	ysample;

	xsample = (t.u / t.w) * (render->map.img_size.x);
	ysample = (t.v / t.w) * (render->map.img_size.y);

	/*if (xsample >= render->map.size.x || xsample < 0 || ysample < 0 || ysample >= render->map.size.y)
	{
		printf("xsample %i", xsample);
		printf("ysample %i \n", ysample);
		printf("img dim %i %i \n\n", render->map.size.x, render->map.size.y);
	}*/
	//render->
	return(render->map.data[ysample * render->map.size.x + xsample]);
}

typedef struct s_barycentric
{
	float	v1;
	float	v2;
	float	v3;
	float	v4;
	float	v5;
	float	b1;
	float	b2;
	float	b3;
}	t_barycentric;

//	w1 = ((float)p[0].x * (float)(p[2].y - p[0].y) + (float)(y - p[0].y) * (float)(p[2].x - p[0].x) - (float)ax * (float)(p[2].y - p[0].y)) / (float)((float)(p[1].y - p[0].y) * (float)(p[2].x - p[0].x) - (float)(p[1].x - p[0].x) * (float)(p[2].y - p[0].y));
//	w2 = (float)(y - p[0].y - w1 * (float)(p[1].y - p[0].y)) / (float)(p[2].y - p[0].y);

static void fill_point_tri_bot(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	t_texture		t_step[3];
	int				ax;
	int				bx;
	int				y;
	float			delta;
	float			w1;
	float			w2;
	t_texture		t_temp;
	t_texture		temp;
	t_barycentric	bary;

	p = triangle.p;
	t = triangle.t;
	delta = p[1].y - p[0].y;
	step[0] = (p[0].x - p[1].x) / delta;
	step[1] = (p[0].x - p[2].x) / delta;

	bary.b1 = p[2].y - p[0].y;
	bary.b2	= p[2].x - p[0].x;
	bary.b3 = p[1].y - p[0].y;

	bary.v1 = p[0].x * bary.b1;
	bary.v2 = bary.b3 * bary.b2 - (float)(p[1].x - p[0].x) * bary.b1;
	y = p[1].y;
	while (y >= p[0].y)
	{
		delta = p[1].y - y;
		ax = p[1].x + (step[0] * delta);
		bx = p[2].x + (step[1] * delta);
	
		float b_temp = bary.v1 + (float)(y - p[0].y) * bary.b2;
		w1 = (b_temp - ax * bary.b1) / bary.v2;
		w2 = (float)(y - p[0].y - w1 * bary.b3) / bary.b1;
		temp.u = ft_flerp(t[0].u, t[1].u, w1);
		temp.u += ((t[2].u - t[0].u) * w2);
		temp.v = ft_flerp(t[0].v, t[1].v, w1);
		temp.v += ((t[2].v - t[0].v) * w2);
		temp.w = ft_flerp(t[0].w, t[1].w, w1);
		temp.w += ((t[2].w - t[0].w) * w2);

		w1 = (b_temp - bx * bary.b1) / bary.v2;
		w2 = (float)(y - p[0].y - w1 * bary.b3) / bary.b1;
		t_temp.u = ft_flerp(t[0].u, t[1].u, w1);
		t_temp.u += ((t[2].u - t[0].u) * w2);
		t_temp.v = ft_flerp(t[0].v, t[1].v, w1);
		t_temp.v += ((t[2].v - t[0].v) * w2);
		t_temp.w = ft_flerp(t[0].w, t[1].w, w1);
		t_temp.w += ((t[2].w - t[0].w) * w2);

		render_bitmask_row(ax, bx, 1.0f / temp.w, 1.0f / t_temp.w, y, sdl);
		delta = bx - ax;
		t_step[0].u = (t_temp.u - temp.u) / delta;
		t_step[0].v = (t_temp.v - temp.v) / delta;
		t_step[0].w = (t_temp.w - temp.w) / delta;
		while(ax <= bx)
		{
			if (temp.w > sdl->zbuffer[ax + y * sdl->window_w])
			{
				sdl->zbuffer[ax + y * sdl->window_w] = temp.w;
				((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] =
					sample_img(render, temp);
			}
			temp.u += t_step[0].u;
			temp.v += t_step[0].v;
			temp.w += t_step[0].w;
			ax++;
		}
		y--;
	}
}

static t_texture calc_step_texture2(t_texture *t, float delta)
{	
	t_texture step;
	step.u = (t[2].u - t[1].u) / delta;
	step.v = (t[2].v - t[1].v) / delta;
	step.w = (t[2].w - t[1].w) / delta;
	return(step);
}

#include <assert.h>

static void fill_point_tri_top(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	t_texture		t_step[3];
	int				ax;
	int				bx;
	int				y;
	float			delta;
	float			w1;
	float			w2;
	t_texture		t_temp;
	t_texture		temp;
	t_barycentric	bary;

	p = triangle.p;
	t = triangle.t;
	delta = p[0].y - p[1].y;
	step[0] = (p[0].x - p[1].x) / delta;
	step[1] = (p[0].x - p[2].x) / delta;

	bary.b1 = p[2].y - p[0].y;
	bary.b2	= p[2].x - p[0].x;
	bary.b3 = p[1].y - p[0].y;

	bary.v1 = p[0].x * bary.b1;
	bary.v2 = bary.b3 * bary.b2 - (float)(p[1].x - p[0].x) * bary.b1;

	y = p[1].y;
	while (y <= p[0].y)
	{
		delta = y - p[1].y;
		ax = p[1].x + (step[0] * delta);
		bx = p[2].x + (step[1] * delta);
	
		float b_temp = bary.v1 + (float)(y - p[0].y) * bary.b2;
		w1 = (b_temp - ax * bary.b1) / bary.v2;
		w2 = (float)(y - p[0].y - w1 * bary.b3) / bary.b1;
		temp.u = ft_flerp(t[0].u, t[1].u, w1);
		temp.u += ((t[2].u - t[0].u) * w2);
		temp.v = ft_flerp(t[0].v, t[1].v, w1);
		temp.v += ((t[2].v - t[0].v) * w2);
		temp.w = ft_flerp(t[0].w, t[1].w, w1);
		temp.w += ((t[2].w - t[0].w) * w2);

		w1 = (b_temp - bx * bary.b1) / bary.v2;
		w2 = (float)(y - p[0].y - w1 * bary.b3) / bary.b1;
		t_temp.u = ft_flerp(t[0].u, t[1].u, w1);
		t_temp.u += ((t[2].u - t[0].u) * w2);
		t_temp.v = ft_flerp(t[0].v, t[1].v, w1);
		t_temp.v += ((t[2].v - t[0].v) * w2);
		t_temp.w = ft_flerp(t[0].w, t[1].w, w1);
		t_temp.w += ((t[2].w - t[0].w) * w2);

		render_bitmask_row(ax, bx, 1.0f / temp.w, 1.0f / t_temp.w, y, sdl);

		delta = bx - ax;
		t_step[0].u = (t_temp.u - temp.u) / delta;
		t_step[0].v = (t_temp.v - temp.v) / delta;
		t_step[0].w = (t_temp.w - temp.w) / delta;
		while(ax <= bx)
		{
			if (temp.w > sdl->zbuffer[ax + y * sdl->window_w])
			{
				sdl->zbuffer[ax + y * sdl->window_w] = temp.w;
				((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] =
					sample_img(render, temp);
			}
			temp.u += t_step[0].u;
			temp.v += t_step[0].v;
			temp.w += t_step[0].w;
			ax++;
		}
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
	if (p[0].y != p[1].y)
		fill_point_tri_top(sdl, triangle, render);
	p[0] = p_temp;
	triangle.t[0] = t_temp;
	if (p[0].y != p[1].y)
		fill_point_tri_bot(sdl, triangle, render);
}
