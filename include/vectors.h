/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 16:25:20 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:51:44 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTORS_H
# define VECTORS_H

# include <math.h>
# include <stdbool.h>
# include "libft.h"

//returns true if v1 == v2 in the error range of epsilon
bool			float_cmp_epsilon(float v1, float v2, float epsilon);

/* t_vector2 functions */
typedef struct s_vector2
{
	float	x;
	float	y;
}	t_vector2;

struct	s_line;
struct	s_sdlcontext;

//returns closest flo
t_vector2		vector2_ceilf(t_vector2 v);

t_vector2		vector2_floorf(t_vector2 v);

t_vector2		vector2_roundf(t_vector2 v);

t_vector2		vector2_truncf(t_vector2 v);

//shorthand for (t_vector2){0, 0}
t_vector2		vector2_zero(void);

//shorthand for (t_vector2){1, 1}
t_vector2		vector2_one(void);

//TODO: explain this better lol
//returns 'vec' but with x and y signs flipped
t_vector2		vector2_negative(t_vector2 vec);

//returns vector 'first' added by vector 'second'
t_vector2		vector2_add(t_vector2 first, t_vector2 second);

//returns vector 'first' substracted by vector 'second'
t_vector2		vector2_sub(t_vector2 first, t_vector2 second);

//returns vector 'vec' with 'add' added to x and y
t_vector2		vector2_add_xy(t_vector2 vec, float add);

//TODO: better explanation
//returns vector 'vec' snapped into 'grid' that is the size of 'interval'
t_vector2		vector2_snap(t_vector2 vec, int interval);

//returns vector 'vec' multiplied by 'mul'
t_vector2		vector2_mul(t_vector2 vec, float mul);

//returns vector 'first' multiplied by 'second'
t_vector2		vector2_multiply(t_vector2 first, t_vector2 second);

//returns vector 'vec' divided by 'div'
t_vector2		vector2_div(t_vector2 vec, float div);

//returns vector 'vec' normalised
t_vector2		vector2_normalise(t_vector2 vec);

//returns the magnitude of the vector 'vec'
float			vector2_magnitude(t_vector2 vec);

//returns the squared magnitude of the vector 'vec'
float			vector2_sqr_magnitude(t_vector2 vec);

//return the approximate distance between vectors 'first' and 'second'
float			vector2_sqr_dist(t_vector2 first, t_vector2 second);

//returns distance between first and second vector
float			vector2_dist(t_vector2 first, t_vector2 second);

//returns dot product of vector 'first' and vector 'second'
float			vector2_dot(t_vector2 first, t_vector2 second);

//returns cross product of vector 'first' and vector 'second'
float			vector2_cross(t_vector2 first, t_vector2 second);

//TODO: explain better lol
//returns interpolated vector between 'v1' and 'v2'
t_vector2		vector2_lerp(t_vector2 v1, t_vector2 v2, float lerp);

//returns absolute vector2
t_vector2		vector2_abs(t_vector2 vec);

//returns the angle in radians between two vectors
float			vector2_anglebetween(t_vector2 first, t_vector2 second);

//returns true if both vectors are identical
bool			vector2_cmp(t_vector2 first, t_vector2 second);

//returns copy of 'vec' but with magnitude clamped to 'max_magnitude'
t_vector2		vector2_clamp_magnitude(t_vector2 vec, float max_magnitude);

//returns float distance, to plane_p position and direction vector plane_n
float			vector2_fdist_to_plane(t_vector2 p, t_vector2 plane_n, \
									t_vector2 plane_p);

//returns vector 2 abs
t_vector2		vector2_abs(t_vector2 v);

//returns true if v1 == v2 in the error range of epsilon
bool			vector2_cmp_epsilon(t_vector2 v1, t_vector2 v2, float epsilon);

//returns distance of vec along line
float			vector2_dist_along_line(t_vector2 vec, struct s_line line);

//returns float from 0 to 1 depending on where the line intersected plane.
//Pass t_vector3 plane_p location and t_vector3 plane_n normalised,
//currently function does normalise plane_n just in case,
//but it will be changed later.
float			vector2_line_intersect_plane(t_vector2 plane_p, \
					t_vector2 plane_n, t_vector2 start, t_vector2 end);

