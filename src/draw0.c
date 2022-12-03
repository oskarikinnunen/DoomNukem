/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 05:48:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/03 07:18:00 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"
#include "shapes.h"

void	draw(t_sdlcontext sdl, t_point pos, uint32_t clr)
{
	if (pos.x < 0 || pos.x >= sdl.window_w - 2
		|| pos.y < 0 || pos.y >= sdl.window_h - 2)
		return ;
	((uint32_t *)sdl.surface->pixels)[pos.x + (pos.y * sdl.window_w)] = clr;
}

void	screen_blank(t_sdlcontext sdl)
{
	bzero(sdl.surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
	bzero(sdl.zbuffer, sizeof(float) * sdl.window_h * sdl.window_w);
}

# define CRCL_SIDES 16

void	drawcircle(t_sdlcontext sdl, t_point pos, int size, uint32_t clr)
{
	t_point	edges[CRCL_SIDES + 1];
	int		i;
	float	angl;

	i = 0;
	angl = 0.0f;
	while (i < CRCL_SIDES + 1)
	{
		edges[i].x = pos.x + (sinf(angl) * size);
		edges[i].y = pos.y + (cosf(angl) * size);
		if (i >= 1)
			drawline(sdl, edges[i - 1], edges[i], clr);
		angl += FULLRAD / CRCL_SIDES;
		i++;
	}
}

//Draws an unfilled rectangle 'rect'
void	drawrectangle(t_sdlcontext sdl, t_rectangle rect, uint32_t clr)
{
	t_point	from;
	t_point	to;

	from = rect.position;
	to = point_add(from, (t_point){rect.size.x, 0});
	drawline(sdl, from, to, clr); 
	to = point_add(from, (t_point){0, rect.size.y});
	drawline(sdl, from, to, clr);
	from = point_add(rect.position, rect.size);
	to = point_add(from, (t_point){0, -rect.size.y});
	drawline(sdl, from, to, clr);
	to = point_add(from, (t_point){-rect.size.x, 0});
	drawline(sdl, from, to, clr);
}

void	draw_rectangle_filled(t_sdlcontext sdl, t_rectangle rect, uint32_t clr)
{
	t_point	p;
	int		i;

	p = rect.position;

	i = 0;
	while (p.x < rect.position.x + rect.size.x)
	{
		p.y = rect.position.y;
		while (p.y < rect.position.y + rect.size.y)
		{
			//if (p.x % 2 == 0 && p.y % 2 == 0)
			draw(sdl, p, clr);
			p.y++;
			i++;
		}
		p.x++;
		i++;
	}
}

void	draw_triangle(t_sdlcontext *sdl, t_point p1, t_point p2, t_point p3, uint32_t clr)
{
	drawline(*sdl, p1, p2, clr);
	drawline(*sdl, p2, p3, clr);
	drawline(*sdl, p3, p1, clr);
}


/*void	draw_triangle_filled(t_sdlcontext *sdl, t_point p1, t_point p2, t_point p3, uint32_t clr)
{
	drawline(*sdl, p1, p2, clr);
	drawline(*sdl, p2, p3, clr);
	drawline(*sdl, p3, p1, clr);
}*/

void	draw_image(t_sdlcontext sdl, t_point pos, t_img img, t_point scale)
{
	t_point		pixel;
	t_point		sample;
	t_vector2	scalar;
	uint32_t	color;

	if (img.size.x == 0 || img.size.y == 0 || img.length == 0)
		return ;
	//scale.y += 2; //LUL this is to fix the shitty png reader
	scalar.x = ((float)img.size.x / scale.x);
	scalar.y = ((float)img.size.y / scale.y);
	pixel.y = 0;
	while (pixel.y < scale.y) //TODO: it's the png readers fault that the second to last line is garbled, for now it just stops early, hence the '- 2'
	{
		pixel.x = 0;
		if (pixel.y + pos.y < 0 || pixel.y + pos.y >= sdl.window_h - 1)
		{
			pixel.y++;
			continue;
		}
		while (pixel.x < scale.x)
		{
			if (pixel.x + pos.x < 0 || pixel.x + pos.x >= sdl.window_w - 1)
			{
				pixel.x++;
				continue;
			}
			sample.x = ft_clamp(pixel.x * scalar.x, 0, img.size.x - 1);
			sample.y = ft_clamp(pixel.y * scalar.y, 0, img.size.y - 1);
			color = img.data[sample.x + (sample.y * img.size.x)];
			draw(sdl, point_add(pos, pixel), color);
			pixel.x++;
		}
		pixel.y++;
	}
}

void	drawline(t_sdlcontext sdl, t_point from, t_point to, uint32_t clr)
{
	static t_bresenham	b;

	populate_bresenham(&b, from, to);
	draw(sdl, b.local, clr);
	while (step_bresenham(&b) != 1)
		draw(sdl, b.local, clr);
	draw(sdl, b.local, clr);
}
