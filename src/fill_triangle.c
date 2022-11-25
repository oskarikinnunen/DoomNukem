/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/24 19:10:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

//not currently used
static void	z_draw(t_sdlcontext sdl, t_point pos, uint32_t clr, float z)
{
	if (z > sdl.zbuffer[pos.x + pos.y * sdl.window_w])
	{
		sdl.zbuffer[pos.x + pos.y * sdl.window_w] = z;
		((uint32_t *)sdl.surface->pixels)[pos.x + pos.y * sdl.window_w] = clr;
	}
}

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

static void  calc_step(float x_step[2], t_texture t_step[2], t_triangle triangle, float delta)
{
	t_quaternion	*q;
	t_texture		*t;

	q = triangle.p;
	t = triangle.t;
	x_step[0] = (q[0].v.x - q[1].v.x) * delta;
	x_step[1] = (q[0].v.x - q[2].v.x) * delta;

	t_step[0].u = (t[0].u - t[1].u) * delta;
	t_step[0].v = (t[0].v - t[1].v) * delta;
	t_step[0].w = (t[0].w - t[1].w) * delta;

	t_step[1].u = (t[0].u - t[2].u) * delta;
	t_step[1].v = (t[0].v - t[2].v) * delta;
	t_step[1].w = (t[0].w - t[2].w) * delta;
}

static t_triangle step_triangle(t_triangle triangle, float x_step[2], t_texture t_step[2])
{
	t_quaternion	*q;
	t_texture		*t;

	q = triangle.p;
	t = triangle.t;

	q[1].v.x += x_step[0];
	q[2].v.x += x_step[1];

	t[1].u += t_step[0].u;
	t[1].v += t_step[0].v;
	t[1].w += t_step[0].w;

	t[2].u += t_step[1].u;
	t[2].v += t_step[1].v;
	t[2].w += t_step[1].w;

	return (triangle);
}

static t_triangle step_triangle_f(t_triangle triangle, float x_step[2], float w_step[2])
{
	t_quaternion	*q;

	q = triangle.p;

	q[1].v.x += x_step[0];
	q[2].v.x += x_step[1];
	q[1].w += w_step[0];
	q[2].w += w_step[1];
	return (triangle);
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
	//result += ((clr >> 8) & 0xFF) >> 8;
}

static void step_ab(t_texture *step, t_triangle triangle, float delta)
{
	//t_quaternion	*q;
	t_texture		*t;

	//q = triangle.p;
	t = triangle.t;
	
	step->u = (t[2].u - t[1].u) * delta;
	step->v = (t[2].v - t[1].v) * delta;
	step->w = (t[2].w - t[1].w) * delta;
}

static uint32_t sample_img(t_img *img, t_texture t)
{
	static uint8_t	x8b;
	static uint8_t	y8b;
	uint8_t	xsample;
	uint8_t	ysample;

	x8b = (t.u / t.w) * 255;
	xsample = (x8b * (img->size.x - 1)) / 255;
	y8b = (t.v / t.w) * 255;
	ysample = (y8b * (img->size.y - 1)) / 255;
	return (img->data[(xsample * img->size.x) + ysample]);
}