/* t_vector3 functions */
typedef struct s_vector3
{
	float	x;
	float	y;
	float	z;
}	t_vector3;

//shorthand for (t_vector3){0, 0, 0}
t_vector3		vector3_zero(void);

//shorthand for (t_vector3){1, 1, 1}
t_vector3		vector3_one(void);

//shorthand for (t_vector3){0, 0, 1}
t_vector3		vector3_up(void);

//shorthand for (t_vector3){0, 0, -1}
t_vector3		vector3_down(void);

//TODO: explain this better lol
//returns 'vec' but with x, y, and z signs flipped
t_vector3		vector3_negative(t_vector3 vec);

//returns vector 'first' added by vector 'second'
t_vector3		vector3_add(t_vector3 first, t_vector3 second);

//returns vector 'first' substracted by vector 'second'
t_vector3		vector3_sub(t_vector3 first, t_vector3 second);

//returns vector 'vec' with 'add' added to x, y and z
t_vector3		vector3_add_xyz(t_vector3 vec, float add);

//returns vector 'vec' multiplied by 'mul'
t_vector3		vector3_mul(t_vector3 vec, float mul);

//returns vector 'vec' divided by 'div'
t_vector3		vector3_div(t_vector3 vec, float div);

//TODO: explain delta
//returns vector 'vec' moved towards set 'direction'
t_vector3		vector3_move_towards(t_vector3 vec, t_vector3 to, float delta);

//returns absolute vector
t_vector3		vector3_abs(t_vector3 v);

//returns the magnitude of the vector 'vec'
float			vector3_magnitude(t_vector3 vec);

//returns the squared magnitude of the vector 'vec'
float			vector3_sqr_magnitude(t_vector3 vec);

//returns distance between first and second vector
float			vector3_dist(t_vector3 first, t_vector3 second);

//TODO: better explanation
//returns distance between first and second vector, but different
float			vector3_sqr_dist(t_vector3 first, t_vector3 second);

//returns dot product of vector 'first' and vector 'second'
float			vector3_dot(t_vector3 first, t_vector3 second);

//returns true if both vectors are identical
bool			vector3_cmp(t_vector3 first, t_vector3 second);

//returns true if both vectors are in the error range of epsilon
bool			vector3_cmp_epsilon(t_vector3 v1, t_vector3 v2, float epsilon);

t_vector3		vector3_snap(t_vector3 vec, int interval);

//returns vector3 multiplied by vector 'v1' and vector 'v2'
t_vector3		vector3_mul_vector3(t_vector3 v1, t_vector3 v2);

//normalises vector 'v' and returns it
t_vector3		vector3_normalise(t_vector3 v);

//returns crossproduct from vector 'v1' and vector 'v2'
t_vector3		vector3_cross_product(t_vector3 v1, t_vector3 v2);

//returns copy of 'vec' but with magnitude clamped to 'max_magnitude'
t_vector3		vector3_clamp_magnitude(t_vector3 vec, float max_magnitude);

//returs vector 'vec' rotated around vector3_zero() by eulerangles
t_vector3		vector3_rotate_euler(t_vector3 original, t_vector3 eulers);

//returns signed shortest distance from point to plane,
//plane normal must be normalised
float			vector3_fdist_to_plane(t_vector3 p, t_vector3 plane_n, \
									t_vector3 plane_p);

//returns float from 0 to 1 depending on where the line intersected plane.
//Pass t_vector3 plane_p location and t_vector3 plane_n normalised,
//currently function does normalise plane_n just in case,
//but it will be changed later.
float			line_intersect_plane(t_vector3 plane_p, t_vector3 plane_n, \
								t_vector3 start, t_vector3 end);

//returns linearly interpolated value between 'v1' and 'v2' using lerp
//(which should be in range 0.0 - 1.0f)
t_vector3		vector3_lerp(t_vector3 v1, t_vector3 v2, float lerp);

