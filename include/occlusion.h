#ifndef OCCLUSION_H
# define OCCLUSION_H

#include "vectors.h"

typedef struct s_occluder
{
	t_point			vector[2]; // position vector matches normal len
	t_vector2		normal[3]; // which way the occluder plane points to in 2d screenspace in theory should be able to do this in 3d space, saving time
}	t_occluder;

typedef struct s_occlusion
{
	t_occluder			occluder[12];
	uint32_t			occluder_count;
	bool				occlude;
	bool				cull;
	bool				is_backface_cull;
	bool				is_occluded;
}	t_occlusion;

typedef struct s_bit_tri
{
	t_point	p[3];
	float	w[3];
}	t_bit_tri;

typedef struct s_tile
{
	float		max1; // closer one
	float		max0; // further one
	uint64_t	mask;
}	t_tile;

typedef struct s_bitmask
{
	__uint128_t			*bitmask;
	//float				*dist;
	t_point				bitmask_chunks; //amount of bitmask chunks in x and y axis
	t_point				tile_chunks; //amount of tile chunks in x and y axis
	t_tile				*tile;
}	t_bitmask;

#endif