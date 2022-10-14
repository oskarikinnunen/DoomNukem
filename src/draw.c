/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 05:48:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 15:22:37 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

int isvalid(int crd[2]);

int isvalid(int crd[2])
{
	if (crd[X] < 0 || crd[X] >= WINDOW_W
		|| crd[Y] < 0 || crd[Y] >= WINDOW_H)
		return (0);
	return(1);
}

void	draw(uint32_t *pxls, int crd[2], uint32_t clr) // TODO: Implement zbuffer into draw function/Make a struct for draw function that has point, clr, zbuffer
{
	//printf("window size is %d\n", (crd[X] + crd[Y] * WINDOW_W) & (WINDOW_H * WINDOW_W - 1));
	
	if ((crd[X] & (1023)) == crd[X] && (crd[Y] & (1023)) == crd[Y])
		pxls[crd[X] + crd[Y] * WINDOW_W] = clr;

}

# define CRCL_SIDES 16

void	drawcircle(uint32_t *pxls, int crd[2], int size, uint32_t clr)
{
	int		edges[CRCL_SIDES][2];
	int		i;
	float	angl;

	i = 0;
	angl = 0.0f;
	while (i <= CRCL_SIDES)
	{
		edges[i][X] = crd[X] + (sin(angl) * size);
		edges[i][Y] = crd[Y] + (cos(angl) * size);
		if (i > 0)
			drawline(pxls, edges[i - 1], edges[i], clr);
		angl += FULLRAD / CRCL_SIDES;
		i++;
	}
}

void	imgtoscreen(uint32_t *pxls, t_img *img)
{
	int			p[2];
	uint32_t	clr;

	p[Y] = 0;
	while(p[Y] < img->size[Y])
	{
		p[X] = 0;
		while(p[X] < img->size[X])
		{
			clr = img->data[p[X] + p[Y] * img->size[X]]; //TODO: sampleimg funtion;
			if (clr != 0)
				draw(pxls, p, clr); //TODO: sampleimg function
			p[X]++;
		}
		p[Y]++;
	}
}

void	drawline(uint32_t *pxls, int from[2], int to[2], uint32_t clr)
{
	static t_bresenham	b;

	populate_bresenham(&b, from, to);
	draw(pxls, b.local, clr);
	while (step_bresenham(&b) != 1)
		draw(pxls, b.local, clr);
	draw(pxls, b.local, clr);
}
