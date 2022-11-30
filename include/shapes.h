/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 13:54:56 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/30 17:04:18 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPES_H
# define SHAPES_H

#include "vectors.h"

typedef struct s_rectangle
{
	t_point	position;
	t_point	size;
}	t_rectangle;

typedef struct s_square
{
	t_vector2 min;
	t_vector2 max;
}	t_square;

typedef struct s_edge
{
	t_vector3	v[2];
}	t_edge;

# endif