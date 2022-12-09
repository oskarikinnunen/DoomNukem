/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 16:25:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/08 18:21:08 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTORS_H
# define VECTORS_H

#include <math.h>
#include <stdbool.h>
#include "libft.h"

/* t_vector2 functions */
typedef struct s_vector2
{
	float	x;
	float	y;
}	t_vector2;

//shorthand for (t_vector2){0, 0}
t_vector2	vector2_zero();

//shorthand for (t_vector2){1, 1}
t_vector2	vector2_one();

//returns 'vec' but with x and y signs flipped //TODO: explain this better lol
t_vector2	vector2_negative(t_vector2 vec);

//returns vector 'first' added by vector 'second'
t_vector2	vector2_add(t_vector2 first, t_vector2 second);

//returns vector 'first' substracted by vector 'second'
t_vector2	vector2_sub(t_vector2 first, t_vector2 second);

//returns vector 'vec' with 'add' added to x and y
t_vector2	vector2_add_xy(t_vector2 vec, float add);

//returns vector 'vec' snapped into 'grid' that is the size of 'interval' //TODO: better explanation
t_vector2	vector2_snap(t_vector2 vec, int interval);

//returns vector 'vec' multiplied by 'mul'
t_vector2	vector2_mul(t_vector2 vec, float mul);

//returns vector 'vec' divided by 'div'
t_vector2	vector2_div(t_vector2 vec, float div);

//returns vector 'vec' normalised
t_vector2	vector2_normalise(t_vector2 vec);

//returns the magnitude of the vector 'vec'
float		vector2_magnitude(t_vector2 vec);

//returns the squared magnitude of the vector 'vec'
float		vector2_sqr_magnitude(t_vector2 vec);

//returns distance between first and second vector
float		vector2_dist(t_vector2 first, t_vector2 second);

//returns dot product of vector 'first' and vector 'second'
float		vector2_dot(t_vector2 first, t_vector2 second);

//returns cross product of vector 'first' and vector 'second'
float		vector2_cross(t_vector2 first, t_vector2 second);

//returns interpolated vector between 'v1' and 'v2' TODO: explain better lol
t_vector2	vector2_lerp(t_vector2 v1, t_vector2 v2, float lerp);

//returns absolute vector2
t_vector2	vector2_abs(t_vector2 vec);

//returns the angle in radians between two vectors
float vector2_anglebetween(t_vector2 first, t_vector2 second);

//returns true if both vectors are identical
bool		vector2_cmp(t_vector2 first, t_vector2 second);

//returns copy of 'vec' but with magnitude clamped to 'max_magnitude'
t_vector2	vector2_clamp_magnitude(t_vector2 vec, float max_magnitude);

//returns float distance, to plane_p position and direction vector plane_n
float vector2_fdist_to_plane(t_vector2 p, t_vector2 plane_n, t_vector2 plane_p);

//returns float from 0 to 1 depending on where the line intersected plane. Pass t_vector3 plane_p location and t_vector3 plane_n normalised, currently function does normalise plane_n just in case, but it will be changed later.
float	vector2_line_intersect_plane(t_vector2 plane_p, t_vector2 plane_n, t_vector2 start, t_vector2 end);

/* t_vector3 functions */
typedef struct s_vector3
{
	float	x;
	float	y;
	float	z;
}	t_vector3;

//shorthand for (t_vector3){0, 0, 0}
t_vector3	vector3_zero();

//shorthand for (t_vector3){1, 1, 1}
t_vector3	vector3_one();

//shorthand for (t_vector3){0, 0, 1}
t_vector3	vector3_up();

//shorthand for (t_vector3){0, 0, -1}
t_vector3	vector3_down();

//returns 'vec' but with x, y, and z signs flipped //TODO: explain this better lol
t_vector3	vector3_negative(t_vector3 vec);

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

//returns vector 'vec' moved towards set 'direction' //TODO: explain delta
t_vector3	vector3_movetowards(t_vector3 vec, t_vector3 direction, float delta);

//returns the magnitude of the vector 'vec'
float		vector3_magnitude(t_vector3 vec);

