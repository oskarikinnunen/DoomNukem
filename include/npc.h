#ifndef NPC_H
# define NPC_H

#include "navigation.h"
#include "physics.h"
#include "components.h"

typedef enum e_npc_type
{
	nt_default,
	nt_civilian,
	nt_enemy,
	nt_count // this should always be last as it is responsible for keeping count on how many npc types there are.
}	t_npc_type;

typedef enum e_npc_state
{
	NPC_STATE_IDLE,
	NPC_STATE_CAUTIOUS, //TODO: rename
	NPC_STATE_CHASE,
	NPC_STATE_AIM,
	NPC_STATE_SHOOT,
	NPC_STATE_DEAD
}	t_npc_state;

typedef struct s_npc
{
	t_characterphysics	phys;
	int32_t				health;
	bool				hit;
	t_path				path;
	t_anim				strafe_anim;
	t_audiosource		audiosource;
	float				movementspeed;
	bool				npc_type_changed;
	bool				aggroed;
	bool				seesplayer;
	uint32_t			next_action_time;
	uint32_t			next_shoot_time;
	float				orig_x_rotation;
	t_vector3			lastseen_playerpos;
	t_npc_state			state;
	t_npc_type			npc_type;
	void				(*func_take_damage)(struct s_entity *,struct s_world *); //on take damage can change civilians movement function to follow player and action to attack player
	void				(*func_update)(struct s_entity *,struct s_world *); //general stuff
	void				(*func_movement)(struct s_entity *,struct s_world *); //movement
	void				(*func_action)(struct s_entity *,struct s_world *); //actions
	void				(*func_anim)(struct s_entity *,struct s_world *); //animation updates
}	t_npc;

#endif