/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:25:15 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 13:00:48 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLISION_H
# define COLLISION_H

# include "shapes.h"
# include "entity.h"
# include "render.h"

struct	s_render;
struct	s_sdlcontext;
struct	s_entity;
struct	s_triangle;
struct	s_line;
struct	s_world;
struct	s_bound;
struct	s_meshtri;

typedef struct s_collision
{
	t_vector2	normal;
	float		depth;
}	t_collision;

typedef struct s_vector3_tri
{
	t_vector3	a;
	t_vector3	b;
	t_vector3	c;
}	t_vector3_tri;

typedef struct s_character_physics
{
	float		height;
	float		radius;
	float		*gravity_override;
	t_vector3	*position;
	t_vector3	velocity;
	float		max_velocity;
	bool		is_grounded;
	bool		landing_trigger;
	bool		ceiling_trigger;
	t_vector3	impact_velocity;
}	t_character_physics;

typedef struct s_character_collision
{
	t_collision			collision;
	t_character_physics	cp;
	t_vector3			potential_pos;
	struct s_wall		*wall;
}	t_character_collision;

typedef struct s_line_line_intersect
{
	float	x_diff1;
	float	x_diff2;
	float	y_diff1;
	float	y_diff2;
	float	fa;
	float	fb;
	float	x;
	float	y;
}	t_line_line_intersect;

bool			vector_is_in_triangle(t_vector3 vec, t_vector3_tri tri);
bool			collision_point_rectangle(t_point p, t_rectangle rect);
bool			collision_point_triangle(t_point point, t_world_triangle tri);
bool			collision_point_circle(t_vector2 p, t_vector2 cp, float r);
bool			collision_line_point(t_vector2 start, t_vector2 end, \
										t_vector2 point);
bool			collision_line_line_intersect(t_line line1, t_line line2);

bool			check_character_collision(struct s_world *world, \
		t_character_physics cp, t_vector3 potential_pos, t_vector3 *new_pos);

void			capsule_damp(t_character_physics *phys, struct s_world *world);
void			capsule_add_xy_velocity(t_vector2 vel, \
							t_character_physics *phys, struct s_world *world);
void			capsule_phys_step(t_character_physics *charp, \
									struct s_world *world);
void			capsule_apply_gravity(t_character_physics charp, \
								struct s_world *world);

// Don't use this one for general use. Use line_circle_collision instead
// Saves the collision point to t_collision *
bool			collision_line_circle_save_collision(t_line line, \
				t_vector2 circle, float radius, t_collision *collision);

// Use this for general use
bool			collision_line_circle(t_line line, t_vector2 circle, \
										float radius);

//Internal function, don't call outside of collision logic
bool			charphys_floor_share_z(t_character_physics *cp, \
										struct s_meshtri *floor);
//Internal function, don't call outside of collision logic
bool			touches_edges(t_vector2 pos, float radius, \
								struct s_meshtri *floor);
//Internal function, don't call outside of collision logic
bool			touches_edges_ramp(t_vector2 pos, float radius, \
									t_vector3_tri ramp);
//Internal function, don't call outside of collision logic
bool			charphys_ramp_share_z(t_character_physics *cp, \
										t_vector3_tri ramp);
//Internal function, don't call outside of collision logic
struct s_bound	get_bound(t_character_physics *cp, struct s_world *world);
//Internal function, don't call outside of collision logic
float			get_z_from_areas(t_character_physics *cp, \
									struct s_world *world);
//Internal function, don't call outside of collision logic
bool			is_in_ramp(t_character_physics *cp, t_vector3_tri *ramp);

bool			is_in_ceil(t_character_physics *cp, struct s_meshtri *ceil);

t_vector3_tri	worldspace_tri(struct s_entity *entity, int index);

#endif