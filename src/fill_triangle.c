/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/26 16:25:14 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

static void	z_draw(t_sdlcontext sdl, t_point pos, uint32_t clr, float z)
{
	if (z > sdl.zbuffer[pos.x + pos.y * WINDOW_W])
	{
		sdl.zbuffer[pos.x + pos.y * WINDOW_W] = z;
		//draw(sdl.surface->pixels, pos, clr);
		((uint32_t *)sdl.surface->pixels)[pos.x + pos.y * WINDOW_W] = clr;
	}
}

static void z_draw_x_line(t_sdlcontext sdl, t_point from, int to, uint32_t clr, float z)
{
	while(from.x < to)
	{
		z_draw(sdl, from, clr, z);
		from.x++;
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

static void fill_tri_bot(t_sdlcontext sdl, t_triangle triangle, float z)
{
	float index;
	float max;
	int i;
	t_quaternion	*q;
	t_texture		*t;
	t_point			a;
	t_point			b;
	t_texture		tex;
	t_texture       t_a;
	t_texture       t_b;

	q = triangle.p;
	t = triangle.t;
	index = 0;
	max = q[1].v.y - q[0].v.y;
	while (index < max)
	{
		a.x = (int)flerp(q[1].v.x, q[0].v.x, index/max);
		a.y = q[1].v.y - index;
		t_a.u = flerp(t[1].u, t[0].u, index/max);
		t_a.v = flerp(t[1].v, t[0].v, index/max);
		t_a.w = flerp(t[1].w, t[0].w, index/max);

		b.x = (int)flerp(q[2].v.x, q[0].v.x, index/max);
		b.y = a.y;
		t_b.u = flerp(t[2].u, t[0].u, index/max);
		t_b.v = flerp(t[2].v, t[0].v, index/max);
		t_b.w = flerp(t[2].w, t[0].w, index/max);

		int i = a.x;
		while(i < b.x)
		{
			tex.u = flerp(t_a.u, t_b.u, ((float)(i - a.x))/(b.x - a.x));
			tex.v = flerp(t_a.v, t_b.v, ((float)(i - a.x))/(b.x - a.x));
			tex.w = flerp(t_a.w, t_b.w, ((float)(i - a.x))/(b.x - a.x));
			
			z_draw(sdl, (t_point){i, a.y}, sdl.debug_tex[64 * (int)((tex.u / tex.w) * 64.0f) + (int)((tex.v / tex.w) * 64.0f)], tex.w);
			i++;
		}
		//z_draw_x_line(sdl, a, b.x, CLR_PRPL, z);

		index++;
	}
}

static void fill_tri_top(t_sdlcontext sdl, t_triangle triangle, float z)
{
	float index;
	float max;
	int i;
	t_quaternion	*q;
	t_texture		*t;
	t_point			a;
	t_point			b;
	t_texture		tex;
	t_texture       t_a;
	t_texture       t_b;
	
	q = triangle.p;
	t = triangle.t;
	index = 0;
	max = q[0].v.y - q[1].v.y;
	while (index < max)
	{
		a.x = flerp(q[1].v.x, q[0].v.x, index/max);
		a.y = q[1].v.y + index;
		t_a.u = flerp(t[1].u, t[0].u, index/max);
		t_a.v = flerp(t[1].v, t[0].v, index/max);
		t_a.w = flerp(t[1].w, t[0].w, index/max);

		b.x = flerp(q[2].v.x, q[0].v.x, index/max);
		b.y = a.y;
		t_b.u = flerp(t[2].u, t[0].u, index/max);
		t_b.v = flerp(t[2].v, t[0].v, index/max);
		t_b.w = flerp(t[2].w, t[0].w, index/max);

		int i = a.x;
		while(i < b.x)
		{
			tex.u = flerp(t_a.u, t_b.u, ((float)(i - a.x))/(b.x - a.x));
			tex.v = flerp(t_a.v, t_b.v, ((float)(i - a.x))/(b.x - a.x));
			tex.w = flerp(t_a.w, t_b.w, ((float)(i - a.x))/(b.x - a.x));

			z_draw(sdl, (t_point){i, a.y}, sdl.debug_tex[64 * (int)((tex.u / tex.w) * 64.0f) + (int)((tex.v / tex.w) * 64.0f)], tex.w);
			i++;
		}
		index++;
	}
}

/*
creates two triangles from the given triangle one flat top and one flat bottom.
both triangles are then assigned to t_point p[3] array and passed onto fill_tri_bot/top functions.
p[0] is always the pointy head of the triangle p[1] and p[2] are flat points where, p[1] x is smaller than p[2]
*/
void	z_fill_tri(t_sdlcontext sdl, t_triangle triangle)
{
	t_quaternion	q_split;
	t_texture		t_split;
	t_texture		t_temp;
	t_quaternion	q_temp;
	t_quaternion	*q;
	t_point			p[3];
	float			lerp;

	//printf("\n\n");
	q = triangle.p;
	sort_tris(triangle.p, triangle.t);
	//printf_tri(triangle);
	lerp = (float)(q[1].v.y - q[2].v.y) / (float)(q[0].v.y - q[2].v.y);
	q_split.v.x = q[2].v.x + (lerp * (q[0].v.x - q[2].v.x));
	q_split.v.y = q[1].v.y;
	q_split.v.z = q[1].w; // not same maybe
	
//	lerp = (float)(triangle.t[1].v - triangle.t[2].v) / (float)(triangle.t[0].v - triangle.t[2].v);
/*	t_split.u = triangle.t[2].u + (lerp * (triangle.t[0].u - triangle.t[2].u));
	t_split.v = triangle.t[1].v;
	t_split.w = triangle.t[1].w;
*/
	t_split.u = flerp(triangle.t[2].u, triangle.t[0].u, lerp);
	t_split.v = flerp(triangle.t[2].v, triangle.t[0].v, lerp);
	t_split.w = flerp(triangle.t[2].w, triangle.t[0].w, lerp);
	//printf_tri(triangle);
	if (q_split.v.x < q[1].v.x)
	{
		ft_swap(&q[1], &q_split, sizeof(t_quaternion));
		ft_swap(&triangle.t[1], &t_split, sizeof(t_texture));
	}
	if (0)
	{
		t_triangle test;
		test.p[0] = (t_quaternion){0, 500.0f, 0, 0};
		test.p[1] = (t_quaternion){0, 0, 0, 0};
		test.p[2] = (t_quaternion){500.0f, 0, 0, 0};
		test.t[0] = (t_texture){0, 1, 1};
		test.t[2] = (t_texture){0, 0, 1};
		test.t[1] = (t_texture){1, 0, 1};
		fill_tri_top(sdl, test, q[0].w);
	}
	else
	{
		q[0].w -= 1000.0f;
	q_temp = q[2];
	t_temp = triangle.t[2];
	q[2] = q_split;
	triangle.t[2] = t_split;
	//printf_tri(triangle);
	fill_tri_top(sdl, triangle, q[0].w); 
	q[0] = q_temp;
	triangle.t[0] = t_temp;
	fill_tri_bot(sdl, triangle, q[0].w);
	}
}

/*
static void fill_tri_top(t_sdlcontext sdl, t_triangle triangle, float z)
{
	float index;
	float max;
	int i;
	t_quaternion	*q;
	t_texture		*t;
	t_point			a;
	t_point			b;
	t_texture		tex;
	t_texture       t_a;
	t_texture       t_b;
	
	q = triangle.p;
	t = triangle.t;
	index = 0;
	max = q[0].v.y - q[1].v.y;
	float maxu = t[1].u - t[0].u;
	printf("triangle u %f v %f w %f\n\n", triangle.t[0].u, triangle.t[0].v, triangle.t[0].w);
	printf("u0 %f u1 %f u2 %f\n\n", t[0].u, t[1].u, t[2].u);
	while (index < max)
	{
		a.x = (int)flerp(q[1].v.x, q[0].v.x, index/max);
		a.y = q[1].v.y + index;
		t_a.u = flerp(t[1].u, t[0].u, index/max);

		b.x = (int)flerp(q[2].v.x, q[0].v.x, index/max);
		b.y = a.y;
		t_b.u = flerp(t[2].u, t[0].u, index/max);

		tex.v = flerp(t[1].v, t[0].v, index/max);
		//tex.w = flerp(t[0].w, t[1].w, index/max);

		printf("v is %f\n", tex.v);
		printf("tex a %f b %f\n", t_a.u, t_b.u);
		int i = a.x;
		while(i < b.x)
		{
			tex.u = flerp(t_a.u, t_b.u, ((float)(i - a.x))/(b.x - a.x));
			//printf("x is %f\n", tex.u);
			z_draw(sdl, (t_point){i, a.y}, sdl.debug_tex[64 * (int)(((tex.u) * 64.0f) + ((tex.v) * 64.0f))], z);
			i++;
		}
		printf("\n");
		//printf("tex coordinate %d\n", 64 * (int)(((tex.u/tex.w) * 64.0f) + ((tex.v/tex.w) * 64.0f)));
		//z_draw_x_line(sdl, a, b.x, sdl.debug_tex[64 * (int)(((tex.u/tex.w) * 64.0f) + ((tex.v/tex.w) * 64.0f))], z);
		index++;
	}
}*/