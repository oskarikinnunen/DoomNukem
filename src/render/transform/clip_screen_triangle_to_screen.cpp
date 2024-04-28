/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clip_screen_triangle_to_screen.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 19:15:23 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 20:23:27 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	screen_edge_clip(t_screen_triangle *buff, int side,
t_v2_rectangle screen_edge, t_screen_triangle triangle)
{
	int	tricount;

	if (side == 0)
		tricount = clip_screen_triangle_plane(screen_edge.min, \
		(t_vector2){0.0f, 1.0f}, triangle, buff);
	else if (side == 1)
		tricount = clip_screen_triangle_plane(screen_edge.max, \
		(t_vector2){0.0f, -1.0f}, triangle, buff);
	else if (side == 2)
		tricount = clip_screen_triangle_plane(screen_edge.min, \
		(t_vector2){1.0f, 0.0f}, triangle, buff);
	else
		tricount = clip_screen_triangle_plane(screen_edge.max, \
		(t_vector2){-1.0f, 0.0f}, triangle, buff);
	return (tricount);
}

t_point	clip_screen_triangle_against_screen_edge(t_screen_triangle *triangles,
t_v2_rectangle screen_edge)
{
	t_screen_triangle	clipped[2];
	t_point				start_end;
	int					p;
	int					nnewtriangles;
	int					ntristoadd;

	start_end = (t_point){0, 1};
	nnewtriangles = 1;
	p = 0;
	while (p < 4)
	{
		ntristoadd = 0;
		while (nnewtriangles > 0)
		{
			nnewtriangles--;
			ntristoadd = screen_edge_clip(
					clipped, p, screen_edge, triangles[start_end.x++]);
			ft_memcpy(&triangles[start_end.y], clipped,
				ntristoadd * sizeof(t_screen_triangle));
			start_end.y += ntristoadd;
		}
		nnewtriangles = start_end.y - start_end.x;
		p++;
	}
	return (start_end);
}
