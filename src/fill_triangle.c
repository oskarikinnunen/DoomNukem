/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/03 20:02:54 by okinnune         ###   ########.fr       */
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

static void step_ab(t_texture *step, t_triangle triangle, float delta)
{
	t_quaternion	*q;
	t_texture		*t;

	q = triangle.p;
	t = triangle.t;
	
	step->u = (t[2].u - t[1].u) * delta;
	step->v = (t[2].v - t[1].v) * delta;
	step->w = (t[2].w - t[1].w) * delta;
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
				index = 1;
				((uint32_t *)sdl.surface->pixels)[(int)(i) + (int)q[1].v.y * sdl.window_w]
					= img->data[img->size.x * (int)((t[0].u / t[0].w) * img->size.x - 1) + (int)((t[0].v / t[0].w) * img->size.y - 1)];
			}
			else
				index++;
			t[0].w += t_step[2].w;
			i++;
		}
		triangle = step_triangle(triangle, x_step, t_step);
		q[1].v.y--;
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
				index = 1;
				((uint32_t *)sdl.surface->pixels)[(int)(i) + (int)q[1].v.y * sdl.window_w] = img->data[img->size.x * (int)((t[0].u / t[0].w) * img->size.x - 1) + (int)((t[0].v / t[0].w) * img->size.y - 1)];
			}
			else
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
	q_split.v.z = q[1].w;

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
