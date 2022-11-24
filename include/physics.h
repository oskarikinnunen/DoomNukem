/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:53:58 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/24 16:01:12 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DEPRECATED UPDATED TO COLLISION.H*/
#ifndef PHYSICS_H
# define PHYSICS_H

#include "entity.h"
#include "render.h"

typedef struct s_physics
{
	t_entity		*entities[20]; //points to entities in 'editor->entitylist'
	int				cube[100][100][10];
}	t_physics;

struct s_render;
struct s_sdlcontext;
struct s_entity;
struct s_triangle;
struct s_line;

void			calculate_colliders(t_physics *p);
void			draw_colliders(t_physics p, struct s_sdlcontext sdl, struct s_render render);
struct s_entity	*entity_collides(t_physics p, struct s_entity ent);
bool			pointtrianglecollision (t_point point, struct s_triangle tri); //TODO: pls deprecate, the other one is better
bool			pointtrianglecollisionp (t_point point, t_point	t1, t_point	t2, t_point	t3);
bool			linelineintersect(struct s_line line1, struct s_line line2);

#endif