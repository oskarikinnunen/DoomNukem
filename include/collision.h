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

typedef struct s_character_collision
{
	t_collision			collision;
	t_character_physics	cp;
	t_vector3			potential_pos;
	struct s_wall		*wall;
}	t_character_collision;

bool	vector_is_in_triangle(t_vector3 vec, t_vector3_tri tri);
bool	collision_point_rectangle(t_point p, t_rectangle rect);
bool	collision_point_circle(t_vector2 p, t_vector2 cp, float r);
bool	collision_line_point(t_vector2 start, t_vector2 end, t_vector2 point);

// Don't use this one for general use. Use line_circle_collision instead
// Saves the collision point to t_collision *
bool	collision_line_circle_save_collision(t_line line, t_vector2 circle, float radius, t_collision *collision);

// Use this for general use
bool	collision_line_circle(t_line line, t_vector2 circle, float radius);

#endif