typedef struct s_transform
{
	t_vector3			position;
	t_vector3			rotation;
	t_vector3			scale;
	struct s_transform	*parent;
}	t_transform;

typedef struct s_point
{
	int		x;
	int		y;
}	t_point;

//shorthand for (t_point){0, 0}
t_point			point_zero(void);

//shorthand for (t_point){1, 1}
t_point			point_one(void);

//TODO: explain this better lol
//returns 'point' but with x and y signs flipped
t_point			point_negative(t_point point);

//returns point 'first' added by point 'second'
t_point			point_add(t_point first, t_point second);

//returns point 'first' substracted by point 'second'
t_point			point_sub(t_point first, t_point second);

//returns point 'point' where 'x = x % mod' and 'y = y % mod'
t_point			point_mod(t_point point, int mod);

//returns point 'point' with 'add' added to x and y
t_point			point_add_xy(t_point point, int add);

//TODO: better explanation
//returns point 'point' snapped into 'grid' that is the size of 'interval'
t_point			point_snap(t_point point, int interval);

//returns point 'point' multiplied by 'mul'
t_point			point_mul(t_point point, int mul);

//returns point 'point' divided by 'div'
t_point			point_div(t_point point, int div);

//returns point 'point' multiplied by float 'mul'
t_point			point_fmul(t_point point, float mul);

//returns point 'point' divided by float 'mul'
t_point			point_fdiv(t_point point, float mul);

//returns point 'point' with absolute x and y values
t_point			point_abs(t_point point);

//returns point 'point' normalised
t_point			point_normalise(t_point point);

//returns the magnitude of the point 'point'
int				point_magnitude(t_point point);

//returns the squared magnitude of the point 'point'
int				point_sqr_magnitude(t_point point);

//returns distance between first and second point
int				point_dist(t_point first, t_point second);

//returns dot product of point 'first' and point 'second'
int				point_dot(t_point first, t_point second);

//returns true if both points are identical
bool			point_cmp(t_point first, t_point second);

//returns the squared magnitude of the point 'point'
float			point_sqr_fmagnitude(t_point point);

//returns distance between first and second point
float			point_fdist(t_point first, t_point second);

//returns dot product of point 'first' and point 'second'
float			point_fdot(t_point first, t_point second);

typedef struct s_quaternion
{
	t_vector3	v;
	float		w;
}	t_quaternion;

//shorthand for writing (t_quaternion){1.0f, 1.0f, 1.0f, 1.0f}
t_quaternion	quaternion_identity(void);

t_quaternion	quaternion_rotate_euler(t_vector3 original, t_vector3 eulers);

//returns quaternion multiplication result 'first * second'
t_quaternion	quaternion_mul(t_quaternion first, t_quaternion second);

//returns quaternion lerped from, to by delta
t_quaternion	lerp_quaternion(t_quaternion from, t_quaternion to, \
								float delta);

typedef struct s_mat4x4
{
	float	m[4][4];
}	t_mat4x4;

//TODO: check all matrix functions and flip variables. 
//For example function vector3_mul_matrix(matrix, vector)
//should be (vector, matrix)

//shorthand for (t_mat4x4){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//							0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
t_mat4x4		matrix_zero(void);

//returns zeroed matrix, with [0][0], [1][1], [2][2], [3][3] set to 1.0f
t_mat4x4		matrix_make_identity(void);

//returns rotation matrix for angle x
t_mat4x4		matrix_make_rotation_x(float f_angle_rad);

//returns rotation matrix for angle y
t_mat4x4		matrix_make_rotation_y(float f_angle_rad);

//returns rotation matrix for angle z
t_mat4x4		matrix_make_rotation_z(float f_angle_rad);

//matrix makes identity matrix and sets matrix[3][0] =
//'x', matrix.m[3][1] = 'y', matrix.m[3][2] = 'z'
t_mat4x4		matrix_make_translation(t_vector3 v);

//returns matrix scale
t_mat4x4		matrix_make_scale(t_vector3 v);

