#ifndef OCCLUSION_H
# define OCCLUSION_H

#include "vectors.h"
#include "shapes.h"

typedef struct s_occlusion
{
	t_square			box;//legacy
	float				z_dist[2];
	bool				is_backface_cull;
	bool				is_occluded;
}	t_occlusion;

typedef struct s_tile
{
	float		max1;
	float		max0;
	uint64_t	mask;
}	t_tile;

typedef struct s_bitmask
{
	t_point				bitmask_chunks; //amount of bitmask chunks in x and y axis
	t_point				tile_chunks; //amount of tile chunks in x and y axis
	t_tile				*tile;
	float				max_dist;
}	t_bitmask;

uint16_t mask_x(int x, int left_x, int right_x);
#endif