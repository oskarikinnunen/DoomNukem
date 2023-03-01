#ifndef PLAYER_H
# define PLAYER_H

#include "entity.h"
#include "input.h"

#define MAXHEALTH	200

#define AM_SNIPER	0
#define AM_ASSAULT	1
#define AM_PISTOL	2
#define AM_GRENADE	3

typedef struct s_ammo_u8
{
	uint8_t	sniper;
	uint8_t	assault;
	uint8_t	pistol;
	uint8_t	grenade;
}	t_ammo_u8;
/*
sniper,
assault,
pistol,
grenade
*/

typedef struct s_player
{
	t_transform			transform;
	t_transform			head_transform;
	t_characterphysics	cp;
	t_input				input;
	bool				noclip;
	bool				locked;
	bool				isgrounded;
	bool				crouchjumped;
	float				speed; //read-only player speed used for animations etc
	t_vector3			lookdir;
	t_vector3			headposition;
	t_anim				jump;
	t_anim				landing;
	uint32_t			lastjumptime;
	uint32_t			health;
	uint8_t				gun_selection;
	union ammo_union
	{
		t_ammo_u8	ammo_u8;
		uint8_t		ammo_arr[4];
		uint32_t	mask;
	} ammo_union;
	uint8_t				gun_ammos[GUNPRESETCOUNT];
	t_gun				guns[GUNPRESETCOUNT];
	float				height;
	struct s_gun		*gun;
	float				collision_radius;
	float				fov;
}	t_player;

struct s_world;

void	playermovement_normal(t_player *player, struct s_world *world);
void	playermovement_noclip(t_player *player, struct s_world *world);

void	protagonist_play_audio(t_player *player, struct s_world *world, char *soundname);

#endif

