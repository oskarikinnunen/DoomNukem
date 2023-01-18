#ifndef COLLISION_H
# define COLLISION_H

#include "shapes.h"

typedef struct s_collision
{
	t_vector2	normal;
	float		depth;
}	t_collision;

typedef struct s_vector3_tri
{
	t_vector3	a;
	t_vector3	b;
	t_vector3	c;
} t_vector3_tri;

/* PHYSICS.C */
bool	vector_is_in_triangle(t_vector3 vec, t_vector3_tri tri);
bool	pointrectanglecollision(t_point p, t_rectangle rect);
bool	pointcirclecollision(t_vector2 p, t_vector2 cp, float r);
bool	linepoint(t_vector2 start, t_vector2 end, t_vector2 point);
bool	linecircle(t_line line, t_vector2 circle, float radius, t_collision *collision);

#endif