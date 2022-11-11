
#include "doomnukem.h"
#include "objects.h"

static t_material	*planemat()
{
	t_material	*mat;

	mat = ft_memalloc(sizeof(t_material));
	if (mat == NULL)
		error_log(EC_MALLOC);
	ft_strcpy(mat->texturename, "gwall.png");
	mat->kd = INT_MAX;
	return (mat);
}



t_object	*object_plane()
{
	t_object	*plane;

	plane = ft_memalloc(sizeof(t_object));
	plane->vertices = ft_memalloc(sizeof(t_vector3) * 4);
	plane->vertice_count = 4;
	plane->faces = ft_memalloc(sizeof(t_face) * 2);
	plane->face_count = 2;
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
	plane->uvs = ft_memalloc(sizeof(t_vector2) * 4);
	plane->uv_count = 4;
	plane->uvs[0] = (t_vector2){0.0f, 0.0f};
	plane->uvs[1] = (t_vector2){1.0f, 0.0f};
	plane->uvs[2] = (t_vector2){0.0f, 1.0f};
	plane->uvs[3] = (t_vector2){1.0f, 1.0f};

	/*sizeof(t_wall);
	sizeof(t_object);*/
	return (plane);
}