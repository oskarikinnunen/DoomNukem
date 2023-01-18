#ifndef PLAYER_H
# define PLAYER_H

#include "entity.h"

typedef struct s_player
{
	t_transform		transform;
	bool			noclip;
	bool			locked;
	bool			isgrounded;
	t_vector3		velocity;
	t_vector3		speed; //read-only player speed used for animations etc
	t_vector3		lookdir;
	t_anim			jump;
	uint32_t		lastjumptime;
	float			height;
	struct s_gun	*gun;
	float			collision_radius;
	
}	t_player;

#endif

