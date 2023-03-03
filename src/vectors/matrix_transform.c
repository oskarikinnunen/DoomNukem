#include "vectors.h"

t_mat4x4 matrix_makescale(t_vector3 v)
{
	t_mat4x4 matrix = matrix_makeidentity();
	matrix.m[0][0] = v.x;
	matrix.m[1][1] = v.y;
	matrix.m[2][2] = v.z;
	return (matrix);
}

t_mat4x4 matrix_maketranslation(t_vector3 v)
{
	t_mat4x4 matrix = matrix_makeidentity();
	matrix.m[3][0] = v.x;
	matrix.m[3][1] = v.y;
	matrix.m[3][2] = v.z;
	return matrix;
}

#include "doomnukem.h"

static t_vector3 relative(t_vector2 angle, t_vector3 pos)
{
	t_quaternion	temp;
	t_mat4x4		matcamerarot;

	matcamerarot = matrix_makerotationy(angle.y);
	temp = quaternion_mul_matrix(matcamerarot, (t_quaternion){pos.x, pos.y, pos.z, 1.0f});
	matcamerarot = matrix_makerotationz(angle.x);
	temp = quaternion_mul_matrix(matcamerarot, temp);
	return (temp.v);
}

t_mat4x4 make_transform_matrix(t_transform transform)
{
	t_mat4x4 matworld;
	t_mat4x4 matrotz, matroty, matrotx;
	t_mat4x4	matscale;
	t_mat4x4	matpos, matparentpos;
	if (transform.parent != NULL)
	{
		//return (matrix_zero());
	
		matrotz = matrix_makerotationz(transform.rotation.x);
		matrotz = matrix_multiply_matrix(matrotz, matrix_makerotationz(transform.parent->rotation.x));

		matroty = matrix_makerotationy(transform.rotation.y);
		matroty = matrix_multiply_matrix(matroty, matrix_makerotationy(transform.parent->rotation.y));
		matrotx = matrix_makerotationx(0.1f);

		matscale = matrix_makescale(transform.scale);
		matparentpos = matrix_maketranslation(transform.parent->position);

		
		t_vector3 forward, up, right;
		t_quaternion result;
		result = quaternion_rotate_euler(result.v, transform.parent->rotation);
		forward = lookdirection((t_vector2){transform.parent->rotation.x, transform.parent->rotation.y});
		up = lookdirection((t_vector2){transform.parent->rotation.x, transform.parent->rotation.y + RAD90});
		right = vector3_crossproduct(forward, vector3_up());
		forward = vector3_normalise(forward);
		right = vector3_normalise(right);
		up = vector3_normalise(up);
		result.v = vector3_add(result.v, vector3_mul(forward, transform.position.y));
		result.v = vector3_add(result.v, vector3_mul(right, transform.position.x));
		result.v = vector3_add(result.v, vector3_mul(up, transform.position.z));
		result.v = vector3_add(transform.parent->position, result.v);
		matparentpos = matrix_maketranslation(result.v);



		matpos = matrix_maketranslation(transform.position);

		//matworld = matrix_multiply_matrix(matrotz, matroty);
		//matworld = matrix_multiply_matrix(matworld, matrotx);
		//matworld = matrix_multiply_matrix(matscale, matrix_multiply_matrix(matroty, matrotz));
		matworld = matrix_multiply_matrix(matworld, matrotx);
		matworld = matrix_multiply_matrix(matworld, matparentpos);
		//matworld = matscale;
		//matworld = matrix_multiply_matrix(matworld, matscale);
		//matworld = matrix_multiply_matrix(matworld, matparentpos);
	}
	else
	{
		matrotz = matrix_makerotationz(transform.rotation.x);
		matroty = matrix_makerotationy(transform.rotation.y);
		matrotx = matrix_makerotationx(transform.rotation.z);

		matscale = matrix_makescale(transform.scale);
		matpos = matrix_maketranslation(transform.position);
		matworld = matrix_multiply_matrix(matrotz, matroty);
		matworld = matrix_multiply_matrix(matworld, matrotx);
		matworld = matrix_multiply_matrix(matworld, matscale);
		matworld = matrix_multiply_matrix(matworld, matpos);
	}
	return (matworld);
}
