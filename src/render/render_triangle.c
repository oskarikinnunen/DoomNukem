/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_triangle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2022/12/10 19:23:15 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

static void ft_swap(void * a, void * b, size_t len)
{
	unsigned char	*a1;
	unsigned char	*b1;
	unsigned char	temp;
	size_t			i;

	a1 = a;
	b1 = b;
	i = 0;
	while (i < len)
	{
		temp = a1[i];
		a1[i] = b1[i];
		b1[i] = temp;
		i++;
	}
}

static void	sort_tris(t_quaternion	*q, t_texture *t)
{
	int	s_x;
	int	s_j;
	t_quaternion	temp_q;
	t_texture 		temp_t;

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (q[s_j].v.y < q[s_j + 1].v.y)
			{
				temp_q = q[s_j];
				q[s_j] = q[s_j + 1];
				q[s_j + 1] = temp_q;

				temp_t = t[s_j];
				t[s_j] = t[s_j + 1];
				t[s_j + 1] = temp_t;
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

static void	sort_point_tris(t_point *p, t_texture *t)
{
	int	s_x;
	int	s_j;
	t_point			temp_p;
	t_texture 		temp_t;

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (p[s_j].y < p[s_j + 1].y)
			{
				temp_p = p[s_j];
				p[s_j] = p[s_j + 1];
				p[s_j + 1] = temp_p;

				temp_t = t[s_j];
				t[s_j] = t[s_j + 1];
				t[s_j + 1] = temp_t;
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

float flerp(float from, float to, float delta)
{
	return (from + ((to - from) * delta));
}

static void  calc_points_step(float x_step[2], t_texture t_step[2], t_point *p, t_texture *t, float delta)
{
	x_step[0] = (p[0].x - p[1].x) * delta;
	x_step[1] = (p[0].x - p[2].x) * delta;

	t_step[0].u = (t[0].u - t[1].u) * delta;
	t_step[0].v = (t[0].v - t[1].v) * delta;
	t_step[0].w = (t[0].w - t[1].w) * delta;

	t_step[1].u = (t[0].u - t[2].u) * delta;
	t_step[1].v = (t[0].v - t[2].v) * delta;
	t_step[1].w = (t[0].w - t[2].w) * delta;
}

static t_texture calc_step_texture(t_texture *t, float delta)
{	
	t_texture step;
	step.u = (t[2].u - t[1].u) * delta;
	step.v = (t[2].v - t[1].v) * delta;
	step.w = (t[2].w - t[1].w) * delta;
	return(step);
}

typedef	struct s_rgb
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} t_rgb;

typedef struct s_color
{
	union cdata_u
	{
		t_rgb		rgb;
		uint32_t	color;
	} dat;
}	t_color;

static uint32_t	flip_channels(uint32_t clr)
{
	t_color		result;
	t_color		orig;
	orig.dat.color = clr;
	result.dat.rgb.r = orig.dat.rgb.b;
	result.dat.rgb.g = orig.dat.rgb.g;
	result.dat.rgb.b = orig.dat.rgb.r;
	result.dat.rgb.a = orig.dat.rgb.a;
	return (result.dat.color);
}

inline uint32_t sample_img(const t_render *render, const t_texture t)
{
	t_img		*img;
	uint32_t	xsample;
	uint32_t	ysample;

	img = render->img;
	
	xsample = (t.u / t.w) * (img->size.x - 1);
	ysample = (t.v / t.w) * (img->size.y - 1);
	return(render->img->data[xsample + img->size.x * ysample]);
}

#define FOG 0.0025f

static void fill_point_tri_bot(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
{
	t_point			*p;
	t_texture		*t;
	//t_point		t[3];
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
		while(x <= ax)
		{
			/*if (t[0].w < FOG && t[0].w > sdl->zbuffer[x + y * sdl->window_w])
			{
				sdl->zbuffer[x + y * sdl->window_w] = t[0].w;
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] = 1;
			}
			else */if (t[0].w > sdl->zbuffer[x + y * sdl->window_w])
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
		t_step[2] = calc_step_texture(t, 1.0f / (float)(ax - x));
		t[0].u = t[1].u;
		t[0].v = t[1].v;
		t[0].w = t[1].w;
		while(x <= ax)
		{
			/*if (t[0].w < FOG && t[0].w > sdl->zbuffer[x + y * sdl->window_w])
			{
				sdl->zbuffer[x + y * sdl->window_w] = t[0].w;
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] = 1;
			}
			else */if (t[0].w > sdl->zbuffer[x + y * sdl->window_w])
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
t_point_triangle	ps1(t_point_triangle in, int div)
{
	t_point_triangle	res;

	res = in;
	res.p[0] = point_div(res.p[0], div);
	res.p[1] = point_div(res.p[1], div);
	res.p[2] = point_div(res.p[2], div);

	res.p[0] = point_mul(res.p[0], div);
	res.p[1] = point_mul(res.p[1], div);
	res.p[2] = point_mul(res.p[2], div);

	return (res);
}

void	render_triangle(t_sdlcontext *sdl, t_render *render, int index)
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
	sort_point_tris(triangle.p, triangle.t);
	lerp = ((float)p[1].y - (float)p[2].y) / ((float)p[0].y - (float)p[2].y);
	p_split.x = p[2].x + (lerp * ((float)p[0].x - (float)p[2].x));
	p_split.y = p[1].y;
	t_split.u = flerp(triangle.t[2].u, triangle.t[0].u, lerp);
	t_split.v = flerp(triangle.t[2].v, triangle.t[0].v, lerp);
	t_split.w = flerp(triangle.t[2].w, triangle.t[0].w, lerp);

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

t_point_triangle	wf_tri(t_point_triangle in, float scaling)
{
	t_point_triangle	res;

	res = in;
	res.p[0] = point_fdiv(in.p[0], scaling);
	res.p[1] = point_fdiv(in.p[1], scaling);
	res.p[2] = point_fdiv(in.p[2], scaling);

	return (res);
}

void render_buffer_triangles(t_sdlcontext *sdl, t_render *render)
{
	int index;

	index = 0;
	while (index < render->screenspace_ptri_count)
	{
		render_triangle(sdl, render, index);
		index++;
	}
}

void render_buffer_triangle_wireframes(t_sdlcontext *sdl, t_render *render)
{
	int	index;

	index = 0;
	while (index < render->screenspace_ptri_count)
	{
		t_point_triangle t1 = wf_tri(render->screenspace_ptris[index], sdl->resolution_scaling);
		drawline(*sdl, t1.p[0], t1.p[1], render->gizmocolor);
		drawline(*sdl, t1.p[1], t1.p[2], render->gizmocolor);
		drawline(*sdl, t1.p[2], t1.p[0], render->gizmocolor);
		index++;
	}
}

//TODO: Gona be deprecated one year
void render_solid_triangle(t_sdlcontext *sdl, t_render *render)
{
	int	index;

	index = 0;
	while (index < render->screenspace_ptri_count)
	{
		t_img	img;

		img.data = (uint32_t [1]) {render->screenspace_ptris[index].clr};
		img.size.x = 1;
		img.size.y = 1;
		img.length = 1;
		render->img = &img;
		render_triangle(sdl, render, index);
		render->img = NULL;
		index++;
	}
}

void render_buffer(t_sdlcontext *sdl, t_render *render)
{
	if (!render->wireframe && render->img != NULL)
		render_buffer_triangles(sdl, render);
	if (render->wireframe)
		render_buffer_triangle_wireframes(sdl, render);
	if (render->img == NULL)
		render_solid_triangle(sdl, render);
	render->rs.triangle_count += render->screenspace_ptri_count;
}