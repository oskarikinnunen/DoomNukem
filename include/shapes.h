/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 13:54:56 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/12 09:07:12 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPES_H
# define SHAPES_H

#include "vectors.h"
#include "inttypes.h"

typedef struct s_rectangle
{
	t_point	position;
	t_point	size;
}	t_rectangle;

typedef struct s_square //TODO: maybe rectangle?
{
	t_point min;
	t_point max;
}	t_square;

typedef struct s_v2rectangle
{
	t_vector2	min;
	t_vector2	max;
}	t_v2rectangle;

typedef struct	s_triangle
{
	t_quaternion	p[3];
	t_texture		t[3];
	uint32_t		clr;
}	t_triangle;

typedef struct s_point_triangle
{
	t_point			p[3];
	t_texture		t[3];
	uint32_t		clr;
}	t_point_triangle;

typedef struct s_edge
{
	t_vector3	v[2];
}	t_edge;

# endif