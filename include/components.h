#ifndef COMPONENTS_H
# define COMPONENTS_H

# ifndef INTTYPES_H
	#include <inttypes.h>
# endif

#include "navigation.h"

typedef enum e_componenttype
{
	pft_none,
	pft_interactable,
	pft_light,
	pft_npc,
	pft_audiosource,
	pft_eventtrigger
}	t_component_type;

struct s_autogui;
struct s_world;


//Func allocate: called when ed
typedef struct s_component
{
	t_component_type	type;
	size_t				data_size;
	void				(*func_allocate)(struct s_entity *,struct s_world	*);
	void				(*func_loadassets)(struct s_entity *,struct s_world	*); //Only used if component needs to hold assets/pointer data, in which case you can implement the loading behaviour here
	void				(*func_gui_edit)(struct s_component *, struct s_autogui *gui);
	void				(*func_update)(struct s_entity *,struct s_world	*);
	void				(*func_ui_update)(struct s_entity *,struct s_world	*);
	void				*data;
}	t_component;

typedef struct s_componentdefinition
{
	t_component_type	type;
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

//ft_movetowards(world->navmesh[npc._path_index[index]].mid_point)
typedef struct s_npc
{
	t_path		path;
	float		movementspeed;
}	t_npc;

/* AUDIOSOURCE FUNCTIONS */
bool	source_is_playing(t_audiosource *source);
void	assign_component_audiosource(t_component *component);
/* AUDIOSOURCE INTERNAL FUNCTIONS */
void	_audiosource_start(t_sdlcontext *sdl, t_audiosource	*source, t_vector3 *pos);

/*NPC*/
void	assign_component_npc(t_component *component);

/* GENERIC COMPONENT FUNCTIONS*/
t_componentdefinition	*get_component_definitions();
void	component_empty_function_pointers(t_component	*component);

#endif