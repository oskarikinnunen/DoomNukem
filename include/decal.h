#ifndef DECAL_H
# define DECAL_H

# include "entity.h"

typedef struct s_decal
{
    t_img       *img;
    int         size;
    t_vector3   position;
    t_vector3   rotation;
    t_vector3   normal;
    t_entity    *entity;
}   t_decal;

void decal(struct s_world *world, t_decal decal);

#endif