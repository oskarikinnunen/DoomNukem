/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 13:54:56 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 20:23:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPES_H
# define SHAPES_H

# include "vectors.h"
# include "inttypes.h"

# define CIRCLE_SIDES 8

typedef struct s_rectangle
{
	t_point	position;
	t_point	size;
}	t_rectangle;

//TODO: maybe rectangle?
typedef struct s_square
{
	t_point	min;
	t_point	max;
}	t_square;

typedef struct s_v2_rectangle
{
	t_vector2	min;
	t_vector2	max;
}	t_v2_rectangle;

typedef struct s_world_triangle
{
	t_quaternion	p[3];
	t_vector3		t[3];
}	t_world_triangle;

typedef struct s_triangle_v3
{
	t_vector3	a;
	t_vector3	b;
	t_vector3	c;
}	t_triangle_v3;

typedef struct s_triangle_v2
{
	t_vector2	a;
	t_vector2	b;
	t_vector2	c;
}	t_triangle_v2;

typedef struct s_box
{
	t_vector3	max;
	t_vector3	min;
}	t_box;

typedef struct s_screen_triangle
{
	t_vector2	p[3];
	t_vector3	t[3];
}	t_screen_triangle;

typedef struct s_line
{
	t_vector2	start;
	t_vector2	end;
}	t_line;

#endif