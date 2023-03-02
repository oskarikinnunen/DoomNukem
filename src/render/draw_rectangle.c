/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_rectangle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 21:35:16 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/02 21:38:09 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "shapes.h"
#include "colors.h"

//Draws an unfilled rectangle 'rect'
void	draw_rectangle(t_sdlcontext sdl, t_rectangle rect, uint32_t clr)
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

void	draw_rectangle_raster(t_sdlcontext sdl, t_rectangle rect, uint32_t clr)
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
			if (!(p.x % 2 == 0 && p.y % 2 == 0))
				draw(sdl, p, clr);
			p.y++;
			i++;
		}
		p.x++;
		i++;
	}
}

void	draw_rectangle_raster_few(t_sdlcontext sdl,
								t_rectangle rect, uint32_t clr)
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
			if ((p.x % 3 == 0 && p.y % 3 == 0))
				draw(sdl, p, clr);
			p.y++;
			i++;
		}
		p.x++;
		i++;
	}
}

void	draw_rectangle_filled(t_sdlcontext sdl, t_rectangle rect, uint32_t clr)
{
	t_point	p;
	int		i;

	p = rect.position;
	i = 0;
	if (point_cmp(rect.size, point_zero()))
		return ;
	while (p.x < rect.position.x + rect.size.x)
	{
		p.y = rect.position.y;
		while (p.y < rect.position.y + rect.size.y)
		{
			draw(sdl, p, clr);
			p.y++;
			i++;
		}
		p.x++;
		i++;
	}
}
