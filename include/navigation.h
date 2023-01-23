#ifndef NAVIGATION_W
# define NAVIGATION_W

#include "vectors.h"

typedef struct s_navnode
{
	t_vector3	vertex[3];
	t_vector3	mid_point;
	t_vector3	enter_point;
	t_vector3	line_point[100];
	uint32_t	neighbors;
	uint32_t	neighbors_id[100];
	uint32_t	parent;
	float		f;
	float		g;
	float		h;
	bool		valid;
	bool		visited;
	bool		blocked;
	uint32_t	index;
}	t_navnode;

typedef struct s_path
{
	bool		valid_path;
	t_navnode	path[32];
	uint32_t	ai;
	uint32_t	bi;
	uint32_t	start;
	uint32_t	end;
	t_vector3	target;
}	t_path;

typedef struct s_navigation
{
	uint32_t	node_amount;
	t_navnode	*navmesh;
	t_navnode	*openlist;
	uint32_t	malloc_size;
	float		clip_size;
	bool		show_navmesh;
}	t_navigation;
# endif