/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:53:58 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 15:06:43 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHYSICS_H
# define PHYSICS_H

#include "doomnukem.h" //TODO: only needs entity

typedef struct s_physics
{
	t_entity	*entities[20]; //points to entities in 'editor->entitylist'
	uint8_t		cube[512][512][512];
}	t_physics;

//void	physics_gather_entities

#endif