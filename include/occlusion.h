#ifndef OCCLUSION_H
# define OCCLUSION_H

#include "vectors.h"

typedef struct s_boundingbox //will be deprecated
{
	t_vector3		boundingbox[8];
	float			height;
	bool			is_wall;
}	t_boundingbox;

typedef enum e_bound_type
{
	box,
	plane,
	sphere
}	t_bound_type;

typedef struct s_bounds
{
	t_vector3		box[8];
	t_vector3		plane[4];
	float			sphere;
	t_bound_type	bound_type;
}	t_bounds;

typedef enum e_occlusion_cull
{
	ignore,
	occlude,
	cull,
	occlude_and_cull,
}	t_occlusion_cull;

typedef struct s_occlusion
{
	t_bounds			bounds;
	t_occlusion_cull	occlusion_cull;
	bool				backface_cull;
	bool				is_occluded;
	int32_t				id;
}	t_occlusion;

#endif