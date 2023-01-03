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

typedef struct s_bitmask
{
	__uint128_t			*bitmask;
	float				*dist;
	t_point				chunk_size; // (currently shows how many chunks in x y axis)should store how wide each chunk is in x and y axis make another that has chunk amount on x and y axis
}	t_bitmask;

#endif