/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:41:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/30 16:32:26 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

#include <inttypes.h>
#include "vectors.h"
//#include "colliders.h"

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
	t_vector3			*vertices; //null terminated 'array' of vertices
	t_face				*faces; //null terminated 'array' of faces
	t_vector2			*uvs;
	uint32_t			material_count;
	uint32_t			vertice_count;
	uint32_t			uv_count;
	uint32_t			face_count;
	t_objectanim		o_anim;
	struct s_colliders	*col;
}	t_object;

struct s_world;
struct s_sdlcontext;

t_object		objparse(char *filename);
struct s_list	*get_uv_list(int fd);
struct s_list	*get_vertex_list(int fd);
t_list			*get_face_list(int fd, t_list *materials);
void			objects_init(struct s_sdlcontext *sdl);
void			parseanim(t_object *object, char *animname);
void			parse_animframe(int fd, t_objectanimframe *frame, t_object *object);
t_object		*get_object_by_name(struct s_sdlcontext sdl, char *name);
t_object		*object_plane(struct s_sdlcontext *sdl);
t_object		*object_tri(struct s_sdlcontext *sdl);

#endif