//returns matrix projection, fFovDegrees is player field of view in degrees,
//aspect ratio is float window height divided by window width,
//fnear is how close the camera clips and ffar is how far the camera clips
t_mat4x4		matrix_make_projection(float f_fov_degrees, \
					float f_aspect_ratio, float f_near, float f_far);

//returns matrix multiplied by matrix 'm1' and matrix 'm2'
t_mat4x4		matrix_multiply_matrix(t_mat4x4 m1, t_mat4x4 m2);

//returns matrix looking at vector 'target' from
//vector 'pos' vector 'up' being which axis is up
t_mat4x4		matrix_lookat(t_vector3 pos, t_vector3 target, t_vector3 up);

//returns matrix 'm' with matrix[3][0], matrix[3][1],
//matrix[3][2] inversed and matrix[3][3] == 1.0f
t_mat4x4		matrix_quick_inverse(t_mat4x4 m);

//returns quaternion 'i' multiplied by matrix 'm'
t_quaternion	quaternion_mul_matrix(t_mat4x4 m, t_quaternion i);

//returns vector3 look_direction, from vector2 angle
t_vector3		look_direction(t_vector2 angle);

//returns vector3 look_direction, from vector3 angle (only uses x and y for now)
t_vector3		look_direction_3(t_vector3 angle);

//returns vector3 'i' multiplied by matrix 'm'
t_vector3		vector3_mul_matrix(t_mat4x4 m, t_vector3 i);

/* CONVERSIONS */
float			fov_deg_to_fov_rad(float fovdeg);

float			rad_to_deg(float rad);

t_quaternion	vector3_to_quaternion(t_vector3 v);

t_vector3		v2tov3(t_vector2 vec);

t_vector2		v3tov2(t_vector3 vec);

//Returns t_vector2 'vec' casted to t_point
//(Shorthand for '*(t_point *)&vec')
t_point			vector2_to_point(t_vector2 vec);

//Returns t_point 'point' casted to t_vector2
//(Shorthand for '*(t_vector2 *)&point')
t_vector2		point_to_vector2(t_point point);

//Returns a point on the screen that is calculated by interpolating the screen
//size with the given x and y values using linear interpolation
t_point			screenlerp(float x_lerp, float y_lerp, \
							struct s_sdlcontext *sdl);

//Returns static str for given vector3
char			*vector_string(t_vector3 vec);

typedef struct s_ray
{
	t_vector3	origin;
	t_vector3	dir;
}	t_ray;

// triangle math functions

//normal barycentric coordinats x is delta value between
//p0 and p1 and y is delta between p1 and p2
t_vector2		barycentric_coordinates(t_vector2 *p, t_vector2 v);

//returns vector3 location from 3 t_vector3 points,
//using t_vector2 as barycentric coordinates for w1 and w2;
t_vector3		get_vector3_from_barycentric(t_vector3 *p, t_vector2 bary);

//returns normal for 3  vector3 points
t_vector3		normal_calc(t_vector3 p[3]);

//returns normal for 3 quaternion points
t_vector3		normal_calc_quaternion(t_quaternion p[3]);

//returns max values from 3 t_vector2 points
t_vector2		get_max_from_vector2_triangle(t_vector2 p[3]);

//returns min values from 3 t_vector2 points
t_vector2		get_min_from_vector2_triangle(t_vector2 p[3]);

//returns max x and y from 3 t_vector3
t_vector2		get_max_vector2_from_vector3_triangle(t_vector2 max, \
														t_vector3 t[3]);

//>= 1.0f == counterclockwise, <= -1.0f == clockwise and 0.0f is collinear
float			clockwise(t_vector2 p[3]);

//sorts 3 quaternions and 3 vector3 by 3 distances
void			sort_quaternion_vector3_by_dist(float dist[3], \
							t_quaternion q[3], t_vector3 t[3]);

//sorts 3 vector2 vector3 by vector2 y
void			sort_vector2_vector3_by_dist(float dist[3], t_vector2 p[3], \
													t_vector3 t[3]);

//returns true if triangle is degenerate
bool			is_triangle_degenerate(t_vector3 *p);

#endif