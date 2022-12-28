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

typedef struct s_floor_area
{
	t_vector2	*edges[32];
	uint32_t	edge_indices[64];
	uint32_t	edgecount;
	uint32_t	unique_wallcount;
	int32_t		height;
}	t_floor_area;

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
	t_line		line;
	t_edgeline	edgeline;
	uint32_t	height;
	bool		disabled;
	bool		connection;
	//t_room		*connected;
	char		texname[256];
}	t_wall;

typedef struct s_room //TODO: rewrite rooms/walls so rooms have t_vector2 edges and walls have pointers to those
{
	char			name[32];
	t_vector2		*edges;
	t_wall			*walls;
	t_meshtri		*floors;
	t_meshtri		*ceilings;
	t_floor_area	*floor_areas; //
	char			floortex[64];
	uint32_t		height;
	uint32_t		ceiling_height;
	uint32_t		edgecount;
	uint32_t		wallcount;
	uint32_t		floorcount;
	uint32_t		ceilingcount;
	uint32_t		floor_areacount;
}	t_room;

void	assign_default_floor_area(t_room *room);

struct s_world;

bool			edge_exists(t_vector2 edge, t_room	*room);
void			makefloor_room(struct s_world *world, t_room *room);
t_room			*add_room_to_world(struct s_world *world, t_room *room);
void			applywallmesh(t_wall *wall, t_room *room, struct s_world *world);
void			applytrimesh(t_meshtri tri, t_object *obj);
void			init_room_meshes(t_room *room, t_sdlcontext *sdl, struct s_world *world);
void			walls_init(struct s_world *world, struct s_sdlcontext *sdl);

#endif