/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 05:48:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 20:00:29 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

void	draw(uint32_t *pxls, t_point pos, uint32_t clr)
{
	if (pos.x < 0 || pos.x >= WINDOW_W
		|| pos.y < 0 || pos.y >= WINDOW_H)
		return ;
	pxls[pos.x + (pos.y * WINDOW_W)] = clr;
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

void	draw_image(uint32_t *pxls, t_point pos, t_img img, t_point scale)
{
	t_point		pixel;
	t_point		sample;
	t_vector2	scalar;
	uint32_t	color;

	if (img.size.x == 0 || img.size.y == 0 || img.length == 0)
		return ;
	scalar.x = ((float)img.size.x / scale.x);
	scalar.y = ((float)img.size.y / scale.y);
	pixel.y = 0;
	while (pixel.y < scale.y - 2) //TODO: it's the png readers fault that the second to last line is garbled, for now it just stops early, hence the '- 2'
	{
		pixel.x = 0;
		if (pixel.y + pos.y < 0 || pixel.y + pos.y > WINDOW_H)
			continue;
		while (pixel.x < scale.x - 1)
		{
			if (pixel.x + pos.x < 0 || pixel.x + pos.x >= WINDOW_W)
				continue;
			sample.x = ft_clamp(pixel.x * scalar.x, 0, img.size.x - 1);
			sample.y = ft_clamp(pixel.y * scalar.y, 0, img.size.y - 1);
			color = img.data[sample.x + sample.y + (sample.y * img.size.x)]; //TODO: NO idea why y needs to be there twice. Probably the png readers fault aswell
			draw(pxls, point_add(pos, pixel), color);
			pixel.x++;
		}
		pixel.y++;
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
