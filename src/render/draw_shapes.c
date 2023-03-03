/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_shapes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 21:39:40 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/02 21:46:10 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "shapes.h"
#include "bresenham.h"

void	drawcircle(t_sdlcontext sdl, t_point pos, int size, uint32_t clr)
{
	t_point	edges[CIRCLE_SIDES + 1];
	int		i;
	float	angl;

	i = 0;
	angl = 0.0f;
	while (i < CIRCLE_SIDES + 1)
	{
		edges[i].x = pos.x + (sinf(angl) * size);
		edges[i].y = pos.y + (cosf(angl) * size);
		if (i >= 1)
			drawline(sdl, edges[i - 1], edges[i], clr);
		angl += FULLRAD / CIRCLE_SIDES;
		i++;
	}
}

//TODO: make norminette compliant by using t_point_triangle
void	draw_triangle(t_sdlcontext *sdl,
					t_point p1, t_point p2, t_point p3, uint32_t clr)
{
	drawline(*sdl, p1, p2, clr);
	drawline(*sdl, p2, p3, clr);
	drawline(*sdl, p3, p1, clr);
}

void	drawline(t_sdlcontext sdl, t_point from, t_point to, uint32_t clr)
{
	static t_bresenham	b;

	if ((from.x == to.x && from.y == to.y) || from.x < 0 || to.x < 0
		|| from.x > sdl.window_w - 1 || to.x > sdl.window_w - 1 || from.y < 0
		|| to.y < 0 || from.y > sdl.window_h - 1 || to.y > sdl.window_h - 1)
		return ;
	if (from.x == to.x && from.y == to.y)
		return ;
	populate_bresenham(&b, from, to);
	draw(sdl, b.local, clr);
	while (step_bresenham(&b) != 1)
		draw(sdl, b.local, clr);
	draw(sdl, b.local, clr);
}
