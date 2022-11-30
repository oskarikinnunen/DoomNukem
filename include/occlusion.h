#ifndef OCCLUSION_H
# define OCCLUSION_H

#include "vectors.h"

typedef struct s_occluder
{
	t_vector3		vector[2]; // position vector matches normal len
	t_vector3		normal[3]; // which way the occluder plane points to in 2d screenspace in theory should be able to do this in 3d space, saving time
}	t_occluder;

typedef enum e_occlusion_cull
{
	oc_ignore,
	oc_occlude,
	oc_cull,
	oc_occlude_and_cull,
}	t_occlusion_cull;

typedef struct s_occlusion
{
	t_occlusion_cull	type;
	t_occluder			occluder[12];
	uint32_t			occluder_count;
	bool				is_backface_cull;
	bool				is_occluded;
}	t_occlusion;

#endif