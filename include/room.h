#ifndef ROOM_H
# define ROOM_H

#include "entity.h"
#include "render.h"
#include "gamestring.h"

typedef struct s_meshtri
{
	t_entity	*entity;
	t_face		face;
	t_vector3	v[3];
	t_vector2	uv[3];
}	t_meshtri;

typedef struct s_edgeline
{
	uint32_t	start_index;
	uint32_t	end_index;
	t_vector2	*start;
	t_vector2	*end;
}	t_edgeline;

typedef struct s_wall
{
	t_entity		*entity;
	t_edgeline		edgeline;
	uint32_t		z_offset;
	uint32_t		height;
	bool			ceilingwall;
	bool			connection;
	t_gamestring	s_walltex;
	t_vector2		uv_offset;
	float			uv_scale;
}	t_wall;

/*
	IDLE:
	get random path	thats near current pos (in random interval)
	if (shot at || seeplayer || hearplayer)
	{
		lastseenpos = playerpos //Or APPROX player pos, if not directly seen
		gotoalert
	}
*/


// ALERT:
/*	if !seeplayer && nopath
	{
		if (reached lastseenpos)
			move to random pos near current pos
		else
			path = path to lastseenpos
	}
	if seeplayer
		shoot
	if seeplayer && highhealth
		strafe around
	if seeplayer && lowhealth
		path = path away from player
	if curtime - lastseenplayertime	> threshold
		gotoidle
*/

/*
	split floors and ceilings to separate arrays
*/
typedef struct s_areastatus
{
	bool	is_floor;
	bool	is_exterior;
	bool	is_room;
}	t_areastatus;

typedef struct s_room
{
	char			name[32];
	t_vector2		edges[32];
	t_wall			walls[32];
	t_meshtri		floors[32];
	t_meshtri		ceilings[32];
	t_meshtri		ramps[32];
	t_gamestring	s_ceiltex;
	t_gamestring	s_floortex;
	bool			walls_enabled;
	bool			floor_enabled;
	bool			ceiling_enabled;
	bool			loop;
	uint32_t		height;
	uint32_t		ceiling_height;
	uint32_t		edgecount;
	uint32_t		wallcount;
	uint32_t		floorcount;
	uint32_t		ceilingcount;
	uint32_t		rampcount;
}	t_area;

struct s_world;


bool			room_edge_is_legal(t_vector2 *edge, t_area *room);
void			room_recalculate_joined_rooms(struct s_world *world, t_area *room);
void			room_init(t_area *room, struct s_world *world);
void			room_init_shallow(t_area *room, struct s_world *world);
bool			room_is_legal(struct s_world *world, t_area *room);
void			room_remove_entities(t_area *room, struct s_world *world);
bool			edge_exists(t_vector2 edge, t_area	*room);
void			room_make_floor(struct s_world *world, t_area *room);
t_area			*world_add_room(struct s_world *world, t_area *room);
void			world_remove_room(struct s_world *world, t_area *room);
void			room_setpreset_floor(t_area *room);
void			room_setpreset_exterior(t_area *room);
void			room_setpreset_room(t_area *room);
void			applywallmesh(t_wall *wall, t_area *room, struct s_world *world);
void			applytrimesh(t_meshtri tri, t_object *obj);
void			walls_init(struct s_world *world, struct s_sdlcontext *sdl);

#endif