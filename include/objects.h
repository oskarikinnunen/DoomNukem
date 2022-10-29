/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:41:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/27 16:35:23 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

#include <inttypes.h>

typedef struct s_material
{
	char			name[256];
	uint32_t		kd;
	struct s_img	*img;
}	t_material;

typedef struct s_face //Indexer for constructing faces (triangles)
{
	uint32_t	indices[3];
}	t_face;

typedef struct s_object
{
	char				name[256];
	t_material			*materials; //null terminated 'array' of materials
	struct s_vector3	*vertices; //null terminated 'array' of vertices
	t_face				*faces; //null terminated 'array' of faces
	uint32_t			material_count;
	uint32_t			vertice_count;
	uint32_t			face_count;
}	t_object;

t_object		objparse(char *filename);
struct s_list	*get_vertex_list(int fd);
struct s_list	*get_face_list(int fd);

#endif