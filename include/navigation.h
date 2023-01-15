#ifndef NAVIGATION_W
# define NAVIGATION_W

#include "vectors.h"

typedef struct s_navcell
{
	t_vector3	vertex[3];
	t_vector3	mid_point;
	uint32_t	neighbors;
	uint32_t	neighbors_id[3];
}	t_navcell;

# endif