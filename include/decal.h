#ifndef DECAL_H
# define DECAL_H

# include "entity.h"

typedef struct s_decal
{
	t_img		*img;
	float		size;
	t_vector3	position;
	t_vector3	normal;
}	t_decal;

void decal(struct s_world *world, t_decal decal);

#endif