#ifndef COMPONENTS_H
# define COMPONENTS_H

# ifndef INTTYPES_H
	#include <inttypes.h>
# endif



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
}	t_componenttype;

struct s_autogui;
struct s_world;


//Func allocate: called when ed
typedef struct s_component
{
	t_componenttype		type;
	uint32_t			data_preset;
	size_t				data_size;
	void				(*func_take_damage)(struct s_entity *,struct s_world *);
	void				(*func_allocate)(struct s_entity *,struct s_world *);
	void				(*func_loadassets)(struct s_entity *,struct s_world	*); //Only used if component needs to hold assets/pointer data, in which case you can implement the loading behaviour here
	//void				(*func_gui_edit)(struct s_component *, struct s_autogui *gui); //TODO: s_entity *, world *, gui *
	void				(*func_gui_edit)(struct s_entity *, struct s_autogui *gui, struct s_world *world); //TODO: s_entity *, world *, gui *
	void				(*func_update)(struct s_entity *,struct s_world	*);
	void				(*func_ui_update)(struct s_entity *,struct s_world	*);
	void				*data;
}	t_component;

typedef struct s_componentdefinition
{
	t_componenttype		type;
	char				name[32];
	void				(*func_assign_component)(t_component *component);
}	t_componentdefinition;

/* COMPONENT_AUDIOSOURCE */
typedef struct s_audiosource
{
	t_audiosample	sample;
	FMOD_CHANNEL	*channel;
	float			range;
	float			volume;
	float			reverb;
	bool			play_always;
	bool			queue_play;
	bool			queue_stop;
	int				random_delay_min;
	int				random_delay_max;
	float			_realrange;
	uint32_t		_nextstart;
}	t_audiosource;

#include "npc.h"

typedef struct s_interactable
{
	t_characterphysics	phys;
	t_anim		wiggle;
	t_vector3	velocity;
	float		radius;
	bool		isgrounded;
	bool		interacted;
}	t_interactable;

typedef struct	s_mechasuit
{
	bool	in_use;
	float	p_dist;
}	t_mechasuit;
typedef struct	s_watercollider
{
	float	TESTVARIABLE;
}	t_watercollider;

typedef struct	s_healthpack
{
	float	TESTVARIABLE;
}	t_healthpack;
typedef struct	s_npc_civilian
{
	float	TESTVARIABLE;
}	t_npc_civilian;
typedef struct	s_playerspawn
{
	float	TESTVARIABLE;
}	t_playerspawn;
typedef struct	s_gun_pickup
{
	float	original_z;
}	t_gun_pickup;
typedef struct	s_auto_door
{
	float	original_z;
}	t_auto_door;
typedef struct	s_storyevent
{
	bool		triggered;
	uint32_t	trigger_time;
	char		text[128];
}	t_storyevent;
typedef struct	s_boutton
{
	float	dist;
}	t_boutton;
/*STRUCTDEF END, DONT REMOVE SINCE THE CREATECOMPONENT SCRIPT DEPENDS ON THIS*/

/* AUDIOSOURCE FUNCTIONS */
bool	source_is_playing(t_audiosource *source);
void	assign_component_audiosource(t_component *component);
void	assign_component_interactable(t_component *component);
void	assign_component_mechasuit(t_component *component);
void	assign_component_watercollider(t_component *component);
void	assign_component_light(t_component *component);
void	assign_component_healthpack(t_component *component);
void	assign_component_npc_civilian(t_component *component);
void	assign_component_playerspawn(t_component *component);
void	assign_component_gun_pickup(t_component *component);
void	assign_component_auto_door(t_component *component);
void	assign_component_storyevent(t_component *component);
void	assign_component_boutton(t_component *component);
/* ASSIGNFUNC END, DONT REMOVE SINCE THE CREATECOMPONENT SCRIPT DEPENDS ON THIS*/
/* AUDIOSOURCE INTERNAL FUNCTIONS */
void	_audiosource_start(t_sdlcontext *sdl, t_audiosource	*source, t_vector3 *pos);
void	_audiosource_2D_start(t_sdlcontext *sdl, t_audiosource	*source);

/*NPC*/
void	assign_component_npc(t_component *component);

/* GENERIC COMPONENT FUNCTIONS*/
t_componentdefinition	*get_component_definitions();
void	component_empty_function_pointers(t_component	*component);

#endif