//returns the squared magnitude of the vector 'vec'
float		vector3_sqr_magnitude(t_vector3 vec);

//returns distance between first and second vector
float		vector3_dist(t_vector3 first, t_vector3 second);

//returns dot product of vector 'first' and vector 'second'
float		vector3_dot(t_vector3 first, t_vector3 second);

//returns true if both vectors are identical
bool		vector3_cmp(t_vector3 first, t_vector3 second);

//returns vector3 multiplied by vector 'v1' and vector 'v2'
t_vector3	vector3_mul_vector3(t_vector3 v1, t_vector3 v2);

//normalises vector 'v' and returns it
t_vector3	vector3_normalise(t_vector3 v);

//returns crossproduct from vector 'v1' and vector 'v2'
t_vector3	vector3_crossproduct(t_vector3 v1, t_vector3 v2);

//returns copy of 'vec' but with magnitude clamped to 'max_magnitude'
t_vector3	vector3_clamp_magnitude(t_vector3 vec, float max_magnitude);

//returs vector 'vec' rotated around vector3_zero() by eulerangles
t_vector3	vector3_rotate_euler(t_vector3 original, t_vector3 eulers);

//returns signed shortest distance from point to plane, plane normal must be normalised
float		vector3_fdist_to_plane(t_vector3 p, t_vector3 plane_n, t_vector3 plane_p);

//returns float from 0 to 1 depending on where the line intersected plane. Pass t_vector3 plane_p location and t_vector3 plane_n normalised, currently function does normalise plane_n just in case, but it will be changed later.
float	line_intersect_plane(t_vector3 plane_p, t_vector3 plane_n, t_vector3 start, t_vector3 end);

//returns linearly interpolated value between 'v1' and 'v2' using lerp (which should be in range 0.0 - 1.0f)
t_vector3	vector3_lerp(t_vector3 v1, t_vector3 v2, float lerp);

typedef struct s_point
{
	int		x;
	int		y;
}	t_point;

//shorthand for (t_point){0, 0}
t_point		point_zero();

//shorthand for (t_point){1, 1}
t_point		point_one();

//returns 'point' but with x and y signs flipped //TODO: explain this better lol
t_point		point_negative(t_point point);

//returns point 'first' added by point 'second'
t_point		point_add(t_point first, t_point second);

//returns point 'first' substracted by point 'second'
t_point		point_sub(t_point first, t_point second);

//returns point 'point' where 'x = x % mod' and 'y = y % mod'
t_point		point_mod(t_point point, int	mod);

//returns point 'point' with 'add' added to x and y
t_point		point_add_xy(t_point point, int add);

//returns point 'point' snapped into 'grid' that is the size of 'interval' //TODO: better explanation
t_point		point_snap(t_point point, int interval);

//returns point 'point' multiplied by 'mul'
t_point		point_mul(t_point point, int mul);

//returns point 'point' divided by 'div'
t_point		point_div(t_point point, int div);

//returns point 'point' multiplied by float 'mul'
t_point		point_fmul(t_point point, float mul);

//returns point 'point' divided by float 'mul'
t_point		point_fdiv(t_point point, float mul);

//returns point 'point' with absolute x and y values
t_point		point_abs(t_point point);

//returns point 'point' normalised
t_point		point_normalise(t_point point);

//returns the magnitude of the point 'point'
int			point_magnitude(t_point point);

//returns the squared magnitude of the point 'point'
int			point_sqr_magnitude(t_point point);

//returns distance between first and second point
int			point_dist(t_point first, t_point second);

//returns dot product of point 'first' and point 'second'
int			point_dot(t_point first, t_point second);

//returns true if both points are identical
bool		point_cmp(t_point first, t_point second);

//returns the squared magnitude of the point 'point'
float		point_sqr_fmagnitude(t_point point);

//returns distance between first and second point
float		point_fdist(t_point first, t_point second);

//returns dot product of point 'first' and point 'second'
float		point_fdot(t_point first, t_point second);

//returns point lerped;
t_point lerp_point(t_point from, t_point to, float delta);

