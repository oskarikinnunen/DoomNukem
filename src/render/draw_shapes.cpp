/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_shapes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 21:39:40 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 11:57:09 by vlaine           ###   ########.fr       */
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

void	draw_triangle(t_triangle_draw td)
{
	drawline(*(td.sdl), td.p1, td.p2, td.clr);
	drawline(*(td.sdl), td.p2, td.p3, td.clr);
	drawline(*(td.sdl), td.p3, td.p1, td.clr);
}

static bool	clip_line_to_plane(
	t_point *from, t_point *to, t_vector2 plane_p, t_vector2 plane_n)
{
	float	lerp;
	float	dist[2];

	dist[0] = vector2_fdist_to_plane(point_to_vector2(*from), plane_n, plane_p);
	dist[1] = vector2_fdist_to_plane(point_to_vector2(*to), plane_n, plane_p);
	if (dist[0] < 0.0f && dist[1] < 0.0f)
		return (false);
	if (dist[0] < 0.0f || dist[1] < 0.0f)
	{
		if (dist[0] > dist[1])
			ft_swap(from, to, sizeof(t_point));
		lerp = vector2_line_intersect_plane(plane_p, plane_n, \
		point_to_vector2(*from), point_to_vector2(*to));
		*from = vector2_to_point(\
		vector2_lerp(point_to_vector2(*from), point_to_vector2(*to), lerp));
	}
	return (true);
}

static bool	screen_edge_line_clip(t_sdlcontext *sdl, t_point *from, t_point *to)
{
	t_vector2	screen_edge;

	screen_edge = (t_vector2){sdl->window_w - 1, sdl->window_h - 1};
	if (!clip_line_to_plane(from, to, vector2_zero(), (t_vector2){0.0f, 1.0f}))
		return (false);
	if (!clip_line_to_plane(from, to, screen_edge, (t_vector2){0.0f, -1.0f}))
		return (false);
	if (!clip_line_to_plane(from, to, vector2_zero(), (t_vector2){1.0f, 0.0f}))
		return (false);
	if (!clip_line_to_plane(from, to, screen_edge, (t_vector2){-1.0f, 0.0f}))
		return (false);
	return (true);
}

void	drawline(t_sdlcontext sdl, t_point from, t_point to, uint32_t clr)
{
	static t_bresenham	b;

	if (point_cmp(from, to))
		return ;
	if (screen_edge_line_clip(&sdl, &from, &to) == false)
		return ;
	populate_bresenham(&b, from, to);
	draw(sdl, b.local, clr);
	while (step_bresenham(&b) != 1)
		draw(sdl, b.local, clr);
	draw(sdl, b.local, clr);
}
