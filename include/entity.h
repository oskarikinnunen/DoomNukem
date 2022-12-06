#ifndef ENTITY_H
# define ENTITY_H

#include "vectors.h"
#include "animation.h"
#include "objects.h"

typedef struct s_bound
{
	float	min;
	float	max;
}	t_bound;

typedef struct s_transform
{
	t_vector3			position;
	t_vector3			rotation;
	t_vector3			scale;
	struct s_transform	*parent;
}	t_transform;

typedef enum s_entitystatus
{
	es_free,
	es_inactive,
	es_active
}	t_entitystatus;

/*typedef struct s_entityroot
{
	t_entity	*entity;
	uint16_t	entity_id;
	uint16_t	root_id;
}	t_entityroot;*/

typedef struct s_entity
{
	t_transform		transform;
	bool			uneditable;
	uint32_t		object_index;
	char			object_name[64];
	t_entitystatus	status;
	uint16_t		id;
	t_bound			z_bound;
	t_anim			animation;
	t_object		*obj;
	//uint16_t		root_id;
}	t_entity;

typedef struct s_gun
{
	t_entity	entity;
	char		object_name[128];
	char		preset_name[32];
	t_anim		shoot_anim;
	t_anim		view_anim;
	t_vector3	holsterpos;
	t_vector3	aimpos;
	uint32_t	lastshottime;
	bool		readytoshoot;
	bool		disabled;
	t_vector2	viewrecoil;
	t_vector2	recoiljump;
	t_vector2	recoilangle;
	float		anglerecovery;
	bool		fullauto;
	uint32_t	firedelay;
}	t_gun;

typedef struct s_entitycache
{
	t_entity	*entities;
	uint32_t	existing_entitycount;
	uint32_t	alloc_count;
}	t_entitycache;

#endif