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

typedef struct s_wall
{
	t_entity	*entity;
	uint16_t	saved_entityid;
	uint16_t	id;
	t_line		line;
	uint32_t	height;
	bool		disabled; //TODO: implement
	char		texname[256];
}	t_wall;

typedef struct s_room
{
	char		name[32];
	t_wall		*walls;
	t_meshtri	*floors;
	t_meshtri	*ceilings;
	uint32_t	wallcount;
	uint32_t	floorcount;
	uint32_t	ceilingcount;
}	t_room;


struct s_world;

void			add_room_to_world(struct s_world *world, t_room *room);
void			applywallmesh(t_wall *wall);
void			applytrimesh(t_meshtri tri, t_object *obj);
void			init_room_meshes(t_room *room, t_sdlcontext *sdl, struct s_world *world);
void			walls_init(struct s_world *world, struct s_sdlcontext *sdl);

#endif