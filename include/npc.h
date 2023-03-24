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

//State			Anim
//Idle			(idle)
//Chase			(walk_weapon)
//Cautious_idle	(aim / idle)
//Cautious_move	(walk_weapon)
//Aggro_shoot	(aim)
//Aggro_push	(walk_weapon)
//Dead			(random death)

typedef enum e_npc_state
{
	NPC_STATE_IDLE,
	NPC_STATE_CAUTIOUS_MOVE, //TODO: rename
	NPC_STATE_CHASE,
	NPC_STATE_AGGRO,
	NPC_STATE_AGGRO_PUSH,
	NPC_STATE_SHOOT,
	NPC_STATE_DEAD
}	t_npc_state;

typedef struct s_npc
{
	t_character_physics	phys;
	int32_t				health;
	bool				hit;
	t_path				path;
	t_anim				push_anim;
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
	t_npc_state			prev_state;
	t_npc_type			npc_type;
}	t_npc;

#endif