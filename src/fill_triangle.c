/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/24 16:30:23 by vlaine           ###   ########.fr       */
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
		((uint32_t *)sdl.surface->pixels)[pos.x + pos.y * WINDOW_W] = INT_MAX;
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
	//printf("\nz_fill_sub_tri\n");
//	printf_quat(p[0]);
//	printf_quat(p[1]);
//	printf_quat(p[2]);
	int y = q[1].v.y;
	populate_bresenham(&(b[0]), (t_point){q[0].v.x, q[0].v.y}, (t_point){q[1].v.x, q[1].v.y});
	populate_bresenham(&(b[1]), (t_point){q[0].v.x, q[0].v.y}, (t_point){q[2].v.x, q[2].v.y});
	//printf_point(b[0].local);
	//printf_point(b[1].local);
	//printf("%f\n", y);
	while (b[0].local.y != y)
	{
//		printf("x %d y %d\n", b[0].local.y, b[1].local.y);
		z_drawline(sdl, b[0].local, b[1].local, triangle.clr, q[0].w);
		while (b[0].local.y == b[1].local.y)
			step_bresenham(&(b[0]));
		while (b[1].local.y != b[0].local.y)
			step_bresenham(&(b[1]));
	}
	z_drawline(sdl, b[0].local, b[1].local, triangle.clr, q[0].w);
}

//delta could be uint8 and it could go from 0 to 100; if we dont need that much accuracy
t_point point_lerp(t_point from, t_point to, float delta)
{
	t_point result;

	result.x = from.x + ((to.x - from.x) * delta);
	result.y = from.y + ((to.y - from.y) * delta);

	return (result); // might return - if from is greater than to
}

//delta could be uint8 and it could go from 0 to 100; if we dont need that much accuracy
t_point quaternion_lerp(t_quaternion from, t_quaternion to, float delta)
{
	t_point result;

	printf("lerp x %f y %f\n", from.v.x + ((to.v.x - from.v.x) * delta), from.v.y + ((to.v.y - from.v.y) * delta));
	result.x = roundf(from.v.x + ((to.v.x - from.v.x) * delta));
	result.y = roundf(from.v.y + ((to.v.y - from.v.y) * delta));
	printf_point(result);

	return (result); // might return - if from is greater than to
}


static void fill_triangle(t_sdlcontext sdl, t_quaternion q[3]) 
{
	float index = 1;
	float max1;
	float max2;
	//t_point from = (t_point){q[0].v.x, q[0].v.y}, (t_point){q[1].v.x, q[1].v.y};
	//t_point to = (t_point){q[0].v.x, q[0].v.y}, (t_point){q[2].v.x, q[2].v.y};
	t_point p[3];
	t_point p1;
	t_point p2;
	p[0].x = q[0].v.x;
	p[0].y = q[0].v.y;
	p[1].x = q[1].v.x;
	p[1].y = q[1].v.y;
	p[2].x = q[2].v.x;
	p[2].y = q[2].v.y;
	max1 = p[1].y - p[2].y;
	printf_quat(q[0]);
	printf_quat(q[1]);
	printf_quat(q[2]);
	printf_point(p[0]);
	printf_point(p[1]);
	printf_point(p[2]);
	printf("\n");
	max1 = q[0].v.y - q[1].v.y;
	max2 = q[0].v.y - q[2].v.y;
	index = 1.0f;
	while (index < max1)
	{
		drawline(sdl.surface->pixels, quaternion_lerp(q[1], q[0], index / max1), quaternion_lerp(q[2], q[0], ((index + (q[1].v.y - q[2].v.y))) / max2), INT_MAX);
		index++;
	}
	/*
	index = p[0].y - p[1].y;
	maxx = index;
	maxy = p[0].y - p[2].y;
	while(index > 0)
	{
		p1 = point_lerp(p[1], p[0], (float)index / maxx);
		p2 = point_lerp(p[2], p[0], (float)(index) / maxy);
		printf_point(p1);
		printf_point(p2);
		printf("index %d\n", index);
		drawline(sdl.surface->pixels, p1, p2, INT_MAX);
		index--;
	}
	*/
}

void	z_fill_tri(t_sdlcontext sdl, t_triangle triangle)
{
	t_quaternion	split; 	//Vector that's used to form the subtriangles.
	//t_quaternion	sorted[3];
	t_quaternion	*p;
	float			lerp;

	//sort_tris(ft_memcpy(sorted, triangle.p, sizeof(t_quaternion)));
	p = triangle.p;
	sort_tris(p);
	lerp = (float)(p[1].v.y - p[2].v.y) / (float)(p[0].v.y - p[2].v.y);
	split.v.x = p[2].v.x + (lerp * (p[0].v.x - p[2].v.x));
	split.v.y = p[1].v.y;
	split.v.z = p[1].w;
	//sorted[1].v.z = triangle.p[0].v.z + triangle.p[1].v.z + triangle.p[2].v.z;
	// sorted[1].v.z -= 10000;
//	printf_tri(triangle);
//	printf_quat(p[0]);
//	printf_quat(p[1]);
//	printf_quat(p[2]);
//	printf_quat(split);
	fill_triangle(sdl, (t_quaternion[3]){p[0], p[1], split});
	//z_fill_sub_tri(sdl, triangle, (t_quaternion[3]){p[0], p[1], split});
	//printf("it works yay!!!!\n");
	//exit(0);
	//z_fill_sub_tri((int *[3]){(int *)&(sorted[0]), (int *)&(sorted[1]), (int *)&split}, sdl, clr);
	//z_fill_sub_tri((int *[3]){(int *)&(sorted[2]), (int *)&(sorted[1]), (int *)&split}, sdl, clr);
	//z_drawline(sdl, (t_point){sorted[0][X], sorted[0][Y]}, (t_point){sorted[2][X], sorted[2][Y]}, clr, tris[2][Z]);
}
