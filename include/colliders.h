#ifndef COLLIDERS_H
# define COLLIDERS_H

typedef struct s_box
{
	t_vector3		v[8];
}	t_box;

typedef enum e_bound_type
{
	bt_ignore,
	bt_box,
	bt_plane
}	t_bound_type;

typedef struct s_bounds
{
	t_vector3		origin;
	t_box			box;
	t_bound_type	type;
	float			radius;
}	t_bounds;

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