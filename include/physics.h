/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:53:58 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/18 18:30:35 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHYSICS_H
# define PHYSICS_H

#include "doomnukem.h" //TODO: only needs entity

typedef struct s_physics
{
	struct s_entity	*entities[20]; //points to entities in 'editor->entitylist'
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