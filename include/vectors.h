/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 16:25:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/11 17:15:42 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTORS_H
# define VECTORS_H

/* t_vector3 functions */
typedef struct s_vector3
{
	float	x;
	float	y;
	float	z;
}	t_vector3;

//returns vector 'first' added by vector 'second'
t_vector3	vector3_add(t_vector3 first, t_vector3 second);

//returns vector 'first' substracted by vector 'second'
t_vector3	vector3_sub(t_vector3 first, t_vector3 second);

//returns vector 'vec' with 'add' added to x, y and z
t_vector3	vector3_add_xyz(t_vector3 vec, float add);

//returns vector 'vec' multiplied by 'mul'
t_vector3	vector3_mul(t_vector3 vec, float mul);

//returns vector 'vec' divided by 'div'
t_vector3	vector3_div(t_vector3 vec, float div);

//returns distance between first and second vector
float		vector3_dist(t_vector3 first, t_vector3 second);

//returns true if both vectors are identical
bool		vector3_cmp(t_vector3 first, t_vector3 second);

/* t_vector2 functions */
typedef struct s_vector2
{
	float	x;
	float	y;
}	t_vector2;

//returns vector 'first' added by vector 'second'
t_vector2	vector2_add(t_vector2 first, t_vector2 second);

//returns vector 'first' substracted by vector 'second'
t_vector2	vector2_sub(t_vector2 first, t_vector2 second);

//returns vector 'vec' with 'add' added to x and y
t_vector2	vector2_add_xy(t_vector2 vec, float add);

//returns vector 'vec' multiplied by 'mul'
t_vector2	vector2_mul(t_vector2 vec, float mul);

//returns vector 'vec' divided by 'div'
t_vector2	vector2_div(t_vector2 vec, float div);

//returns distance between first and second vector
float		vector2_dist(t_vector2 first, t_vector2 second);

//returns true if both vectors are identical
bool		vector2_cmp(t_vector2 first, t_vector2 second);

typedef struct s_quaternion
{
	float	x;
	float	y;
	float	z;
	float	w;
}	t_quaternion;

typedef struct s_point
{
	int		x;
	int		y;
}	t_point;

#endif