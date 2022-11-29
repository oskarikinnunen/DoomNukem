#ifndef NPCTOOL_H
# define NPCTOOL_H

#include "room.h"

typedef struct s_npctooldata
{
	t_entity			*ent;
	int32_t				tri_index;
	t_vector3			*vertex;
	t_capsulecollider	col;
	//t_humancolliders	col;
}	t_npctooldata;

# endif