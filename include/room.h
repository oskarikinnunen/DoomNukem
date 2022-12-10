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

typedef struct s_line2
{
	uint32_t	start_index;
	uint32_t	end_index;
	t_vector2	*start;
	t_vector2	*end;
}	t_line2;

typedef struct s_wall
{
	t_entity	*entity;
	uint16_t	saved_entityid;
	t_line		line;
	uint32_t	height;
	bool		disabled; //TODO: implement
	char		texname[256];
}	t_wall;

typedef struct s_floor_area
{
	uint32_t	wall_indices[64];
	uint32_t	wall_count;
}	t_floor_area;
typedef struct s_room //TODO: rewrite rooms/walls so rooms have t_vector2 edges and walls have pointers to those
{
	char			name[32];
	t_wall			*walls;
	t_meshtri		*floors;
	t_meshtri		*ceilings;
	t_floor_area	*floor_areas;
	uint32_t		wallcount;
	uint32_t		floorcount;
	uint32_t		ceilingcount;
	uint32_t		floor_areacount;
}	t_room;

typedef struct s_room2 //TODO: rewrite rooms/walls so rooms have t_vector2 edges and walls have pointers to those
{
	char			name[32];
	//t_vector2		*edges
	t_wall			*walls;
	t_meshtri		*floors;
	t_meshtri		*ceilings;
	t_floor_area	*floor_areas;
	uint32_t		wallcount;
	uint32_t		floorcount;
	uint32_t		ceilingcount;
	uint32_t		floor_areacount;
}	t_room2;

struct s_world;

void			add_room_to_world(struct s_world *world, t_room *room);
void			applywallmesh(t_wall *wall);
void			applytrimesh(t_meshtri tri, t_object *obj);
void			init_room_meshes(t_room *room, t_sdlcontext *sdl, struct s_world *world);
void			walls_init(struct s_world *world, struct s_sdlcontext *sdl);

#endif