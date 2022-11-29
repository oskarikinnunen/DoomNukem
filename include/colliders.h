#ifndef COLLIDERS_H
# define COLLIDERS_H

typedef struct s_capsulecollider
{
	t_entity	entity;
	t_vector3	*start_vertex;
	t_vector3	*end_vertex;
}	t_capsulecollider;

typedef struct s_humancolliders
{
	t_capsulecollider	head;
	t_capsulecollider	body;
	t_capsulecollider	arms[2];
	t_capsulecollider	legs[2];
}	t_humancolliders;

#endif