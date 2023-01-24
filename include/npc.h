#ifndef NPC_H
# define NPC_H

#include "navigation.h"

typedef enum e_npc_type
{
	nt_default,
	nt_civilian,
	nt_enemy,
	nt_count // this should always be last as it is responsible for keeping count on how many npc types there are.
}	t_npc_type;

typedef struct s_npc
{
	t_path		path;
	float		movementspeed;
	bool		npc_type_changed;
	t_npc_type	npc_type;
	void		(*func_take_damage)(struct s_entity *,struct s_world *); //on take damage can change civilians movement function to follow player and action to attack player
	void		(*func_update)(struct s_entity *,struct s_world *); //general stuff
	void		(*func_movement)(struct s_entity *,struct s_world *); //movement
	void		(*func_action)(struct s_entity *,struct s_world *); //actions
	void		(*func_anim)(struct s_entity *,struct s_world *); //animation updates
}	t_npc;

#endif