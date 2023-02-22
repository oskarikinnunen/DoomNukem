#include "doomnukem.h"

inline static uint32_t sample_img_dynamic(t_render *render, t_texture t)
{
	uint32_t	xsample;
	uint32_t	ysample;

	xsample = (t.u) * (render->img->size.x);
	xsample = xsample % (render->img->size.x);
	ysample = (t.v) * (render->img->size.y);
	ysample = ysample % (render->img->size.y);
	return((render->img->data[(ysample * (render->img->size.x)) + xsample]));
}

//	w1 = ((float)p[0].x * (float)(p[2].y - p[0].y) + (float)(y - p[0].y) * (float)(p[2].x - p[0].x) - (float)ax * (float)(p[2].y - p[0].y)) / (float)((float)(p[1].y - p[0].y) * (float)(p[2].x - p[0].x) - (float)(p[1].x - p[0].x) * (float)(p[2].y - p[0].y));
//	w2 = (float)(y - p[0].y - w1 * (float)(p[1].y - p[0].y)) / (float)(p[2].y - p[0].y);


inline static void scanline(int ax, int bx, int y, t_point *p, t_texture *t, t_sdlcontext *sdl)
{
	t_texture	tex;
	float		w1;
	float		w2;
	float		dist;

	w1 = ((float)p[0].x * (float)(p[2].y - p[0].y) + (float)(y - p[0].y) * (float)(p[2].x - p[0].x) - (float)ax * (float)(p[2].y - p[0].y)) / (float)((float)(p[1].y - p[0].y) * (float)(p[2].x - p[0].x) - (float)(p[1].x - p[0].x) * (float)(p[2].y - p[0].y));
	w1 = ft_maxf(w1, 0.0f); //this is horrible
	w2 = (float)(y - p[0].y - w1 * (float)(p[1].y - p[0].y)) / (float)(p[2].y - p[0].y);
	w2 = ft_maxf(w2, 0.0f);
	dist = ft_flerp(t[0].w, t[1].w, w1);
	dist += ((t[2].w - t[0].w) * w2);
	while(ax <= bx)
	{
		tex.u = ft_flerp(t[0].u, t[1].u, w1);
		tex.u += ((t[2].u - t[0].u) * w2);
		tex.v = ft_flerp(t[0].v, t[1].v, w1);
		tex.v += ((t[2].v - t[0].v) * w2);
		tex.w = ft_flerp(t[0].w, t[1].w, w1);
		tex.w += ((t[2].w - t[0].w) * w2);
		if (tex.w > sdl->zbuffer[ax + y * sdl->window_w])
		{
			tex.u = tex.u / tex.w;
			tex.v = tex.v / tex.w;
			sdl->zbuffer[ax + y * sdl->window_w] = tex.w;
			((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] =
				sample_img_dynamic(&sdl->render, tex);
		}
		ax++;
		w1 = ((float)p[0].x * (float)(p[2].y - p[0].y) + (float)(y - p[0].y) * (float)(p[2].x - p[0].x) - (float)ax * (float)(p[2].y - p[0].y)) / (float)((float)(p[1].y - p[0].y) * (float)(p[2].x - p[0].x) - (float)(p[1].x - p[0].x) * (float)(p[2].y - p[0].y));
		w1 = ft_maxf(w1, 0.0f); //this is horrible
		w2 = (float)(y - p[0].y - w1 * (float)(p[1].y - p[0].y)) / (float)(p[2].y - p[0].y);
		w2 = ft_maxf(w2, 0.0f);
	}
	render_bitmask_row(ax, bx, dist * 1000.0f, tex.w * 1000.0f, y, sdl);
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
	delta = p[1].y - p[0].y;
	left.location = p[0].x;
	left.step = (p[1].x - p[0].x) / delta;
	right.location = p[0].x;
	right.step = (p[2].x - p[0].x) / delta;
	y = p[0].y;
	while (y < p[1].y)
	{
		scanline(left.location, right.location, y, p, t, sdl);
		left.location += left.step;
		right.location += right.step;
		y++;
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
	delta = p[0].y - p[1].y;
	left.location = p[1].x;
	left.step = (p[0].x - p[1].x) / delta;
	right.location = p[2].x;
	right.step = (p[0].x - p[2].x) / delta;
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

/*

void	render_triangle_unlit(t_sdlcontext *sdl, t_render *render, int index)
{
	SDL_Thread			*pthread_pool[2];
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
	t_test tri_inf[2];
	p_temp = p[2];
	t_temp = triangle.t[2];
	p[2] = p_split;
	triangle.t[2] = t_split;
	tri_inf[0].triangle = triangle;
	tri_inf[0].sdl = sdl;
	bool efg[2];
	efg[0] = false;
	efg[1] = false;

	if (p[0].y != p[1].y && p[1].x != p[2].x)
	{
		efg[0] = true;
		pthread_pool[0] = SDL_CreateThread(fill_point_tri_top, "thread1", &tri_inf[0]);
	}
		//pthread_create(&pthread_pool, NULL, &fill_point_tri_top, (void *)&tri_inf[0]);
		//fill_point_tri_top(&tri_inf[0]);
	p[0] = p_temp;
	triangle.t[0] = t_temp;
	tri_inf[1].triangle = triangle;
	tri_inf[1].sdl = sdl;
	if (p[0].y != p[1].y && p[1].x != p[2].x)
	{
		efg[1] = true;
		pthread_pool[1] = SDL_CreateThread(fill_point_tri_bot, "thread2", &tri_inf[1]);
	}
		//ill_point_tri_bot(&tri_inf[1]);
	for (int i = 0; i < 2; i++)
	{
		if (efg[i])
			SDL_WaitThread(pthread_pool[i], NULL);
	}
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

		render_bitmask_row(ax, bx, temp.w * 1000.0f, t_temp.w * 1000.0f, y, sdl);
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
					sample_img_dynamic(render, temp);
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

		render_bitmask_row(ax, bx, temp.w * 1000.0f, t_temp.w * 1000.0f, y, sdl);

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
					sample_img_dynamic(render, temp);
			}
			temp.u += t_step[0].u;
			temp.v += t_step[0].v;
			temp.w += t_step[0].w;
			ax++;
		}
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
	if (p[0].y != p[1].y)
		fill_point_tri_top(sdl, triangle, render);
	p[0] = p_temp;
	triangle.t[0] = t_temp;
	if (p[0].y != p[1].y)
		fill_point_tri_bot(sdl, triangle, render);
}

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
	t_vector2	left, right;
	float	inv_step = 1.0f / (p[1].y - p[0].y);
	left.x = p[0].x;
	left.y = (p[1].x - p[0].x) * inv_step;

	right.x = p[0].x;
	right.y = (p[2].x - p[0].x) * inv_step;
	y = p[0].y;
	while (y < p[1].y)
	{
		ax = floorf(left.x);
		bx = floorf(right.x);
		while(ax < bx)
		{
			((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] = CLR_GREEN;
			ax++;
		}
		left.x += left.y;
		right.x += right.y;
		y++;
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

	t_vector2	left, right;
	float	inv_step = 1.0f / (p[0].y - p[1].y);
	left.x = p[1].x;
	left.y = (p[0].x - p[1].x) * inv_step;

	right.x = p[2].x;
	right.y = (p[0].x - p[2].x) * inv_step;
	y = p[1].y;
	while (y < p[0].y)
	{
		ax = floorf(left.x);
		bx = floorf(right.x);
		while(ax < bx)
		{
			((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] = CLR_PRPL;
			ax++;
		}
		left.x += left.y;
		right.x += right.y;
		y++;
	}
}
*/