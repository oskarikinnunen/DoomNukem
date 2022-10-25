/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/25 14:53:11 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

static void	z_draw(t_sdlcontext sdl, t_point pos, uint32_t clr, float z)
{
	if (z < sdl.zbuffer[pos.x + pos.y * WINDOW_W] || 1)
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

static void	sort_tris(t_quaternion *q)
{
	int	s_x;
	int	s_j;
	t_quaternion temp;

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (q[s_j].v.y < q[s_j + 1].v.y)
			{
				temp = q[s_j];
				q[s_j] = q[s_j + 1];
				q[s_j + 1] = temp;
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

static void fill_tri_bot(t_sdlcontext sdl, t_point *p, float z)
{
	float index;
	float max;
	t_point a;
	t_point b;

	index = 0;
	max = p[1].y - p[0].y;
	while (index < max)
	{
		a.x = flerp(p[1].x, p[0].x, index/max);
		a.y = p[1].y - index;
		b.x = flerp(p[2].x, p[0].x, index/max);
		b.y = a.y;
		z_draw_x_line(sdl, a, b.x, CLR_PRPL, z);
		index++;
	}
}

static void fill_tri_top(t_sdlcontext sdl, t_point *p, float z)
{
	float index;
	float max;
	t_point a;
	t_point b;

	index = 0;
	max = p[0].y - p[1].y;
	while (index < max)
	{
		a.x = flerp(p[1].x, p[0].x, index/max);
		a.y = p[1].y + index;
		b.x = flerp(p[2].x, p[0].x, index/max);
		b.y = a.y;
		z_draw_x_line(sdl, a, b.x, CLR_GREEN, z);
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
	t_quaternion	split;
	t_quaternion	*q;
	t_point			p[3];
	float			lerp;

	q = triangle.p;
	sort_tris(q);
	lerp = (float)(q[1].v.y - q[2].v.y) / (float)(q[0].v.y - q[2].v.y);
	split.v.x = q[2].v.x + (lerp * (q[0].v.x - q[2].v.x));
	split.v.y = q[1].v.y;
	split.v.z = q[1].w;

	if (split.v.x < q[1].v.x)
		ft_swap(&q[1].v.x, &split.v.x, sizeof(float));
	//printf("z is %f\n", q[0].w);
	p[0].x = q[0].v.x;
	p[0].y = q[0].v.y;
	p[1].x = q[1].v.x;
	p[1].y = q[1].v.y;
	p[2].x = split.v.x;
	p[2].y = split.v.y;
	fill_tri_top(sdl, p, q[0].w);
	p[0].x = q[2].v.x;
	p[0].y = q[2].v.y;
	fill_tri_bot(sdl, p, q[0].w);
}
