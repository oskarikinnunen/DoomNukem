#ifndef LIGHTING_H
# define LIGHTING_H

#include "render_utils.h"

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

typedef struct s_lightmap
{
	bool		dynamic;
	uint32_t	progress; //tri_index
	bool		done;
	t_point		size;
	uint8_t		dynamic_data;
	//uint32_t	color;
	uint8_t		*data;
	//uint8_t		*draw_buffer;
}	t_lightmap;

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
	//
	t_lightmap			*lightmap;
	uint16_t			entity_id;
	bool				*drawnbuff;
	t_triangle_polygon	**triangles;
    struct s_entity		**entities;
	uint32_t			entities_count;
	t_pointlight		*pointlight;
	uint8_t				ambient_light;
	bool				calculated;
}	t_lighting;

#endif