#ifndef ENTITY_H
# define ENTITY_H

#include "vectors.h"
#include "animation.h"
#include "objects.h"
#include "occlusion.h"
#include "render.h"
#include "lighting.h"

typedef struct s_bound
{
	float	min;
	float	max;
}	t_bound;

typedef struct s_transform
{
	t_vector3			position;
	t_vector3			rotation;
	t_vector3			scale;
	struct s_transform	*parent;
}	t_transform;

typedef enum s_entitystatus
{
	es_free,
	es_inactive,
	es_active
}	t_entitystatus;

/*typedef struct s_entityroot
{
	t_entity	*entity;
	uint16_t	entity_id;
	uint16_t	root_id;
}	t_entityroot;*/

typedef struct s_entity
{
	t_transform		transform;
	bool			uneditable;
	bool			hidden;
	uint32_t		object_index;
	char			object_name[64];
	t_entitystatus	status;
	uint16_t		id;
	t_bound			z_bound;
	t_anim			animation;
	t_object		*obj;
	t_occlusion		occlusion;
	t_vector2		max;
	t_lightmap		*lightmap;
	t_map			*map;
	//uint16_t		root_id;
}	t_entity;

/*typedef enum s_audiosource
{
	t_entity	*entity;
	uint16_t	entity_id;
}	t_audiosource;*/

typedef enum e_prefabtype
{
	//pft_pickup,
	pft_interactable,
	pft_light,
	pft_npc,
	pft_audiosource,
	pft_eventtrigger
}	t_prefabtype;

/*typedef struct s_prefab
{
	t_object		object_name[64];
	t_prefabtype	prefabtype;
	uint16_t		prefab_id;
}	t_prefab;*/

typedef struct s_gun
{
	t_entity	entity;
	char		object_name[128];
	char		preset_name[32];
	t_anim		shoot_anim;
	t_anim		view_anim;
	t_vector3	holsterpos;
	t_vector3	aimpos;
	uint32_t	lastshottime;
	bool		readytoshoot;
	bool		disabled;
	t_vector2	viewrecoil;
	t_vector2	recoiljump;
	t_vector2	recoilangle;
	float		anglerecovery;
	bool		fullauto;
	uint32_t	firedelay;
}	t_gun;

typedef struct s_entitycache
{
	t_entity	*entities;
	uint32_t	existing_entitycount;
	uint32_t	alloc_count;
}	t_entitycache;

/* OCCLUSION FOLDER */
void update_peripheral_culling(t_sdlcontext sdl, t_render *render, t_entity *entity);
void update_occlusion_culling(t_sdlcontext sdl, t_render *render, t_entity *entity);

bool is_entity_culled(struct s_world *world, t_render *render, t_entity *entity);
bool is_entity_frustrum_culled(t_sdlcontext sdl, t_render *render, t_entity *entity);
bool is_entity_peripheral_culled(t_sdlcontext sdl, t_render *render, t_entity *entity);
bool is_entity_occlusion_culled(struct s_world *world, t_render *render, t_entity *cull);

void	calculate_triangles(t_sdlcontext sdl, t_render *render, t_entity *entity);
int		calculate_tris_from_square(t_square s, t_entity *ent, t_render *render);

/* RENDERING */
void	render_entity(t_sdlcontext *sdl, t_render *render, t_entity *entity);
void	draw_wireframe(t_sdlcontext sdl, t_render *render, t_entity *e, uint32_t clr);
void	draw_edges(t_sdlcontext sdl, t_render *render, t_entity *e, uint32_t clr);
void	render_worldspace(t_render *render, t_entity *entity);
void	render_quaternions(t_sdlcontext *sdl, t_render *render, t_entity *entity);

void create_map_for_entity(t_entity *entity, struct s_world *world);
void create_dynamic_map_for_entity(t_entity *entity, struct s_world *world);
void create_lightmap_for_entity(t_entity *entity, struct s_world *world);

#endif
