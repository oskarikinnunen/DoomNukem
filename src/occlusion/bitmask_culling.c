/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmask_culling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 13:07:13 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 17:00:55 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

uint16_t	mask_x(int x, int left_x, int right_x)
{
	const uint16_t	row = ~0;

	return (((row >> ft_clamp(left_x - x, 0, 16)) & \
	~(row >> ft_clamp(right_x - x, 0, 16))));
}

static t_tile	update_tile(float aw, t_tile tile, uint64_t res)
{
	float	dist1t;
	float	dist0t;

	if (aw <= tile.max0)
	{
		dist1t = tile.max1 - aw;
		dist0t = tile.max0 - tile.max1;
		if (dist1t > dist0t)
		{
			tile.max1 = 0;
			tile.mask = 0;
		}
		if (tile.max1 < aw)
			tile.max1 = aw;
		tile.mask |= res;
		if (tile.mask == ~0)
		{
			tile.max0 = tile.max1;
			tile.max1 = 0;
			tile.mask = 0;
		}
	}
	return (tile);
}

void	render_bitmask_row(
	t_point x_line, t_vector2 w_line, int y, t_sdlcontext *sdl)
{
	t_bitmask_helper	bh;

	bh.chunk = (y / 8) * (sdl->bitmask.bitmask_chunks.x) + (x_line.x / 16);
	bh.end_chunk = (y / 8) * (sdl->bitmask.bitmask_chunks.x) + (x_line.y / 16);
	bh.wchunk = bh.chunk * 2;
	bh.wstep = (w_line.y - w_line.x) / (float)(x_line.y - x_line.x);
	bh.wstep *= 8.0f;
	if (x_line.x == x_line.y)
		bh.wstep = 0;
	bh.x = x_line.x - (x_line.x % 16);
	bh.row = ((y % 8) * 8);
	x_line.y++;
	while (bh.chunk <= bh.end_chunk)
	{
		bh.res = mask_x(bh.x, x_line.x, x_line.y);
		sdl->bitmask.tile[bh.wchunk] = update_tile(w_line.x, sdl->bitmask.\
		tile[bh.wchunk++], ((uint64_t)((bh.res >> 8) & 0xFF)) << bh.row);
		w_line.x += bh.wstep;
		sdl->bitmask.tile[bh.wchunk] = update_tile(w_line.x, sdl->bitmask.\
		tile[bh.wchunk++], ((uint64_t)(bh.res & 0xFF)) << bh.row);
		w_line.x += bh.wstep;
		bh.x += 16;
		bh.chunk++;
	}
}
