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

static void fill_point_tri_bot(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	t_texture		t_step[3];
	int				x;
	int				y;
	float			delta;

	p = triangle.p;
	t = triangle.t;
	calc_points_step(step, t_step, p, t, 1.0f / ((float)(p[1].y - p[0].y)));
	y = p[1].y;
	while (y >= p[0].y)
	{
		x = p[1].x + (step[0] * (float)(p[1].y - y));
		int ax =  p[2].x + (step[1] * (float)(p[1].y - y));
		t_step[2] = calc_step_texture(t, 1.0f / (float)(ax - x));
		t[0].u = t[1].u;
		t[0].v = t[1].v;
		t[0].w = t[1].w;
		render_bitmask_row(x, ax, 1.0f / t[1].w, 1.0f / t[2].w, y, sdl);
		int startx = x;
		while(x <= ax)
		{
			
			if (t[0].w > sdl->zbuffer[x + y * sdl->window_w])
			{
				sdl->zbuffer[x + y * sdl->window_w] = t[0].w;
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] =
					sample_img(render, t[0]);
			}
			t[0].u += t_step[2].u;
			t[0].v += t_step[2].v;
			t[0].w += t_step[2].w;
			x++;
		}
		t[1].u += t_step[0].u;
		t[1].v += t_step[0].v;
		t[1].w += t_step[0].w;

		t[2].u += t_step[1].u;
		t[2].v += t_step[1].v;
		t[2].w += t_step[1].w;
		y--;
	}
}

t_texture calc_step_texture2(t_texture *t, float delta)
{	
	t_texture step;
	step.u = (t[2].u - t[1].u) / delta;
	step.v = (t[2].v - t[1].v) / delta;
	step.w = (t[2].w - t[1].w) / delta;
	return(step);
}

static void fill_point_tri_top(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	t_texture		t_step[3];
	int				x;
	int				y;
	float			delta;

	p = triangle.p;
	t = triangle.t;
	calc_points_step(step, t_step, p, t, 1.0f/((float)(p[0].y - p[1].y)));
	y = p[1].y;
	while (y <= p[0].y)
	{
		x = p[1].x + (step[0] * (float)(y - p[1].y));
		int ax =  p[2].x + (step[1] * (float)(y - p[1].y));
		t_step[2] = calc_step_texture2(t, (float)(ax - x));
		t[0].u = t[1].u;
		t[0].v = t[1].v;
		t[0].w = t[1].w;
		render_bitmask_row(x, ax, 1.0f / t[1].w, 1.0f / t[2].w, y, sdl);
		while(x < ax)
		{
			if (t[0].w > sdl->zbuffer[x + y * sdl->window_w])
			{
				sdl->zbuffer[x + y * sdl->window_w] = t[0].w;
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] =
					sample_img(render, t[0]);
			}
			t[0].u += t_step[2].u;
			t[0].v += t_step[2].v;
			t[0].w += t_step[2].w;
			x++;
		}
		t[1].u += t_step[0].u;
		t[1].v += t_step[0].v;
		t[1].w += t_step[0].w;

		t[2].u += t_step[1].u;
		t[2].v += t_step[1].v;
		t[2].w += t_step[1].w;
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
