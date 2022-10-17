/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 05:48:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/17 20:05:37 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"
#include "shapes.h"

void	draw(uint32_t *pxls, t_point pos, uint32_t clr)
{
	if (pos.x < 0 || pos.x >= WINDOW_W
		|| pos.y < 0 || pos.y >= WINDOW_H)
		return ;
	pxls[pos.x + pos.y * WINDOW_W] = clr;
}

# define CRCL_SIDES 16

void	drawcircle(uint32_t *pxls, t_point pos, int size, uint32_t clr)
{
	t_point	edges[CRCL_SIDES + 1];
	int		i;
	float	angl;

	i = 0;
	angl = 0.0f;
	while (i < CRCL_SIDES + 1)
	{
		edges[i].x = pos.x + (sin(angl) * size);
		edges[i].y = pos.y + (cos(angl) * size);
		if (i >= 1)
			drawline(pxls, edges[i - 1], edges[i], clr);
		angl += FULLRAD / CRCL_SIDES;
		i++;
	}
}

//Draws an unfilled rectangle 'rect'
void	drawrectangle(uint32_t *pxls, t_rectangle rect, uint32_t clr)
{
	t_point	from;
	t_point	to;

	from = rect.position;
	to = point_add(from, (t_point){rect.size.x, 0});
	drawline(pxls, from, to, clr); 
	to = point_add(from, (t_point){0, rect.size.y});
	drawline(pxls, from, to, clr); 
	from = point_add(rect.position, rect.size);
	to = point_add(from, (t_point){0, -rect.size.y});
	drawline(pxls, from, to, clr);
	to = point_add(from, (t_point){-rect.size.x, 0});
	drawline(pxls, from, to, clr);
}

void	imgtoscreen(uint32_t *pxls, t_img *img)
{
	t_point		p;
	uint32_t	clr;

	p.y = 0;
	while(p.y < img->size.y)
	{
		p.x = 0;
		while(p.x < img->size.x)
		{
			clr = img->data[p.x + p.y * img->size.x]; //TODO: sampleimg funtion;
			if (clr != 0)
				draw(pxls, p, clr); //TODO: sampleimg function
			p.x++;
		}
		p.y++;
	}
}

void	drawline(uint32_t *pxls, t_point from, t_point to, uint32_t clr)
{
	static t_bresenham	b;

	populate_bresenham(&b, from, to);
	draw(pxls, b.local, clr);
	while (step_bresenham(&b) != 1)
		draw(pxls, b.local, clr);
	draw(pxls, b.local, clr);
}