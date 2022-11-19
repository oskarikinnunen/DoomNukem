/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:32:41 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/19 13:34:31 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

#include "vectors.h"

typedef struct s_transform
{
	t_vector3	location;
	t_vector3	rotation;
	t_vector3	scale;
}	t_transform;

typedef struct s_bound
{
	float	min;
	float	max;
}	t_bound;

typedef struct s_boundingbox
{
	t_vector3		boundingbox[8];
	float			height;
	bool			is_wall;
}	t_boundingbox;

typedef struct s_entity
{
	t_boundingbox	boundingbox;
	t_transform		transform;
	t_bound			z_bound;
	uint32_t		object_index;
	struct s_object	*obj;
}	t_entity;

typedef struct s_bot
{
	t_entity	entity;
}	t_bot;

typedef struct s_item
{
	t_entity	entity;
}	t_item;

#endif