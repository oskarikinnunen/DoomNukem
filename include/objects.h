/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:41:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/08 05:37:50 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

#include <inttypes.h>
#include "vectors.h"

typedef struct s_material
{
	char			texturename[256];
	uint32_t		kd;
	struct s_img	*img;
}	t_material;

typedef struct s_face //Indexer for constructing faces (triangles)
{
	uint32_t	v_indices[3];
	uint32_t	uv_indices[3];
}	t_face;

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
}	t_object;

typedef struct s_line
{
	t_vector2	start;
	t_vector2	end;
}	t_line;

typedef struct s_wall
{
	t_object	object;
	t_line		line;
	uint32_t	height;
	uint8_t		roomindex;
	char		texname[256];
}	t_wall;

struct s_world;

t_object		objparse(char *filename);
struct s_list	*get_uv_list(int fd);
struct s_list	*get_vertex_list(int fd);
struct s_list	*get_face_list(int fd);
void			objects_init(struct s_sdlcontext *sdl);
t_object		*get_object_by_name(struct s_sdlcontext sdl, char *name);
t_object		*object_plane();
void			applywallmesh(t_wall *wall);
void			walls_init(struct s_world *world);

#endif