static void fill_tri_bot(t_sdlcontext sdl, t_triangle triangle, t_img *img)
{
	t_quaternion	*q;
	t_texture		*t;
	float			x_step[2];
	t_texture		t_step[3];
	int				index;
	float			i;

	q = triangle.p;
	t = triangle.t;
	calc_step(x_step, t_step, triangle, 1.0f / (q[1].v.y - q[0].v.y));
	while (q[0].v.y < q[1].v.y)
	{
		step_ab(&t_step[2], triangle, 1.0f / (q[2].v.x - q[1].v.x));
		t[0].u = t[1].u;
		t[0].v = t[1].v;
		t[0].w = t[1].w;
		index = 0;
		i = q[1].v.x;
		while(i < q[2].v.x)
		{
			if (t[0].w > sdl.zbuffer[(int)(i) + (int)q[1].v.y * sdl.window_w])
			{
				sdl.zbuffer[(int)(i) + (int)q[1].v.y * sdl.window_w] = t[0].w;
				t[0].u += (t_step[2].u * index);
				t[0].v += (t_step[2].v * index);
				index = 0;
				static uint8_t	x8b;
				static uint8_t	y8b;
				uint8_t	xsample;
				uint8_t	ysample;

				x8b = (t[0].u / t[0].w) * 255;
				xsample = (x8b * (img->size.x - 1)) / 255;
				y8b = (t[0].v / t[0].w) * 255;
				ysample = (y8b * (img->size.y - 1)) / 255;
				((uint32_t *)sdl.surface->pixels)[(int)(i) + (int)q[1].v.y * sdl.window_w]
					= flip_channels(img->data[(xsample * img->size.x) + ysample]); //used to use sampleimage
			}
			index++;
			t[0].w += t_step[2].w;
			i++;
		}
		triangle = step_triangle(triangle, x_step, t_step);
		q[1].v.y--;
	}
}

static void fill_tri_bot_solid(t_sdlcontext sdl, t_triangle triangle)
{
	t_quaternion	*q;
	float			x_step[2];
	float			w_step[2];
	int				index;
	float			i;

	q = triangle.p;
	calc_step_f(x_step, w_step, triangle, 1.0f / (q[1].v.y - q[0].v.y));
	while (q[0].v.y < q[1].v.y)
	{
		index = 0;
		i = q[1].v.x;
		while(i < q[2].v.x)
		{
			if (q[0].w > sdl.zbuffer[(int)(i) + (int)q[1].v.y * sdl.window_w])
			{
				sdl.zbuffer[(int)(i) + (int)q[1].v.y * sdl.window_w] = q[0].w;
				index = 0;
				((uint32_t *)sdl.surface->pixels)[(int)(i) + (int)q[1].v.y * sdl.window_w]
					= triangle.clr;
			}
			index++;
			i++;
		}
		triangle = step_triangle_f(triangle, x_step, w_step);
		q[1].v.y--;
	}
}

static void fill_tri_top_solid(t_sdlcontext sdl, t_triangle triangle)
{
	t_quaternion	*q;
	float			x_step[2];
	float			w_step[2];
	float			i;

	q = triangle.p;
	calc_step_f(x_step, w_step, triangle, 1.0f / (q[0].v.y - q[1].v.y));
	while (q[1].v.y < q[0].v.y)
	{
		i = q[1].v.x;
		while(i < q[2].v.x)
		{
			q[0].w = q[1].w;
			if (q[0].w > sdl.zbuffer[(int)(i) + (int)q[1].v.y * sdl.window_w])
			{
				sdl.zbuffer[(int)(i) + (int)q[1].v.y * sdl.window_w] = q[0].w;
				((uint32_t *)sdl.surface->pixels)[(int)(i) + (int)q[1].v.y * sdl.window_w] =
					triangle.clr;
			}
			q[0].w += w_step[2];
			i++;
		}
		triangle = step_triangle_f(triangle, x_step, w_step);
		q[1].v.y++;
	}
}

