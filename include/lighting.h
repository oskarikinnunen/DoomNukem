#ifndef LIGHTING_H
# define LIGHTING_H

#include "vectors.h"

typedef struct s_triangle_polygon //TODO: Renam this to light triangle or smt similar
{
	t_point		p2[3];
	t_vector3	p3[3];
	t_vector2	uv[3];
}	t_triangle_polygon;

typedef struct s_map
{
	uint32_t	*data;
	t_point		img_size;
	t_point		size;
}	t_map;

typedef struct s_lightmap
{
	t_point		size;
	uint8_t		*data;
}	t_lightmap;

typedef struct s_pointlight
{
	t_vector3	origin;
	float		radius;
	bool		shadows;
}	t_pointlight;

typedef struct s_lighting
{
	t_lightmap			*lightmap;
	bool				*map;
	t_triangle_polygon	**triangles;
    struct s_entity		**entities;
	uint32_t			entities_count;
	t_pointlight		*pointlight;
}	t_lighting;

#endif