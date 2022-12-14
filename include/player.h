#ifndef PLAYER_H
# define PLAYER_H

#include "entity.h"

typedef struct s_player
{
	t_transform		transform;
	bool			locked;
	t_vector3		speed; //read-only player speed used for animations etc
	t_vector3		lookdir;
	t_anim			jump;
	float			height;
	struct s_gun	*gun;
}	t_player;

#endif

