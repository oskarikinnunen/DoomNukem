/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:53:58 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/05 14:32:24 by okinnune         ###   ########.fr       */
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

bool			pointtrianglecollision (t_point point, struct s_triangle tri); //TODO: pls deprecate, the other one is better
bool			pointtrianglecollisionp (t_point point, t_point	t1, t_point	t2, t_point	t3);
bool			linelineintersect(struct s_line line1, struct s_line line2);

#endif