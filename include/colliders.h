#ifndef COLLIDERS_H
# define COLLIDERS_H

typedef struct s_limbcollider
{
	t_vector3	*start_vertex;
	t_vector3	*end_vertex;
}	t_limbcollider;

typedef struct s_boxcollider
{
	t_vector3	*vertex;
	t_vector3	offset;
	t_vector3	size;
}	t_boxcollider;

typedef struct s_humancolliders
{
	t_limbcollider	head;
	t_limbcollider	body;
	t_limbcollider	arms[2];
	t_limbcollider	legs[2];
}	t_humancolliders;

typedef struct s_colliders
{
	t_limbcollider	limbcolliders[4];
	uint32_t		limbcol_count;
	t_boxcollider	boxcolliders[8];
	uint32_t		boxcol_count;
}	t_colliders;

typedef struct s_objectmetadata
{
	char		objname[64];
	t_colliders	col;
}	t_objectmetadata;


#endif