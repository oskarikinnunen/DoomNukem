#ifndef LIGHTING_H
# define LIGHTING_H

#include "render_utils.h"

# define LIGHT_AMBIENT 0.15f

typedef struct s_map
{
	uint32_t	*texture;
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
	t_vector3		world_position;
	t_cubemap_state	cm_state;
	t_cubemap		cubemap;
	bool			ignoreself;
	uint32_t		clr;
	float			intensity;
	float			ambient;
}	t_light;

typedef struct s_lighting
{
	struct s_world		*world;
	t_triangle			*world_triangles;
	t_light				*light;
	struct s_entity		*light_ent;
	bool				*overdraw;
	float				*zbuffer;
	t_map				*map;
	t_vector2			resolution;
	t_v2rectangle		screen_edge;
	t_camera			camera;
	t_vector3			triangle_normal;
}	t_lighting;

#endif
