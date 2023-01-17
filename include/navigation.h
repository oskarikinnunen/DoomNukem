#ifndef NAVIGATION_W
# define NAVIGATION_W

#include "vectors.h"

typedef struct s_navnode
{
	t_vector3	vertex[3];
	t_vector3	mid_point;
	t_vector3	line_point[3];
	uint32_t	neighbors;
	uint32_t	neighbors_id[20];
	uint32_t	parent;
	float		f;
	float		g;
	float		h;
	bool		valid;
	bool		visited;
	bool		blocked;
}	t_navnode;

typedef struct s_path
{
	t_navnode	path[32];
	uint32_t	ai;
	uint32_t	bi;
	uint32_t	start;
	uint32_t	end;
	t_vector3	target;
}	t_path;
# endif