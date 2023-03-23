#ifndef RAYCAST_H
# define RAYCAST_H

# include "vectors.h"
# include "collision.h"

struct s_entity;
struct s_world;

typedef struct s_raycastinfo
{
	struct s_entity	*hit_entity;
	t_vector3		hit_pos;
	float			distance;
}	t_raycastinfo;

bool	raycast(t_ray r, t_raycastinfo *info, struct s_world *world);
bool	raycast_plane(t_ray r, t_raycastinfo *info, float plane_z);
bool	raycast_entity(t_ray r, t_raycastinfo *info, struct s_entity *entity);
bool	raycast_tri(t_ray r, t_vector3_tri tri, float *dist);

#endif