/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/19 14:19:47 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

static void	sort_tris(int tris[3][3])
{
	int	s_x;
	int	s_j;
	int	temp[3];

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (tris[s_j][Y] < tris[s_j + 1][Y])
			{
				ft_memcpy(temp, tris[s_j], sizeof(int) * 3);
				ft_memcpy(tris[s_j], tris[s_j + 1], sizeof(int) * 3);
				ft_memcpy(tris[s_j + 1], temp, sizeof(int) * 3);
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

static void	z_fill_sub_tri(int *tris[3], t_sdlcontext sdl, uint32_t clr)
{
	t_bresenham	b[2];

	populate_bresenham(&(b[0]), (t_point){tris[0][X], tris[0][Y]}, (t_point){tris[1][X], tris[1][Y]});
	populate_bresenham(&(b[1]), (t_point){tris[0][X], tris[0][Y]}, (t_point){tris[2][X], tris[2][Y]});
	while (b[0].local.y != tris[1][Y])
	{
		drawline((u_int32_t *)sdl.surface->pixels, b[0].local, b[1].local, clr);
		while (b[0].local.y == b[1].local.y)
			step_bresenham(&(b[0]));
		while (b[1].local.y != b[0].local.y)
			step_bresenham(&(b[1]));
	}
	drawline((u_int32_t *)sdl.surface->pixels, b[0].local, b[1].local, clr);
}

void	z_fill_tri(int tris[3][3], t_sdlcontext sdl, uint32_t clr)
{
	int	split[3]; 	//Vector that's used to form the subtriangles.
	int	sorted[3][3];	//Stack memory for the sorted triangles
	float	lerp;

	sort_tris(ft_memcpy(sorted, tris, sizeof(int [3][3])));
	lerp = (float)(sorted[1][Y] - sorted[2][Y]) / (float)(sorted[0][Y] - sorted[2][Y]);
	split[X] = sorted[2][X] + (lerp * (sorted[0][X] - sorted[2][X]));
	split[Y] = sorted[1][Y];
	split[Z] = sorted[1][Z];
	z_fill_sub_tri((int *[3]){(int *)&(sorted[0]), (int *)&(sorted[1]), (int *)&split}, sdl, clr);
	z_fill_sub_tri((int *[3]){(int *)&(sorted[2]), (int *)&(sorted[1]), (int *)&split}, sdl, clr);
	drawline((u_int32_t *)sdl.surface->pixels, (t_point){sorted[0][X], sorted[0][Y]}, (t_point){sorted[2][X], sorted[2][Y]}, clr);
}
