#ifndef COLLISION_H
# define COLLISION_H

#include "shapes.h"

typedef struct s_collision
{
	t_vector2	normal;
	float		depth;
}	t_collision;

/* PHYSICS.C */
bool	pointrectanglecollision(t_point p, t_rectangle rect);
bool	pointcirclecollision(t_vector2 p, t_vector2 cp, float r);

#endif