typedef struct s_quaternion
{
	t_vector3	v;
	float		w;
}	t_quaternion;

//shorthand for writing (t_quaternion){1.0f, 1.0f, 1.0f, 1.0f}
t_quaternion	quaternion_identity();

t_quaternion	quaternion_rotate_euler(t_vector3 original, t_vector3 eulers);

//returns quaternion multiplication result 'first * second'
t_quaternion	quaternion_mul(t_quaternion first, t_quaternion second);

//returns quaternion, for quaternion line that starts from 'linestart' and ends in 'lineend' and intersects plane vector 'plane_p' and 'plane_n'
t_quaternion	quaternion_intersectplane(t_vector3 plane_p, t_vector3 plane_n, t_quaternion lineStart, t_quaternion lineEnd, float *t);

//returns quaternion lerped from, to by delta
t_quaternion	lerp_quaternion(t_quaternion from, t_quaternion to, float delta);

typedef struct s_texture
{
	float	u;
	float	v;
	float	w;
} t_texture;

//returns texture lerped from, to by delta
t_texture		lerp_texture(t_texture from, t_texture to, float delta);

typedef struct s_mat4x4
{
	float	m[4][4];
}	t_mat4x4;
//TODO: check all matrix functions and flip variables. For example function vector3_mul_matrix(matrix, vector) should be (vector, matrix)

//shorthand for (t_mat4x4){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
t_mat4x4 matrix_zero();

//returns zeroed matrix, with [0][0], [1][1], [2][2], [3][3] set to 1.0f
t_mat4x4 matrix_makeidentity();

//returns rotation matrix for angle x
t_mat4x4 matrix_makerotationx(float fAngleRad);

//returns rotation matrix for angle y
t_mat4x4 matrix_makerotationy(float fAngleRad);

//returns rotation matrix for angle z
t_mat4x4 matrix_makerotationz(float fAngleRad);

//matrix makes identity matrix and sets matrix[3][0] = 'x', matrix.m[3][1] = 'y', matrix.m[3][2] = 'z'
t_mat4x4 matrix_maketranslation(float x, float y, float z);

//returns matrix projection, fFovDegrees is player field of view in degrees, aspect ratio is float window height divided by window width, fnear is how close the camera clips and ffar is how far the camera clips 
t_mat4x4 matrix_makeprojection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);

//returns matrix multiplied by matrix 'm1' and matrix 'm2'
t_mat4x4 matrix_multiply_matrix(t_mat4x4 m1, t_mat4x4 m2);

//returns matrix looking at vector 'target' from vector 'pos' vector 'up' being which axis is up
t_mat4x4 matrix_lookat(t_vector3 pos, t_vector3 target, t_vector3 up);

//returns matrix 'm' with matrix[3][0], matrix[3][1], matrix[3][2] inversed and matrix[3][3] == 1.0f
t_mat4x4 matrix_quickinverse(t_mat4x4 m);

//returns quaternion 'i' multiplied by matrix 'm'
t_quaternion quaternion_mul_matrix(t_mat4x4 m, t_quaternion i);

//returns vector3 lookdirection, from vector2 angle
t_vector3	lookdirection(t_vector2 angle);

//returns vector3 lookdirection, from vector3 angle (only uses x and y for now)
t_vector3	lookdirection_3(t_vector3 angle);

//returns vector3 'i' multiplied by matrix 'm'
t_vector3 vector3_mul_matrix(t_mat4x4 m, t_vector3 i);

/* CONVERSIONS */
float	radtodeg(float rad);

t_quaternion	vector3_to_quaternion(t_vector3 v);

t_vector3		vector2_to_vector3(t_vector2 vec);

//Returns t_vector2 'vec' casted to t_point. (Shorthand for '*(t_point *)&vec').
t_point			vector2_to_point(t_vector2 vec);

//Returns t_point 'point' casted to t_vector2. (Shorthand for '*(t_vector2 *)&point').
t_vector2		point_to_vector2(t_point point);

//Returns static str for given vector3
char			*vector_string(t_vector3 vec);

t_texture		vector2_to_texture(t_vector2 v);

#endif