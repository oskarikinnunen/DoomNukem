/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 14:59:10 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 17:31:12 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

inline static void	set_square_from_triangle(
	t_square *s, t_screen_triangle t, float z_dist[2])
{
	int			i;
	float		dist;
	t_vector2	v;

	i = 0;
	while (i < 3)
	{
		v = t.p[i];
		s->max.x = ft_max(s->max.x, v.x);
		s->max.y = ft_max(s->max.y, v.y);
		s->min.x = ft_min(s->min.x, v.x);
		s->min.y = ft_min(s->min.y, v.y);
		dist = 1.0f / t.t[i].z;
		z_dist[0] = ft_maxf(z_dist[0], dist);
		z_dist[1] = ft_minf(z_dist[1], dist);
		i++;
	}
}

void	set_square_from_triangles(
	t_occlusion *occl, t_screen_triangle *t, int count)
{
	t_square	*s;
	t_vector2	v;
	float		dist;
	int			i;

	if (count <= 0)
		return ;
	s = &occl->box;
	s->max = vector2_to_point(t[0].p[0]);
	s->min = vector2_to_point(t[0].p[0]);
	dist = 1.0f / t[0].t[0].z;
	occl->z_dist[0] = dist;
	occl->z_dist[1] = dist;
	i = 0;
	while (i < count)
	{
		set_square_from_triangle(s, t[i], occl->z_dist);
		i++;
	}
}

bool	is_screen_full(t_sdlcontext *sdl)
{
	float	dist;
	int		i;

	dist = sdl->bitmask.last_max_dist + 1000.0f;
	i = 0;
	while (i < sdl->bitmask.bitmask_chunks.x * sdl->bitmask.bitmask_chunks.y)
	{
		if (sdl->bitmask.tile[i].max0 == dist)
			return (false);
		i++;
	}
	return (true);
}