static void fill_tri_top(t_sdlcontext sdl, t_triangle triangle, t_img *img)
{
	t_quaternion	*q;
	t_texture		*t;
	float			x_step[2];
	t_texture		t_step[3];
	int				index;
	float			i;

	q = triangle.p;
	t = triangle.t;
	calc_step(x_step, t_step, triangle, 1.0f / (q[0].v.y - q[1].v.y));
	while (q[1].v.y < q[0].v.y)
	{
		step_ab(&t_step[2], triangle, 1.0f / (q[2].v.x - q[1].v.x));
		t[0].u = t[1].u;
		t[0].v = t[1].v;
		t[0].w = t[1].w;
		index = 0;
		i = q[1].v.x;
		while(i < q[2].v.x)
		{
			if (t[0].w > sdl.zbuffer[(int)(i) + (int)q[1].v.y * sdl.window_w])
			{
				sdl.zbuffer[(int)(i) + (int)q[1].v.y * sdl.window_w] = t[0].w;
				t[0].u += (t_step[2].u * index);
				t[0].v += (t_step[2].v * index);
				index = 0;
				static uint8_t	x8b;
				static uint8_t	y8b;
				uint8_t	xsample;
				uint8_t	ysample;
				x8b = (t[0].u / t[0].w) * 255;
				xsample = (x8b * (img->size.x - 1)) / 255;
				y8b = (t[0].v / t[0].w) * 255;
				ysample = (y8b * (img->size.y - 1)) / 255;
				((uint32_t *)sdl.surface->pixels)[(int)(i) + (int)q[1].v.y * sdl.window_w]
					= flip_channels(img->data[(xsample * img->size.x) + ysample]);
			}
			index++;
			t[0].w += t_step[2].w;
			i++;
		}
		triangle = step_triangle(triangle, x_step, t_step);
		q[1].v.y++;
	}
}


/*
creates two triangles from the given triangle one flat top and one flat bottom.
both triangles are then assigned to t_point p[3] array and passed onto fill_tri_bot/top functions.
p[0] is always the pointy head of the triangle p[1] and p[2] are flat points where, p[1] x is smaller than p[2]
*/
void	z_fill_tri_solid(t_sdlcontext sdl, t_triangle tri)
{
	t_quaternion	q_split;
	t_quaternion	q_temp;
	t_point			p[3];
	float			lerp;

	sort_tris(tri.p, tri.t);
	lerp = (float)(tri.p[1].v.y - tri.p[2].v.y) / (float)(tri.p[0].v.y - tri.p[2].v.y);
	q_split.v.x = tri.p[2].v.x + (lerp * (tri.p[0].v.x - tri.p[2].v.x));
	q_split.v.y = tri.p[1].v.y;
	q_split.w = flerp(tri.p[0].w, tri.p[2].w, lerp);
	if (q_split.v.x < tri.p[1].v.x)
		ft_swap(&tri.p[1], &q_split, sizeof(t_quaternion));
	q_temp = tri.p[2];
	tri.p[2] = q_split;
	fill_tri_top_solid(sdl, tri);
	tri.p[0] = q_temp;
	//fill_tri_bot_solid(sdl, tri);
}

void	z_fill_tri(t_sdlcontext sdl, t_triangle triangle, t_img img)
{
	t_quaternion	q_split;
	t_texture		t_split;
	t_texture		t_temp;
	t_quaternion	q_temp;
	t_quaternion	*q;
	t_point			p[3];
	float			lerp;

	q = triangle.p;
	sort_tris(triangle.p, triangle.t);
	lerp = (float)(q[1].v.y - q[2].v.y) / (float)(q[0].v.y - q[2].v.y);
	q_split.v.x = q[2].v.x + (lerp * (q[0].v.x - q[2].v.x));
	q_split.v.y = q[1].v.y;
	//q_split.v.z = q[1].w;

	t_split.u = flerp(triangle.t[2].u, triangle.t[0].u, lerp);
	t_split.v = flerp(triangle.t[2].v, triangle.t[0].v, lerp);
	t_split.w = flerp(triangle.t[2].w, triangle.t[0].w, lerp);

	if (q_split.v.x < q[1].v.x)
	{
		ft_swap(&q[1], &q_split, sizeof(t_quaternion));
		ft_swap(&triangle.t[1], &t_split, sizeof(t_texture));
	}
	q_temp = q[2];
	t_temp = triangle.t[2];
	q[2] = q_split;
	triangle.t[2] = t_split;
	fill_tri_top(sdl, triangle, &img);
	q[0] = q_temp;
	triangle.t[0] = t_temp;
	fill_tri_bot(sdl, triangle, &img);
}
