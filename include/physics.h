/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:53:58 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/07 06:57:24 by okinnune         ###   ########.fr       */
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

void			calculate_colliders(t_physics *p);
void			draw_colliders(t_physics p, struct s_sdlcontext sdl, struct s_render render);
struct s_entity	*entity_collides(t_physics p, struct s_entity ent);
bool pointtrianglecollision (t_point point, struct s_triangle tri);

#endif