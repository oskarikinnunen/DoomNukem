#ifndef COLLISION_H
# define COLLISION_H

#include "shapes.h"

/* PHYSICS.C */
bool	pointrectanglecollision(t_point p, t_rectangle rect);
bool	pointcirclecollision(t_vector2 p, t_vector2 cp, float r);

#endif