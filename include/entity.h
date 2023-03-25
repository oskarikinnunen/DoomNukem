/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 22:59:13 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 17:51:08 by raho             ###   ########.fr       */
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

# define GUNPRESETCOUNT 2

typedef struct s_bound
{
	float	min;
	float	max;
}	t_bound;

typedef enum e_entitystatus
{
	es_free,
	es_inactive,
	es_active
}	t_entitystatus;

#include "gamestring.h"

typedef struct s_entity
{
	t_gamestring	object_name;
	t_transform		transform;
	t_component		component;
	uint16_t		id;
	bool			ignore_raycasts;
	bool			rigid;
	bool			hidden;
	bool			dont_save;
	t_entitystatus	status;
	t_bound			z_bound;
	t_anim			animation;
	t_object		*obj;
	t_occlusion		occlusion;
	t_world_triangle		*world_triangles;
	t_map			*map;
	t_transform		old_transform;
}	t_entity;

typedef struct s_prefab
{
	t_object			*object;
	t_transform			offset;
	char				object_name[64];
	char				prefab_name[64];
	t_componenttype	prefabtype;
	bool				hidden;
	void				*data;
}	t_prefab;

typedef struct s_gunstats
{
	char		object_name[128];
	char		audio_name[128];
	t_vector3	holsterpos;
	t_vector3	aimpos;
	t_vector2	viewrecoil;
	t_vector2	recoiljump;
	t_vector2	recoilangle;
	float		anglerecovery;
	float		fov_offset;
	float		ads_speed;
	uint32_t	damage;
	uint32_t	magazinecapacity;
	uint32_t	firedelay;
	uint32_t	reloadtime;
	uint32_t	ammomask;
	bool		fullauto;
}	t_gunstats;

typedef struct s_gun
{
	t_entity	*entity;
	t_gunstats	stats;
	t_anim		shoot_anim;
	t_anim		view_anim;
	t_anim		reload_anim;
	uint32_t	lastshottime;
	uint32_t	bullets;
	bool		readytoshoot;
	bool		player_owned;
	bool		disabled;
	float		aim_lerp;
}	t_gun;

typedef struct s_entitycache
{
	t_entity	*entities;
	uint32_t	existing_entitycount;
	uint32_t	alloc_count;
	t_entity	**sorted_entities;
}	t_entitycache;

void	entity_set_component_functions(t_entity *entity, struct s_world *world);

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
void	draw_worldspace_icon(t_vector3 pos, t_img *img, struct s_world *world);
void	highlight_entity(t_sdlcontext *sdl, t_entity *entity, uint32_t color);
void	render_entity(t_sdlcontext *sdl, t_render *render, t_entity *entity);
void	update_bounds_world_triangles(t_entity *entity);
bool	calculate_quat_buffer_for_entity(t_quaternion *q, t_entity *entity);
void	render_entitys_world_triangles(t_sdlcontext *sdl, t_render *render, t_entity *entity);
void	calculate_triangles_for_entity(t_entity *entity, struct s_world *world);

uint32_t	update_pixel_brightness(uint32_t light, uint32_t clr);
uint32_t	get_lighting_for_pixel(t_lighting *lighting, uint32_t light_amount, t_vector3 location);
uint32_t	get_light_amount(float delta, uint32_t clr, uint32_t light_amount);
t_vector3	texcoord_to_loc(t_vector3 ws[3], t_vector2 uv[3], t_vector2 p);
t_step		make_slope(float start, float end, float steps);
t_stepv3	make_uv_slopev3(int start, int end, int y, t_screen_triangle triangle);

/* LIGHTING */
void render_zbuffer(t_lighting *lighting, t_entity *entity);

# endif
