#ifndef PLAYER_H
# define PLAYER_H

#include "entity.h"
#include "input.h"

typedef struct s_player
{
	t_transform		transform;
	t_input			input;
	bool			noclip;
	bool			locked;
	bool			isgrounded;
	bool			crouchjumped;
	bool			landingtrigger;
	t_vector3		velocity;
	float			speed; //read-only player speed used for animations etc
	t_vector3		lookdir;
	t_vector3		headposition;
	t_vector3		impactvelocity; //used for landing sound effect right now
	t_anim			jump;
	t_anim			landing;
	uint32_t		lastjumptime;
	float			height;
	struct s_gun	*gun;
	float			collision_radius;
	
}	t_player;

struct s_world;

void	playermovement_normal(t_player *player, struct s_world *world);
void	playermovement_noclip(t_player *player, struct s_world *world);

#endif

