/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:44:59 by raho              #+#    #+#             */
/*   Updated: 2023/03/28 09:58:40 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

# include "entity.h"
# include "input.h"
# include "raycast.h"

# define DEATH_RETRY_DELAY 6000
# define MAXHEALTH 200
# define AM_SNIPER 0
# define AM_ASSAULT 1
# define AM_PISTOL 2
# define AM_GRENADE 3

struct	s_world;
struct	s_clock;

typedef struct s_ammo_u8
{
	uint8_t	sniper;
	uint8_t	assault;
	uint8_t	pistol;
	uint8_t	grenade;
}	t_ammo_u8;

typedef struct s_gui_notification
{
	uint32_t		starttime;
	t_game_string	str;
}	t_gui_notification;

//TODO: add raycast_info
//player speed is read-only - used for animations etc
typedef struct s_player
{
	t_transform			transform;
	t_transform			head_transform;
	t_character_physics	cp;
	t_input				input;
	bool				no_clip;
	bool				locked;
	bool				crouch_jumped;
	float				speed;
	t_vector3			look_dir;
	t_vector3			head_position;
	t_anim				jump;
	t_anim				landing;
	uint32_t			last_jump_time;
	uint32_t			health;
	uint32_t			last_hurt_time;
	t_vector3			last_hurt_pos;
	uint8_t				gun_selection;
	uint8_t				ammo_arr[4];
	t_gui_notification	gui_notif;
	t_raycast_info		raycast_info;
	t_gun				guns[GUNPRESETCOUNT];
	float				height;
	struct s_gun		*gun;
	float				collision_radius;
	float				fov;
}	t_player;

t_gun_stats	gun_machinegun(void);
t_gun_stats	gun_sniper(void);

void		draw_player_hud(struct s_world *world);
void		player_movement_normal(t_player *player, struct s_world *world);
void		player_movement_noclip(t_player *player, struct s_world *world);
void		player_gun_raycast(t_player *player, struct s_world *world);
void		protagonist_play_audio(t_player *player, struct s_world *world, \
									char *soundname);
void		gun_update_transform(t_player *player, t_gun *gun, \
									struct s_clock c);
void		gun_update_reload_status(t_player *player, t_gun *gun, \
										struct s_world *world);
void		gun_update_shoot_status(t_player *player, t_gun *gun, \
									struct s_world *world);
void		gun_update_reload_anim(t_player *player, t_gun *gun, \
									struct s_world *world);

#endif