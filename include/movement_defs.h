#ifndef MOVEMENT_DEFS_H
# define MOVEMENT_DEFS_H
# define PLAYERRADIUS 16

//Global gravity for all capsulephysics entities
#define GRAVITY -0.8f

# define EDITOR_FLYSPEED 0.175f
# define EDITOR_FLYFASTSPEED 0.3f

# define PLAYER_WALKSPEED 0.1f
# define PLAYER_RUNSPEED 0.2f
# define PLAYER_ACCELERATION 0.1f
# define PLAYER_DECELERATION 0.01f
# define PLAYER_HEIGHT 50.0f
# define PLAYER_CROUCHHEIGHT 25.0f
//How big of an step can the player step over
# define COL_STEP 15.0f
//The speed player goes to crouch
# define PLAYER_CROUCHSPEED 0.1f
# define JUMP_DELAY 500


//# define MAXMOVEMENTSPEED 0.08f
# define ROTATESPEED 0.002f
# define MOUSESPEED 0.002f

/*
	set player input struct in eventloop

	playermove
		if !locked
		{
			turn by input->turn
			normalmovement
			{
				normalizedinputvector
				add ^ to velocity
				clamp velocity magnitude
				checkcollision(player,world)
					use 2d velocity to check wallcollision
			}
		}
			
	update headpos
	



*/


#endif