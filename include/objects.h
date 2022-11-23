/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:41:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/23 18:57:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

#include <inttypes.h>
#include "doomnukem.h"
#include "vectors.h"

typedef struct s_material
{
	char			texturename[256];
	char			name[256];
	uint32_t		kd;
	struct s_img	*img;
}	t_material;

typedef struct s_face //Indexer for constructing faces (triangles)
{
	uint32_t	v_indices[3];
	uint32_t	uv_indices[3];
	uint32_t	materialindex;
	t_material	*material;
}	t_face;

typedef	struct s_deltavertex
{
	uint32_t	v_index;
	t_vector3	delta;
}	t_deltavertex;

typedef struct s_objectanimframe
{
	t_deltavertex	*deltavertices;
	uint32_t		vertcount;
}	t_objectanimframe;

typedef struct s_objectanim
{
	t_objectanimframe	*frames;
	uint32_t			framecount;
	char				name[128];
} t_objectanim;


typedef struct s_object
{
	char				name[256];
	t_material			*materials; //null terminated 'array' of materials
	struct s_vector3	*vertices; //null terminated 'array' of vertices
	t_face				*faces; //null terminated 'array' of faces
	struct s_vector2	*uvs;
	uint32_t			material_count;
	uint32_t			vertice_count;
	uint32_t			uv_count;
	uint32_t			face_count;
	t_objectanim		o_anim;
}	t_object;

typedef struct s_meshtri
{
	t_entity	entity;
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
	t_entity	entity;
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

typedef struct s_gun
{
	t_entity	entity;
	t_anim		shoot_anim;
	t_vector3	holsterpos;
	t_vector3	aimpos;
	bool		readytoshoot;
	float		firerate;
	t_vector2	recoil;
	t_vector2	recoilspread;
	bool		semiauto;
}	t_gun;

struct s_world;
struct s_sdlcontext;

t_object		objparse(char *filename);
struct s_list	*get_uv_list(int fd);
struct s_list	*get_vertex_list(int fd);
t_list			*get_face_list(int fd, t_list *materials);
void			objects_init(struct s_sdlcontext *sdl);
void			parseanim(t_object *object, char *animname);
void			parse_animframe(int fd, t_objectanimframe *frame, t_object *object);
void			init_roomwalls(t_room *room, t_sdlcontext *sdl);
t_object		*get_object_by_name(struct s_sdlcontext sdl, char *name);
t_object		*object_plane(struct s_sdlcontext *sdl);
t_object		*object_tri(struct s_sdlcontext *sdl);
void			applywallmesh(t_wall *wall);
void			applytrimesh(t_meshtri tri, t_object *obj);
void			init_room_meshes(t_room *room, t_sdlcontext *sdl);
bool			object_lookedat(t_editor *ed, t_sdlcontext sdl, t_object *obj);
void			walls_init(struct s_world *world, struct s_sdlcontext *sdl);

#endif