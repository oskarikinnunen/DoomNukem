#ifndef ROOM_H
# define ROOM_H

#include "entity.h"
#include "render.h"

typedef struct s_meshtri
{
	t_entity	*entity;
	uint16_t	saved_entityid;
	t_face		face;
	t_vector3	v[3];
	t_vector2	uv[3];
	char		texname[256];
}	t_meshtri;

typedef struct s_line
{
	t_vector2	start;
	t_vector2	end;
}	t_line;

typedef struct s_edgeline
{
	/*int32_t		start_offset;
	int32_t		end_offset;*/
	uint32_t	start_index;
	uint32_t	end_index;
	t_vector2	*start;
	t_vector2	*end;
}	t_edgeline;

typedef struct s_wall
{
	t_entity	*entity;
	uint16_t	saved_entityid;
//	t_line		line;
	t_edgeline	edgeline;
	uint32_t	z_offset;
	uint32_t	height;
	bool		ceilingwall;
	bool		connection;
	//t_room		*connected;
	char		texname[256];
	t_vector2	uv_offset;
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

//[2][0][0][0][0][3]


typedef struct s_room //TODO: rewrite rooms/walls so rooms have t_vector2 edges and walls have pointers to those
{
	char			name[32];
	t_vector2		edges[32];
	//t_vector2		*edges;
	t_wall			*walls;
	t_meshtri		*floors;
	t_meshtri		*ceilings;
	char			floortex[64];
	bool			walls_enabled;
	bool			floor_enabled;
	bool			ceiling_enabled;
	bool			loop;
	//bool			open_area;
	uint32_t		height;
	uint32_t		ceiling_height;
	uint32_t		edgecount;
	uint32_t		wallcount;
	uint32_t		floorcount;
	uint32_t		ceilingcount;
}	t_room;

void	assign_default_floor_area(t_room *room);

struct s_world;

void			room_init(t_room *room, struct s_world *world);

void			room_remove_entities(t_room *room, struct s_world *world);

bool			edge_exists(t_vector2 edge, t_room	*room);
void			_room_triangulate_floors(struct s_world *world, t_room *room);
t_room			*world_add_room(struct s_world *world, t_room *room);
void			applywallmesh(t_wall *wall, t_room *room, struct s_world *world);
void			applytrimesh(t_meshtri tri, t_object *obj);
void			init_room_meshes(t_room *room, t_sdlcontext *sdl, struct s_world *world);
void			walls_init(struct s_world *world, struct s_sdlcontext *sdl);

#endif