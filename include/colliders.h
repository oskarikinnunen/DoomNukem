#ifndef COLLIDERS_H
# define COLLIDERS_H

typedef struct s_capsulecollider
{
	uint32_t	start_vertex;
	uint32_t	end_vertex;
}	t_capsulecollider;

typedef struct s_humancolliders
{
	t_capsulecollider	head;
	t_capsulecollider	body;
	t_capsulecollider	arms[2];
	t_capsulecollider	legs[2];
}	t_humancolliders;

#endif