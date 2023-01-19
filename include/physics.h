/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:53:58 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/19 05:26:41 by okinnune         ###   ########.fr       */
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

typedef struct s_characterphysics
{
	float		height;
	float		radius;
	float		*gravity_override;
	t_vector3	*position;
	t_vector3	*velocity;
	bool		*isgrounded;
	bool		*landingtrigger;
	t_vector3	*impactvelocity;
}	t_characterphysics;

bool	check_collision_character(struct s_world *world, t_characterphysics cp, t_vector3 potential_pos, t_vector3 *new_pos);
void	apply_capsulephysics(t_characterphysics charp, struct s_world *world);
bool	pointtrianglecollision (t_point point, struct s_triangle tri); //TODO: pls deprecate, the other one is better
bool	pointtrianglecollisionp (t_point point, t_point	t1, t_point	t2, t_point	t3);
bool	linelineintersect(struct s_line line1, struct s_line line2);

#endif