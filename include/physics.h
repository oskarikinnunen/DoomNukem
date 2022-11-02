/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:53:58 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 20:33:37 by okinnune         ###   ########.fr       */
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

void calculate_colliders(t_physics *p);
void draw_colliders(t_physics p, struct s_sdlcontext sdl, struct s_render render);

#endif