#ifndef ENTITY_H
# define ENTITY_H

#include "vectors.h"
#include "animation.h"
#include "objects.h"
#include "occlusion.h"
#include "render.h"

typedef struct s_bound
{
	float	min;
	float	max;
}	t_bound;

typedef struct s_transform
{
	t_vector3			location;
	t_vector3			rotation;
	t_vector3			scale;
	struct s_transform	*parent;
}	t_transform;

typedef struct s_entity
{
	t_transform		transform;
	uint32_t		object_index;
	char			object_name[64];
	t_bound			z_bound;
	t_anim			animation;
	t_object		*obj;
	t_occlusion		occlusion;
	int32_t			id;
}	t_entity;

void update_peripheral_culling(t_sdlcontext sdl, t_render *render, t_entity *entity);
void update_occlusion_culling(t_sdlcontext sdl, t_render *render, t_entity *entity);

bool is_entity_culled(t_sdlcontext sdl, t_render *render, t_entity *entity);
bool is_entity_frustrum_culled(t_sdlcontext sdl, t_render *render, t_entity *entity);
bool is_entity_peripheral_culled(t_sdlcontext sdl, t_render *render, t_entity *entity);
bool is_entity_occlusion_culled(t_sdlcontext sdl, t_render *render, t_entity *entity);
void	render_entity(t_sdlcontext sdl, t_render *render, t_entity *entity);

#endif
