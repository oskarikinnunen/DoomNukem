/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2022/12/06 17:44:56 by vlaine           ###   ########.fr       */
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

static void  calc_step_f(float x_step[2], float w_step[2], t_triangle triangle, float delta)
{
	t_quaternion	*q;

	q = triangle.p;
	x_step[0] = (q[0].v.x - q[1].v.x) * delta;
	x_step[1] = (q[0].v.x - q[2].v.x) * delta;
	w_step[0] = (q[0].w - q[1].w) * delta;
	w_step[1] = (q[0].w - q[2].w) * delta;
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

static uint32_t sample_img(t_img *img, t_texture t, t_lightmap *lightmap)
{
	uint32_t	clr;
	static uint8_t	x8b;
	static uint8_t	y8b;
	uint8_t	xsample;
	uint8_t	ysample;
	float	light;

	x8b = (t.u / t.w) * 255;
	xsample = (x8b * (lightmap->size.x - 1)) / 255;
	y8b = (t.v / t.w) * 255;
	ysample = (y8b * (lightmap->size.y - 1)) / 255;
	light = lightmap->data[(xsample * lightmap->size.x) + ysample] / 255.0f;
	x8b = (t.u / t.w) * 255;
	xsample = (x8b * (img->size.x - 1)) / 255;
	y8b = (t.v / t.w) * 255;
	ysample = (y8b * (img->size.y - 1)) / 255;
	clr = img->data[(xsample * img->size.x) + ysample];
	uint8_t blue = ((clr >> 16) & 0xFF) * light;
	uint8_t green = ((clr >> 8) & 0xFF) * light;
	uint8_t red = (clr & 0xFF) * light;

//do math

//new_RGB = (blue << 16) | (green << 8) | red;
	//clr = ((blue << 16))
	//clr = (uint8_t)(((clr >> 16) & 0xFF) * light) |  (uint8_t)(((clr >> 16) & 0xFF) * light) | (uint8_t)(((clr >> 8) & 0xFF) * light);
	return ((blue << 16) | (green << 8) | red);
}

static void fill_point_tri_bot(t_sdlcontext *sdl, t_point_triangle triangle, t_img *img, t_lightmap *lightmap)
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
		while(x <= ax)
		{
			if (t[0].w > sdl->zbuffer[x + y * sdl->window_w])
			{
				sdl->zbuffer[x + y * sdl->window_w] = t[0].w;
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] =
					sample_img(img, t[0], lightmap);
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


static void fill_point_tri_top(t_sdlcontext *sdl, t_point_triangle triangle, t_img *img, t_lightmap *lightmap)
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
			if (t[0].w > sdl->zbuffer[x + y * sdl->window_w])
			{
				sdl->zbuffer[x + y * sdl->window_w] = t[0].w;
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] =
					sample_img(img, t[0], lightmap);
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
void	render_triangle(t_sdlcontext *sdl, t_point_triangle triangle, t_img *img, t_lightmap *lightmap)
{
	t_point			p_split;
	t_texture		t_split;
	t_texture		t_temp;
	t_point			p_temp;
	t_point			*p;
	float			lerp;

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
		fill_point_tri_top(sdl, triangle, img, lightmap);
	p[0] = p_temp;
	triangle.t[0] = t_temp;
	if (p[0].y != p[1].y)
		fill_point_tri_bot(sdl, triangle, img, lightmap);
}
