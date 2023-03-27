/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:51:59 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 22:44:24 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NPC_H
# define NPC_H

# include "navigation.h"
# include "collision.h"
# include "components.h"

struct	s_entity;
struct	s_world;

// nt_count should always be last as it is responsible for 
// keeping count on how many npc types there are.
typedef enum e_npc_type
{
	nt_default,
	nt_civilian,
	nt_enemy,
	nt_count
}	t_npc_type;

//State			Anim
//Idle			(idle)
//Chase			(walk_weapon)
//Cautious_idle	(aim / idle)
//Cautious_move	(walk_weapon)
//Aggro_shoot	(aim)
//Aggro_push	(walk_weapon)
//Dead			(random death)

//TODO: rename NPC_STATE_CAUTIOUS_MOVE
typedef enum e_npc_state
{
	NPC_STATE_IDLE,
	NPC_STATE_CAUTIOUS_MOVE,
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
	float				movement_speed;
	bool				npc_type_changed;
	bool				aggroed;
	bool				sees_player;
	uint32_t			next_action_time;
	uint32_t			next_shoot_time;
	float				orig_x_rotation;
	t_vector3			last_seen_player_pos;
	t_npc_state			state;
	t_npc_state			prev_state;
	t_npc_type			npc_type;
}	t_npc;

void	npc_get_hit(struct s_entity *entity, struct s_world *world);
void	npc_play_sound(struct s_entity *entity, \
						struct s_world *world, char *sound_name);
void	npc_shoot(struct s_entity *entity, t_npc *npc, \
					struct s_world *world);
void	npc_start_push_anim(struct s_entity *entity, t_npc *npc, \
								struct s_world *world);
void	npc_update_push_anim(struct s_entity *entity, t_npc *npc, \
								struct s_world *world);
void	npc_update_anim_state(struct s_entity *entity, t_npc *npc, \
								struct s_world *world);
void	npc_player_raycast(struct s_entity *entity, t_npc *npc, \
							struct s_world *world);
void	npc_switch_to_chase_state(struct s_entity *entity, t_npc *npc, \
									struct s_world *world);
void	npc_switch_to_cautious_move_state(struct s_entity *entity, \
										t_npc *npc, struct s_world *world);
bool	npc_sees_player_and_ready_to_aggro(t_npc *npc);
void	npc_switch_to_aggro_state(struct s_entity *entity, t_npc *npc, \
									struct s_world *world);
void	npc_start_pushanim_and_switch_to_push_state(struct s_entity *entity, \
										t_npc *npc, struct s_world *world);

#endif