#ifndef NPCTOOL_H
# define NPCTOOL_H

#include "room.h"
#include "colliders.h"

typedef enum e_npctoolmode
{
	ntm_select,
	ntm_addbox,
	ntm_addvbox
}	t_npctoolmode;

typedef struct s_npctooldata
{
	t_entity			*ent;
	int32_t				tri_index;
	t_vector3			*vertex;
	t_objectmetadata	metadata;
	int					i_col;
	bool				vertex_set;
	t_limbcollider		col;
	t_npctoolmode		ntm;
	t_autogui			main_gui;
	t_autogui			object_gui;
	t_autogui			collider_gui;
}	t_npctooldata;

# endif