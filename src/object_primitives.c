
#include "doomnukem.h"
#include "objects.h"

static t_material	*planemat()
{
	t_material	*mat;

	mat = ft_memalloc(sizeof(t_material));
	if (mat == NULL)
		error_log(EC_MALLOC);
	ft_strcpy(mat->texturename, "metal04.png");
	mat->kd = INT_MAX;
	return (mat);
}

static t_material	*trimat()
{
	t_material	*mat;

	mat = ft_memalloc(sizeof(t_material));
	if (mat == NULL)
		error_log(EC_MALLOC);
	ft_strcpy(mat->texturename, "metal03.png");
	mat->kd = INT_MAX;
	return (mat);
}

t_vector2 flipped_uv(t_vector2 og)
{
	t_vector2	uv;

	uv.x = -og.y;
	uv.y = og.x;
	return (uv);
}

t_object	*object_tri(t_sdlcontext *sdl)
{
	t_object	*tri;

	tri = ft_memalloc(sizeof(t_object));
	tri->vertices = ft_memalloc(sizeof(t_vector3) * 3);
	tri->vertice_count = 3;
	tri->faces = ft_memalloc(sizeof(t_face) * 1);
	tri->face_count = 1;
	if (tri->vertices == NULL || tri->faces == NULL)
		error_log(EC_MALLOC);
	tri->vertices[0] = (t_vector3){0.0f, 0.0f, 0.0f};
	tri->vertices[1] = (t_vector3){10.0f, 0.0f, 0.0f};
	tri->vertices[2] = (t_vector3){0.0f, 0.0f, 10.0f};
	 //first tri:
	tri->faces[0].v_indices[0] = 1;
	tri->faces[0].v_indices[1] = 2;
	tri->faces[0].v_indices[2] = 3;
	tri->faces[0].uv_indices[0] = 1;
	tri->faces[0].uv_indices[1] = 2;
	tri->faces[0].uv_indices[2] = 3;

	tri->materials = trimat();
	tri->faces[0].material = &tri->materials[0];
	tri->materials->img = get_image_by_name(*sdl, tri->materials->texturename);
	tri->material_count = 1;
	tri->uvs = ft_memalloc(sizeof(t_vector2) * 3);
	tri->uv_count = 3;
	tri->uvs[0] = (t_vector2){0.0f, 0.0f};
	tri->uvs[1] = (t_vector2){1.0f, 0.0f};
	tri->uvs[2] = (t_vector2){0.0f, 1.0f};
	return (tri);
}

t_object	*object_plane(t_sdlcontext *sdl)
{
	t_object	*plane;

	plane = ft_memalloc(sizeof(t_object));
	plane->vertices = ft_memalloc(sizeof(t_vector3) * 4);
	plane->vertice_count = 4;
	plane->faces = ft_memalloc(sizeof(t_face) * 2);
	plane->face_count = 2;
	strcpy(plane->name, "plane");
	if (plane->vertices == NULL || plane->faces == NULL)
		error_log(EC_MALLOC);
	plane->vertices[0] = (t_vector3){0.0f, 0.0f, 0.0f};
	plane->vertices[1] = (t_vector3){10.0f, 0.0f, 0.0f};
	plane->vertices[2] = (t_vector3){0.0f, 0.0f, 10.0f};
	plane->vertices[3] = (t_vector3){10.0f, 0.0f, 10.0f};
	//first tri:
	plane->faces[0].v_indices[0] = 1;
	plane->faces[0].v_indices[1] = 2;
	plane->faces[0].v_indices[2] = 3;
	plane->faces[0].uv_indices[0] = 1;
	plane->faces[0].uv_indices[1] = 2;
	plane->faces[0].uv_indices[2] = 3;
	//second tri:
	plane->faces[1].v_indices[0] = 2;
	plane->faces[1].v_indices[1] = 3;
	plane->faces[1].v_indices[2] = 4;
	plane->faces[1].uv_indices[0] = 2;
	plane->faces[1].uv_indices[1] = 3;
	plane->faces[1].uv_indices[2] = 4;

	plane->materials = planemat();
	plane->material_count = 1;
	plane->faces[0].material = &plane->materials[0];
	plane->faces[1].material = &plane->materials[0];
	plane->materials->img = get_image_by_name(*sdl, plane->materials->texturename);
	plane->uvs = ft_memalloc(sizeof(t_vector2) * 4);
	plane->uv_count = 2;
	plane->uvs[0] = (t_vector2){0.0f, 0.0f};
	plane->uvs[1] = (t_vector2){1.0f, 0.0f};
	plane->uvs[2] = (t_vector2){0.0f, 1.0f};
	plane->uvs[3] = (t_vector2){1.0f, 1.0f};
	plane->uvs[0] = flipped_uv(plane->uvs[0]);
	plane->uvs[1] = flipped_uv(plane->uvs[1]);
	plane->uvs[2] = flipped_uv(plane->uvs[2]);
	plane->uvs[3] = flipped_uv(plane->uvs[3]);

	/*sizeof(t_wall);
	sizeof(t_object);*/
	return (plane);
}