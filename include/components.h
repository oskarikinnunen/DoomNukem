/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   components.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:27:40 by raho              #+#    #+#             */
/*   Updated: 2023/03/28 09:45:22 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPONENTS_H
# define COMPONENTS_H

# ifndef INTTYPES_H
#  include <inttypes.h>
# endif

# include "collision.h"

struct	s_autogui;
struct	s_entity;
struct	s_world;

typedef enum e_componenttype
{
	COMP_NONE,
	COMP_INTERACTABLE,
	COMP_NPC,
	COMP_AUDIOSOURCE,
	COMP_MECHASUIT,
	COMP_WATERCOLLIDER,
	COMP_LIGHT,
	COMP_HEALTHPACK,
	COMP_NPC_CIVILIAN,
	COMP_PLAYERSPAWN,
	COMP_GUN_PICKUP,
	COMP_AUTO_DOOR,
	COMP_STORYEVENT,
	COMP_BOUTTON
}	t_component_type;

//Func allocate: called when ed
//*func_load_assets is only used if component needs to hold assets/pointer data,
//in which case you can implement the loading behaviour here
typedef struct s_component
{
	t_component_type	type;
	uint32_t			data_preset;
	size_t				data_size;
	void				(*func_take_damage)(\
							struct s_entity *, struct s_world *);
	void				(*func_allocate)(\
							struct s_entity *, struct s_world *);
	void				(*func_load_assets)(\
							struct s_entity *, struct s_world *);
	void				(*func_gui_edit)(\
							struct s_entity *, struct s_autogui *gui, \
							struct s_world *world);
	void				(*func_update)(\
							struct s_entity *, struct s_world *);
	void				(*func_ui_update)(\
							struct s_entity *, struct s_world *);
	void				*data;
}	t_component;

typedef struct s_component_definition
{
	t_component_type	type;
	char				name[32];
	void				(*func_assign_component)(t_component *component);
}	t_component_definition;

/* COMPONENT_AUDIOSOURCE */
typedef struct s_audiosource
{
	t_audio_sample	sample;
	FMOD_CHANNEL	*channel;
	float			range;
	float			volume;
	float			reverb;
	bool			play_always;
	bool			queue_play;
	bool			queue_stop;
	int				random_delay_min;
	int				random_delay_max;
	float			_real_range;
	uint32_t		_next_start;
}	t_audiosource;

typedef struct s_interactable
{
	t_character_physics	phys;
	t_anim				wiggle;
	t_vector3			velocity;
	t_vector2			pushv;
	float				radius;
	bool				is_grounded;
	bool				interacted;
}	t_interactable;

typedef struct s_healthpack
{
	float	test_variable;
	bool	is_actually_ammo;
}	t_healthpack;

typedef struct s_npc_civilian
{
	float	test_variable;
}	t_npc_civilian;

typedef struct s_playerspawn
{
	float	test_variable;
}	t_playerspawn;

typedef struct s_gun_pickup
{
	float	original_z;
}	t_gun_pickup;

typedef struct s_auto_door
{
	float	original_z;
}	t_auto_door;

typedef struct s_storyevent
{
	bool		triggered;
	uint32_t	trigger_time;
	char		text[128];
}	t_storyevent;

typedef struct s_boutton
{
	bool		can_interact;
	uint32_t	last_interact_time;
}	t_boutton;

/*STRUCTDEF END, DONT REMOVE! THE CREATECOMPONENT SCRIPT DEPENDS ON THIS*/

/* AUDIOSOURCE FUNCTIONS */
bool					source_is_playing(t_audiosource *source);
void					assign_component_audiosource(t_component *component);
void					assign_component_interactable(t_component *component);
void					assign_component_light(t_component *component);
void					assign_component_healthpack(t_component *component);
void					assign_component_npc_civilian(t_component *component);
void					assign_component_player_spawn(t_component *component);
void					assign_component_gun_pickup(t_component *component);
void					assign_component_auto_door(t_component *component);
void					assign_component_story_event(t_component *component);
void					assign_component_boutton(t_component *component);

/* ASSIGNFUNC END, DONT REMOVE! THE CREATECOMPONENT SCRIPT DEPENDS ON THIS */
/* AUDIOSOURCE INTERNAL FUNCTIONS */
void					_audiosource_start(t_sdlcontext *sdl, \
									t_audiosource *source, t_vector3 *pos);
void					_audiosource_2d_start(t_sdlcontext *sdl, \
									t_audiosource	*source);

void					comp_audiosource_update(struct s_entity *entity, \
												struct s_world *world);

/*NPC*/
void					assign_component_npc(t_component *component);

/* GENERIC COMPONENT FUNCTIONS*/
t_component_definition	*get_component_definitions(void);
void					component_empty_function_pointers(\
							t_component *component);
/* BOUTTON EVENT FUNCTIONS */
void					comp_boutton_ui_update(struct s_entity *entity, \
												struct s_world *world);
void					comp_boutton_gui_edit(struct s_entity *entity, \
							struct s_autogui *gui, struct s_world *world);
void					comp_boutton_load_assets(struct s_entity *entity, \
												struct s_world *world);
void					comp_boutton_allocate(struct s_entity *entity, \
												struct s_world *world);

/* GUN EVENT FUNCTIONS */
void					comp_gun_pickup_ui_update(struct s_entity *entity, \
													struct s_world *world);
void					comp_gun_pickup_gui_edit(struct s_entity *entity, \
								struct s_autogui *gui, struct s_world *world);
void					comp_gun_pickup_load_assets(struct s_entity *entity, \
													struct s_world *world);
void					comp_gun_pickup_allocate(struct s_entity *entity, \
												struct s_world *world);

/* STORY EVENT FUNCTIONS */
void					comp_storyevent_ui_update(struct s_entity *entity, \
													struct s_world *world);
void					comp_storyevent_gui_edit(struct s_entity *entity, \
								struct s_autogui *gui, struct s_world *world);
void					comp_storyevent_load_assets(struct s_entity *entity, \
													struct s_world *world);
void					comp_storyevent_allocate(struct s_entity *entity, \
													struct s_world *world);

/* INTERACTABLE FUNCTIONS */
void					comp_interactable_update(struct s_entity *entity, \
												struct s_world *world);

#endif