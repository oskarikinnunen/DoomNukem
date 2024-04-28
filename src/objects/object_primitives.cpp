/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_primitives.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 12:50:46 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 17:59:58 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

static void	object_tri_set_faces(t_object *tri)
{
	tri->faces = prot_memalloc(sizeof(t_face) * 1);
	tri->face_count = 1;
	tri->faces[0].v_indices[0] = 1;
	tri->faces[0].v_indices[1] = 2;
	tri->faces[0].v_indices[2] = 3;
	tri->faces[0].uv_indices[0] = 1;
	tri->faces[0].uv_indices[1] = 2;
	tri->faces[0].uv_indices[2] = 3;
}

t_object	*object_tri(t_sdlcontext *sdl)
{
	t_object	*tri;

	tri = prot_memalloc(sizeof(t_object));
	ft_strncpy_term(tri->name, "meshtri", 250);
	tri->vertices = prot_memalloc(sizeof(t_vector3) * 3);
	tri->vertice_count = 3;
	tri->vertices[0] = (t_vector3){0.0f, 0.0f, 0.0f};
	tri->vertices[1] = (t_vector3){10.0f, 0.0f, 0.0f};
	tri->vertices[2] = (t_vector3){0.0f, 0.0f, 10.0f};
	object_tri_set_faces(tri);
	tri->materials = default_mat();
	tri->faces[0].material = &tri->materials[0];
	tri->materials->img = get_image_by_name(*sdl, tri->materials->texturename);
	tri->material_count = 1;
	tri->uvs = prot_memalloc(sizeof(t_vector2) * 3);
	tri->uv_count = 3;
	tri->uvs[0] = (t_vector2){0.0f, 0.0f};
	tri->uvs[1] = (t_vector2){1.0f, 0.0f};
	tri->uvs[2] = (t_vector2){0.0f, 1.0f};
	return (tri);
}

static void	plane_make_faces(t_object *plane)
{
	plane->vertices[0] = (t_vector3){0.0f, 0.0f, 0.0f};
	plane->vertices[1] = (t_vector3){10.0f, 0.0f, 0.0f};
	plane->vertices[2] = (t_vector3){0.0f, 0.0f, 10.0f};
	plane->vertices[3] = (t_vector3){10.0f, 0.0f, 10.0f};
	plane->faces[0].v_indices[0] = 1;
	plane->faces[0].v_indices[1] = 2;
	plane->faces[0].v_indices[2] = 3;
	plane->faces[0].uv_indices[0] = 1;
	plane->faces[0].uv_indices[1] = 2;
	plane->faces[0].uv_indices[2] = 3;
	plane->faces[1].v_indices[0] = 2;
	plane->faces[1].v_indices[1] = 3;
	plane->faces[1].v_indices[2] = 4;
	plane->faces[1].uv_indices[0] = 2;
	plane->faces[1].uv_indices[1] = 3;
	plane->faces[1].uv_indices[2] = 4;
}

static void	plane_make_uvs_and_mats(t_object *plane, t_sdlcontext *sdl)
{
	plane->materials = default_mat();
	plane->material_count = 1;
	plane->faces[0].material = &plane->materials[0];
	plane->faces[1].material = &plane->materials[0];
	plane->materials->img = \
			get_image_by_name(*sdl, plane->materials->texturename);
	plane->uvs = prot_memalloc(sizeof(t_vector2) * 4);
	plane->uv_count = 2;
	plane->uvs[0] = (t_vector2){0.0f, 0.0f};
	plane->uvs[1] = (t_vector2){0.0f, 1.0f};
	plane->uvs[2] = (t_vector2){-1.0f, 0.0f};
	plane->uvs[3] = (t_vector2){-1.0f, 1.0f};
}

t_object	*object_plane(t_sdlcontext *sdl)
{
	t_object	*plane;

	plane = prot_memalloc(sizeof(t_object));
	plane->vertices = prot_memalloc(sizeof(t_vector3) * 4);
	plane->vertice_count = 4;
	plane->faces = prot_memalloc(sizeof(t_face) * 2);
	plane->face_count = 2;
	ft_strncpy_term(plane->name, "plane", 250);
	plane_make_faces(plane);
	plane_make_uvs_and_mats(plane, sdl);
	return (plane);
}
