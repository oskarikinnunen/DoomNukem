#ifndef NAVIGATION_W
# define NAVIGATION_W

#include "vectors.h"

typedef struct s_navnode
{
	t_vector3	vertex[3];
	t_vector3	mid_point;
	uint32_t	neighbors;
	uint32_t	neighbors_id[3];
	uint32_t	parent;
	float		f;
	float		g;
	float		h;
	bool		valid;
	bool		visited;
	bool		blocked;
}	t_navnode;

# endif