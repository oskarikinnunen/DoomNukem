/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmask_culling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 13:07:13 by vlaine            #+#    #+#             */
/*   Updated: 2023/04/02 01:23:12 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

uint16_t mask_x(int x, int left_x, int right_x)
{
	const uint16_t	row = ~0;
	return(((row >> ft_clamp(left_x - x, 0, 16))& ~(row >> ft_clamp(right_x - x, 0, 16))));
}

static t_tile update_tile(float aw, t_tile tile, uint64_t res)
{
	float dist1t;
	float dist0t;

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

void render_bitmask_row(int ax, int bx, float aw, float bw, int y, t_sdlcontext *sdl)
{
	int chunk = (y / 8) * (sdl->bitmask.bitmask_chunks.x) + (ax / 16);
	int end_chunk = (y / 8) * (sdl->bitmask.bitmask_chunks.x) + (bx / 16);
	int wchunk = chunk * 2;
	float wstep = (bw - aw) / (float)(bx - ax);
	wstep *= 8.0f;
	int row;
	int x;
	uint16_t res;

	if (ax == bx)
		wstep = 0;
	x = ax - (ax % 16);
	row = ((y % 8) * 8);
	bx++;
	while (chunk <= end_chunk)
	{
		res = mask_x(x, ax, bx);
		sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], ((uint64_t)((res >> 8) & 0xFF)) << row);
		wchunk++;
		aw += wstep;
		sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], ((uint64_t)(res & 0xFF)) << row);
		wchunk++;
		aw += wstep;
		x += 16;
		chunk++;
	}
}
