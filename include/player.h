#ifndef PLAYER_H
# define PLAYER_H

#include "entity.h"
#include "input.h"
#include "raycast.h"

# define DEATH_RETRY_DELAY 6000

# define MAXHEALTH 200

# define AM_SNIPER 0
# define AM_ASSAULT 1
# define AM_PISTOL 2
# define AM_GRENADE 3

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

typedef struct s_gui_notification
{
	uint32_t		starttime;
	t_gamestring	str;
}	t_gui_notification;

//TODO: add raycast_info
typedef struct s_player
{
	t_transform			transform;
	t_transform			head_transform;
	t_character_physics	cp;
	t_input				input;
	bool				noclip;
	bool				locked;
	bool				crouchjumped;
	float				speed; //read-only player speed used for animations etc
	t_vector3			lookdir;
	t_vector3			headposition;
	t_anim				jump;
	t_anim				landing;
	uint32_t			lastjumptime;
	uint32_t			health;
	uint32_t			lasthurttime;
	t_vector3			lasthurtpos;
	uint8_t				gun_selection;
	uint8_t				ammo_arr[4];
	t_gui_notification	gui_notif;
	t_raycastinfo		raycastinfo;
	uint8_t				gun_ammos[GUNPRESETCOUNT];
	t_gun				guns[GUNPRESETCOUNT];
	float				height;
	struct s_gun		*gun;
	float				collision_radius;
	float				fov;
}	t_player;

struct s_world;

t_gunstats	gun_machinegun(void);
t_gunstats	gun_sniper(void);

void	draw_player_hud(struct s_world *world);
void	playermovement_normal(t_player *player, struct s_world *world);
void	playermovement_noclip(t_player *player, struct s_world *world);
void	protagonist_play_audio(t_player *player, struct s_world *world, char *soundname);

#endif

