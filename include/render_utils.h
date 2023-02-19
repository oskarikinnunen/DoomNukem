#ifndef RENDER_UTILS_H
# define RENDER_UTILS_H

# include "vectors.h"

typedef struct s_camera
{
	t_mat4x4			matview;
	t_mat4x4			matproj;
	t_vector3			position;
	t_vector3			lookdir;
}	t_camera;

#endif