/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 22:59:13 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/16 23:45:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

# include "vectors.h"
# include "animation.h"
# include "objects.h"
# include "occlusion.h"
# include "render.h"
# include "lighting.h"
# include "components.h"

typedef struct s_bound
{
	float	min;
	float	max;
}	t_bound;

typedef enum s_entitystatus
{
	es_free,
	es_inactive,
	es_active
}	t_entitystatus;

# define SAVESTRUCT
typedef struct s_interactable
{
	float		radius;
	int			i32;
	uint64_t	testint2;
	t_anim		anim;
}	t_interactable;

# define SAVESTRUCT
typedef struct s_entity
{
	t_transform		transform;
	bool			ignore_raycasts;
	bool			rigid;
	bool			hidden;
	char			object_name[64];
	t_component		component;
	t_entitystatus	status;
	uint16_t		id;
	t_bound			z_bound;
	t_anim			animation;
	t_object		*obj;
	t_occlusion		occlusion;
	t_vector2		max;
	t_lightmap		*lightmap;
	t_map			*map;
}	t_entity;

typedef struct s_prefab
{
	t_object			*object;
	t_transform			offset;
	char				object_name[64];
	char				prefab_name[64];
	t_component_type	prefabtype;
	bool				hidden;
	void				*data;
}	t_prefab;

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
	t_entity	**sorted_entities;
}	t_entitycache;

void	entity_set_component(t_entity *entity, t_component_type type, struct s_world *world);


/* OCCLUSION*/
void	render_bitmask_row(int ax, int bx, float aw, float bw, int y, t_sdlcontext *sdl);
void	update_frustrum_culling(struct s_world *world, t_sdlcontext *sdl, t_render *render);

bool	is_entity_culled(t_sdlcontext *sdl, t_render *render, t_entity *entity);
bool	is_entity_frustrum_culled(t_sdlcontext *sdl, t_render *render, t_entity *entity);
bool	is_entity_occlusion_culled(t_sdlcontext *sdl, t_render *render, t_entity *entity);

void	calculate_triangles(t_sdlcontext sdl, t_render *render, t_entity *entity);
void	clear_occlusion_buffer(t_sdlcontext *sdl);

//Debug occl
void	draw_wireframe(t_sdlcontext sdl, t_square s, uint32_t clr);
void	bitmask_to_pixels(t_sdlcontext *sdl);

/* RENDERING */
void	draw_entity_icon(t_entity *entity, t_img *img, struct s_world *world);
void	highlight_entity(t_sdlcontext *sdl, t_entity *entity, uint32_t color);
void	render_entity(t_sdlcontext *sdl, t_render *render, t_entity *entity);
void	render_worldspace(t_render *render, t_entity *entity);
void	render_quaternions(t_sdlcontext *sdl, t_render *render, t_entity *entity);

void	create_map_for_entity(t_entity *entity, struct s_world *world);
void	create_dynamic_map_for_entity(t_entity *entity, struct s_world *world);
void	create_lightmap_for_entity(t_entity *entity, struct s_world *world);

# endif
