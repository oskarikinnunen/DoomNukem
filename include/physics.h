/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:53:58 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/09 19:16:01 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DEPRECATED UPDATED TO COLLISION.H*/
#ifndef PHYSICS_H
# define PHYSICS_H

#include "entity.h"
#include "render.h"

struct s_render;
struct s_sdlcontext;
struct s_entity;
struct s_triangle;
struct s_line;
struct s_world;

/*
make xy_physics controller
*/

typedef struct s_characterphysics
{
	float		height;
	float		radius;
	float		*gravity_override;
	t_vector3	*position;
	t_vector3	velocity;
	float		max_velocity;
	bool		isgrounded;
	bool		landingtrigger;
	bool		ceilingtrigger;
	t_vector3	impactvelocity;
}	t_characterphysics;

bool	check_collision_character(struct s_world *world, t_characterphysics cp, t_vector3 potential_pos, t_vector3 *new_pos);
void	capsule_damp(t_characterphysics *phys, struct s_world *world);
void	capsule_add_xy_velocity(t_vector2 vel, t_characterphysics *phys, struct s_world *world);
void	capsule_applygravity_new(t_characterphysics *charp, struct s_world *world);
void	capsule_applygravity(t_characterphysics charp, struct s_world *world);
bool	pointtrianglecollision (t_point point, struct s_triangle tri); //TODO: pls deprecate, the other one is better
bool	pointtrianglecollisionp (t_point point, t_point	t1, t_point	t2, t_point	t3);
bool	linelineintersect(struct s_line line1, struct s_line line2);

#endif