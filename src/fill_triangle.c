/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/24 22:02:32 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

float z_dist;
uint32_t color = CLR_PRPL;

static void	z_draw(t_sdlcontext sdl, t_point pos, uint32_t clr, float z)
{
	//z = (uint32_t)z_dist;
	if (z < sdl.zbuffer[pos.x + pos.y * WINDOW_W] || 1)
	{
		sdl.zbuffer[pos.x + pos.y * WINDOW_W] = z;
		((uint32_t *)sdl.surface->pixels)[pos.x + pos.y * WINDOW_W] = clr;
	}
}

static void z_drawline(t_sdlcontext sdl, t_point from, t_point to, uint32_t clr, float z)
{
	static t_bresenham	b;

	populate_bresenham(&b, from, to);
	z_draw(sdl, b.local, clr, z);
	while (step_bresenham(&b) != 1)
		z_draw(sdl, b.local, clr, z);
	z_draw(sdl, b.local, clr, z);
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

static void ft_swap_x(t_point *from, t_point *to)
{
	t_point temp;
	temp.x = from->x;
	from->x = to->x;
	to->x = temp.x;
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
				/*fun
				ft_swap(&temp, &q[s_j], sizeof(t_quaternion));
				ft_swap(&q[s_j], &q[s_j + 1], sizeof(t_quaternion));
				ft_swap(&q[s_j + 1], &temp, sizeof(t_quaternion));
				*/
				temp = q[s_j];
				q[s_j] = q[s_j + 1];
				q[s_j + 1] = temp;
				
				//ft_memcpy(temp, q[s_j], sizeof(t_quaternion));
				//ft_memcpy(tris[s_j], tris[s_j + 1], sizeof(int) * 3);
				//ft_memcpy(tris[s_j + 1], temp, sizeof(int) * 3);
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

static void	z_fill_sub_tri(t_sdlcontext sdl, t_triangle triangle, t_quaternion ref[3])
{
	t_bresenham		b[2];
	t_quaternion	*q;
	t_quaternion	*p = ref;

	q = ref;

	int y = q[1].v.y;
	populate_bresenham(&(b[0]), (t_point){q[0].v.x, q[0].v.y}, (t_point){q[1].v.x, q[1].v.y});
	populate_bresenham(&(b[1]), (t_point){q[0].v.x, q[0].v.y}, (t_point){q[2].v.x, q[2].v.y});


	while (b[0].local.y != y)
	{
		//printf_point(b[0].local);
		//printf_point(b[1].local);
		z_drawline(sdl, b[0].local, b[1].local, triangle.clr, q[0].w);
		while (b[0].local.y == b[1].local.y)
			step_bresenham(&(b[0]));
		while (b[1].local.y != b[0].local.y)
			step_bresenham(&(b[1]));
	}
	z_drawline(sdl, b[0].local, b[1].local, triangle.clr, q[0].w);
}

int flerp(int from, int to, float delta)
{
	return (from + ((to - from) * delta));
}

static void fill_tri_bot(t_sdlcontext sdl, t_point *p, float z)
{
	float index;
	float max;
	t_point a;
	t_point b;

	if (p[2].x < p[1].x)
		ft_swap_x(&p[1], &p[2]);
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

	if (p[2].x < p[1].x)
		ft_swap_x(&p[1], &p[2]);
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

void	z_fill_tri(t_sdlcontext sdl, t_triangle triangle)
{
	t_quaternion	split; 	//Vector that's used to form the subtriangles.
	//t_quaternion	sorted[3];
	t_quaternion	*q;
	t_point			p[3];
	float			lerp;

	//sort_tris(ft_memcpy(sorted, triangle.p, sizeof(t_quaternion)));
	q = triangle.p;
	sort_tris(q);
	lerp = (float)(q[1].v.y - q[2].v.y) / (float)(q[0].v.y - q[2].v.y);
	split.v.x = q[2].v.x + (lerp * (q[0].v.x - q[2].v.x));
	split.v.y = q[1].v.y;
	split.v.z = q[1].w;

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

	//z_fill_sub_tri(sdl, triangle, (t_quaternion[3]){q[0], q[1], split});
	//z_fill_sub_tri(sdl, triangle, (t_quaternion[3]){q[2], q[1], split});
}
