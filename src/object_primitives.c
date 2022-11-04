
#include "doomnukem.h"
#include "objects.h"

t_object	*object_plane()
{
	static t_object	plane;

	if (plane.vertices != 0)
		return (&plane);
	plane.vertices = ft_memalloc(sizeof(t_vector3) * 4);
	plane.vertice_count = 4;
	plane.faces = ft_memalloc(sizeof(t_face) * 2);
	plane.face_count = 2;
	if (plane.vertices == NULL || plane.faces == NULL)
		error_log(EC_MALLOC);
	plane.vertices[0] = (t_vector3){0.0f, 0.0f, 0.0f};
	plane.vertices[1] = (t_vector3){10.0f, 0.0f, 0.0f};
	plane.vertices[2] = (t_vector3){0.0f, 10.0f, 0.0f};
	plane.vertices[3] = (t_vector3){10.0f, 10.0f, 0.0f};
	 //first tri:
	plane.faces[0].v_indices[0] = 1;
	plane.faces[0].v_indices[1] = 2;
	plane.faces[0].v_indices[2] = 3;
	//second tri:
	plane.faces[1].v_indices[0] = 2; 
	plane.faces[1].v_indices[1] = 3;
	plane.faces[1].v_indices[2] = 4;
	return (&plane);
}