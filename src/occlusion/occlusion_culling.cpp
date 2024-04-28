/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   occlusion_culling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:08:43 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 14:25:26 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	clamp_square(t_square *square, t_point max)
{
	square->max.x = ft_clamp(square->max.x, 0, max.x);
	square->max.y = ft_clamp(square->max.y, 0, max.y);
	square->min.x = ft_clamp(square->min.x, 0, max.x);
	square->min.y = ft_clamp(square->min.y, 0, max.y);
}

static void	set_max_square_from_bitmask(
	t_entity *e, t_square bounds, float w, t_sdlcontext *sdl)
{
	int	y;
	int	x;

	y = bounds.min.y;
	while (y <= bounds.max.y)
	{
		x = bounds.min.x;
		while (x <= bounds.max.x)
		{
			if (w <= sdl->bitmask.tile[y * sdl->bitmask.tile_chunks.x + x].max0)
			{
				e->occlusion.is_occluded = false;
				e->occlusion.clip.max.x = ft_max(e->occlusion.clip.max.x, x);
				e->occlusion.clip.max.y = ft_max(e->occlusion.clip.max.y, y);
				e->occlusion.clip.min.x = ft_min(e->occlusion.clip.min.x, x);
				e->occlusion.clip.min.y = ft_min(e->occlusion.clip.min.y, y);
			}
			x++;
		}
		y++;
	}
}

bool	is_entity_occlusion_culled(
	t_sdlcontext *sdl, t_render *render, t_entity *ent)
{
	t_square			chunk;
	const __uint128_t	max = ~0;
	int					y;
	int					x;

	chunk = ent->occlusion.box;
	chunk.min.x = floorf((float)chunk.min.x / 8.0f);
	chunk.max.x = ceilf((float)chunk.max.x / 8.0f);
	chunk.min.y = floorf((float)chunk.min.y / 8.0f);
	chunk.max.y = ceilf((float)chunk.max.y / 8.0f);
	ent->occlusion.is_occluded = true;
	clamp_square(&chunk, point_sub(sdl->bitmask.tile_chunks, point_one()));
	ent->occlusion.clip = chunk;
	set_max_square_from_bitmask(ent, chunk, ent->occlusion.z_dist[1], sdl);
	if (ent->occlusion.is_occluded == false)
	{
		ent->occlusion.clip.max = point_add_xy(ent->occlusion.clip.max, 1);
		ent->occlusion.clip.min = point_mul(ent->occlusion.clip.min, 8);
		ent->occlusion.clip.max = point_mul(ent->occlusion.clip.max, 8);
		ent->occlusion.clip.max.x = ft_min(ent->occlusion.clip.max.x, \
		sdl->window_w - 1);
		ent->occlusion.clip.max.y = ft_min(ent->occlusion.clip.max.y, \
		sdl->window_h - 1);
	}
	return (ent->occlusion.is_occluded);
}

void	calculate_triangles(
	t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	render->world_triangles = entity->occlusion.world_tri;
	render->start_index = 0;
	render->screenspace_ptri_count = 0;
	render->screen_edge.max.x = \
	(float)(sdl.window_w * sdl.resolution_scaling) - 1.0f;
	render->screen_edge.max.y = \
	(float)(sdl.window_h * sdl.resolution_scaling) - 1.0f;
	render->screen_edge.min.x = 0.0f;
	render->screen_edge.min.y = 0.0f;
	render->end_index = entity->occlusion.world_tri_count - 1;
	render_world_triangle_buffer_to_screen_triangle(render, sdl);
}

void	clear_occlusion_buffer(t_sdlcontext *sdl)
{
	t_tile	temp;
	int		i;

	temp.mask = 0;
	temp.max0 = sdl->bitmask.max_dist + 1000.0f;
	temp.max1 = 0;
	i = 0;
	while (i < ((sdl->window_h * sdl->window_w) / 64))
	{
		sdl->bitmask.tile[i] = temp;
		i++;
	}
}
