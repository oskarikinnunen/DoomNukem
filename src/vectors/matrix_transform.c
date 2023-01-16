#include "vectors.h"

t_mat4x4 matrix_makescale(t_vector3 v)
{
	t_mat4x4 matrix = matrix_makeidentity();
	matrix.m[0][0] = v.x;
	matrix.m[1][1] = v.y;
	matrix.m[2][2] = v.z;
	return matrix;
}

t_mat4x4 matrix_maketranslation(t_vector3 v)
{
	t_mat4x4 matrix = matrix_makeidentity();
	matrix.m[3][0] = v.x;
	matrix.m[3][1] = v.y;
	matrix.m[3][2] = v.z;
	return matrix;
}

t_mat4x4 make_transform_matrix(t_transform transform)
{
	t_mat4x4 matrotz, matroty, matrotx;
	matrotz = matrix_makerotationz(transform.rotation.x);
	matroty = matrix_makerotationy(transform.rotation.y);
	matrotx = matrix_makerotationx(transform.rotation.z);

	t_mat4x4	matscale;
	matscale = matrix_makescale(transform.scale);

	t_mat4x4	matpos;
	matpos = matrix_maketranslation(transform.position);
	
	t_mat4x4 matworld;
	matworld = matrix_multiply_matrix(matrotz, matroty);
	matworld = matrix_multiply_matrix(matworld, matrotx);
	matworld = matrix_multiply_matrix(matworld, matscale);
	matworld = matrix_multiply_matrix(matworld, matpos);
	return(matworld);
}
