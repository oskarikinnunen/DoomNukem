#ifndef LIGHTING_H
# define LIGHTING_H

#include "render_utils.h"

typedef struct s_map
{
	uint32_t	*data;
	uint32_t	*lightmap;
	t_point		img_size;
	t_point		size;
}	t_map;

typedef enum e_cubemap_state
{
	cm_default,
	cm_1,
	cm_2,
	cm_3,
	cm_4,
	cm_5,
	cm_6
}	t_cubemap_state;

typedef struct s_cubemap
{
	t_vector2	resolution;
	float		*shadowmaps[6];
	t_camera	cameras[6];
}	t_cubemap;

typedef struct	s_light
{
	float			radius;
	t_vector3		origin;
	t_cubemap_state	cm_state;	
	t_cubemap		cubemap;
	bool			ignoreself;
}	t_light;

typedef struct s_pointlight
{
	t_vector3	origin;
	float		radius;
	bool		shadows;
	bool		done;
	bool		ignoreself;
	uint32_t	lastmovetime;
}	t_pointlight;

typedef struct s_lighting
{
	t_triangle			*world_triangles;
	uint32_t			end;
	t_camera			camera;
	float				*zbuffer;
	t_v2rectangle		screen_edge;
	struct s_world		*world;
	t_map				*map;
	uint32_t			*img;
	t_vector2			resolution;
	t_light				*light;
	struct s_entity		*entity;
}	t_lighting;

#endif