/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 05:48:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 09:03:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

void	draw(uint32_t *pxls, int crd[2], uint32_t clr)
{
	if (crd[X] < 0 || crd[X] >= WINDOW_W
		|| crd[Y] < 0 || crd[Y] >= WINDOW_H)
		return ;
	pxls[crd[X] + crd[Y] * WINDOW_W] = clr;
}

void	drawcircle(uint32_t *pxls, int crd[2], int size, uint32_t clr)
{
	int		edges[6][2];
	int		i;
	float	angl;

	i = 0;
	angl = 0.0f;
	while (i <= 6)
	{
		edges[i][X] = crd[X] + (sin(angl) * size);
		edges[i][Y] = crd[Y] + (cos(angl) * size);
		if (i > 0)
			drawline(pxls, edges[i - 1], edges[i], clr);
		angl += FULLRAD / 6;
		i++;
	}
}

void	drawline(uint32_t *pxls, int from[2], int to[2], uint32_t clr)
{
	static t_bresenham	b;

	populate_bresenham(&b, from, to);
	while (step_bresenham(&b) != 1)
		draw(pxls, b.local, clr);
}