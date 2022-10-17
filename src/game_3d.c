/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_3d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/17 22:38:26 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"

#define PRINT_DEBUG 1
#define SH WINDOW_H
#define SW WINDOW_W

static void printf_matrix(t_mat4x4 m)
{
	int i = 0;
	int j = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			printf("	mat[%d][%d] = %f", i, j, m.m[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

void printf_vec(t_vector3 v)
{
	printf("x %f y %f z %f\n", v.x, v.y, v.z);
}

void printf_quat(t_quaternion v)
{
	printf("w %f ", v.w);
	printf_vec(v.v);
}

void printf_tri(t_triangle tri)
{
	printf("triangle start\n");
	printf_quat(tri.p[0]);
	printf_quat(tri.p[1]);
	printf_quat(tri.p[2]);
	printf("triangle ends\n");
}

t_quaternion MMatrix_MultiplyVector(t_mat4x4 m, t_quaternion i)
{
	t_quaternion v;// = Initv3();
	v.v.x = i.v.x * m.m[0][0] + i.v.y * m.m[1][0] + i.v.z * m.m[2][0] + i.w * m.m[3][0];
	v.v.y = i.v.x * m.m[0][1] + i.v.y * m.m[1][1] + i.v.z * m.m[2][1] + i.w * m.m[3][1];
	v.v.z = i.v.x * m.m[0][2] + i.v.y * m.m[1][2] + i.v.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.v.x * m.m[0][3] + i.v.y * m.m[1][3] + i.v.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}

t_mat4x4 Init()
{
	t_mat4x4 m;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			m.m[i][j] = 0.0f;
		}
	}
	return(m);
}
/*
t_quaternion	Initv3()
{
	t_quaternion	v3;
	v3.w = 1.0f;
	v3.x = 0.0f;
	v3.y = 0.0f;
	v3.z = 0.0f;
	return(v3);
}*/

t_quaternion Initquater()
{
	t_vector3 v;
	t_quaternion q;

	v.x = 0.0f;
	v.y = 0.0f;
	v.z = 0.0f;
	q.v = v;
	q.w = 1.0f;
	return (q);
}

t_triangle Inittri()
{
	t_triangle t;

	t.p[0] = Initquater();
	t.p[1] = Initquater();
	t.p[2] = Initquater();
	t.clr = CLR_GRAY;
	return(t);
}

t_vector3 Vector_Mul_Vector(t_vector3 v1, t_vector3 v2)
{
	return ((t_vector3){ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z});
}

/*
t_quaternion vector3_sub(t_quaternion v1, t_quaternion v2)
{
	return ((t_quaternion){ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, 1});
}

t_quaternion vector3_mul(t_quaternion v1, float k)
{
	return ((t_quaternion){ v1.x * k, v1.y * k, v1.z * k , 1});
}

t_quaternion Vector_Mul_Vector(t_quaternion v1, t_quaternion v2)
{
	return ((t_quaternion){ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z , v1.w * v2.w});
}

t_quaternion Vector_Div(t_quaternion v1, float k)
{
	return ((t_quaternion){ v1.x / k, v1.y / k, v1.z / k, 1 });
}*/

float Vector_DotProduct(t_vector3 v1, t_vector3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float Vector_Length(t_vector3 v)
{
	return sqrtf(Vector_DotProduct(v, v));
}

t_vector3 Vector_Normalise(t_vector3 v)
{
	float l = Vector_Length(v);
	return((t_vector3){ v.x / l, v.y / l, v.z / l});
}

t_vector3 Vector_CrossProduct(t_vector3 v1, t_vector3 v2)
{
	t_vector3 v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

t_quaternion Vector_IntersectPlane(t_vector3 plane_p, t_vector3 plane_n, t_quaternion lineStart, t_quaternion lineEnd)
{
	plane_n = Vector_Normalise(plane_n);
	float plane_d = -Vector_DotProduct(plane_n, plane_p);
	float ad = Vector_DotProduct(lineStart.v, plane_n);
	float bd = Vector_DotProduct(lineEnd.v, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	t_vector3 lineStartToEnd = vector3_sub(lineEnd.v, lineStart.v);
	t_vector3 lineToIntersect = vector3_mul(lineStartToEnd, t);
	return ((t_quaternion){vector3_add(lineStart.v, lineToIntersect), 1});
}

t_vector3 Matrix_MultiplyVector(t_mat4x4 m, t_vector3 i)
{
	t_vector3 v;// = Initv3();
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + 1 * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + 1 * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + 1 * m.m[3][2];
	//v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}

t_mat4x4 Matrix_MakeIdentity()
{
	t_mat4x4 matrix = Init();
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

t_mat4x4 Matrix_MakeRotationX(float fAngleRad)
{
	t_mat4x4 matrix = Init();
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = cosf(fAngleRad);
	matrix.m[1][2] = sinf(fAngleRad);
	matrix.m[2][1] = -sinf(fAngleRad);
	matrix.m[2][2] = cosf(fAngleRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

t_mat4x4 Matrix_MakeRotationY(float fAngleRad)
{
	t_mat4x4 matrix = Init();
	matrix.m[0][0] = cosf(fAngleRad);
	matrix.m[0][2] = sinf(fAngleRad);
	matrix.m[2][0] = -sinf(fAngleRad);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosf(fAngleRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

t_mat4x4 Matrix_MakeRotationZ(float fAngleRad)
{
	t_mat4x4 matrix = Init();
	matrix.m[0][0] = cosf(fAngleRad);
	matrix.m[0][1] = sinf(fAngleRad);
	matrix.m[1][0] = -sinf(fAngleRad);
	matrix.m[1][1] = cosf(fAngleRad);
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

t_mat4x4 Matrix_MakeTranslation(float x, float y, float z)
{
	t_mat4x4 matrix = Init();
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	matrix.m[3][0] = x;
	matrix.m[3][1] = y;
	matrix.m[3][2] = z;
	return matrix;
}

t_mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
	float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
	t_mat4x4 matrix = Init();
	matrix.m[0][0] = fAspectRatio * fFovRad;
	matrix.m[1][1] = fFovRad;
	matrix.m[2][2] = fFar / (fFar - fNear);
	matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;
	return matrix;
}

t_mat4x4 Matrix_MultiplyMatrix(t_mat4x4 m1, t_mat4x4 m2)
{
	t_mat4x4 matrix = Init();
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	return matrix;
}

t_mat4x4 Matrix_PointAt(t_vector3 pos, t_vector3 target, t_vector3 up)
{
	// Calculate new forward direction
	t_vector3 newForward = vector3_sub(target, pos);
	newForward = Vector_Normalise(newForward);

	// Calculate new Up direction
	t_vector3 a = vector3_mul(newForward, Vector_DotProduct(up, newForward));
	t_vector3 newUp = vector3_sub(up, a);
	newUp = Vector_Normalise(newUp);

	// New Right direction is easy, its just cross product
	t_vector3 newRight = Vector_CrossProduct(newUp, newForward);

	// Construct Dimensioning and Translation Matrix	
	t_mat4x4 matrix = Init();
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;

}

t_mat4x4 Matrix_QuickInverse(t_mat4x4 m) // Only for Rotation/Translation Matrices
{
	t_mat4x4 matrix = Init();
	matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

// Return signed shortest distance from point to plane, plane normal must be normalised
float fdist(t_vector3 p, t_vector3 plane_n, t_vector3 plane_p) // rename
{
	t_vector3 n = Vector_Normalise(p);
	return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
}

int Triangle_ClipAgainstPlane(t_vector3 plane_p, t_vector3 plane_n, t_triangle *in_tri, t_triangle *out_tri1, t_triangle *out_tri2)
{
	// Make sure plane normal is indeed normal
	plane_n = Vector_Normalise(plane_n);

	// Create two temporary storage arrays to classify points either side of plane
	// If dist1ance sign is positive, point lies on "inside" of plane
	t_quaternion inside_points[3];  int nInsidePointCount = 0;
	t_quaternion outside_points[3]; int nOutsidePointCount = 0;
	
	in_tri->clr = CLR_GREEN; //debug colors
	out_tri1->clr = CLR_PRPL; //debug colors
	out_tri2->clr = CLR_TURQ; //debug colors
	for (int e = 0; e < 3; e++)
	{
		inside_points[e] = Initquater();
		outside_points[e] = Initquater();
	}
	// Get signed dist1ance of each point in triangle to plane
	float d0 = fdist(in_tri->p[0].v, plane_n, plane_p);
	float d1 = fdist(in_tri->p[1].v, plane_n, plane_p);
	float d2 = fdist(in_tri->p[2].v, plane_n, plane_p);

	if (d0 >= 0)
	{
		inside_points[nInsidePointCount++] = in_tri->p[0];
	}
	else
	{
		outside_points[nOutsidePointCount++] = in_tri->p[0];
	}
	if (d1 >= 0)
	{
		inside_points[nInsidePointCount++] = in_tri->p[1];
	}
	else
	{
		outside_points[nOutsidePointCount++] = in_tri->p[1];
	}
	if (d2 >= 0)
	{
		inside_points[nInsidePointCount++] = in_tri->p[2];
	}
	else
	{
		outside_points[nOutsidePointCount++] = in_tri->p[2];
	}

	// Now classify triangle points, and break the input triangle into 
	// smaller output triangles if required. There are four possible
	// outcomes...

	if (nInsidePointCount == 0)
	{
		// All points lie on the outside of plane, so clip whole triangle
		// It ceases to exist

		return 0; // No returned triangles are valid
	}

	if (nInsidePointCount == 3)
	{
		// All points lie on the inside of plane, so do nothing
		// and allow the triangle to simply pass through
		*out_tri1 = *in_tri;

		return 1; // Just the one returned original triangle is valid
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		// Triangle should be clipped. As two points lie outside
		// the plane, the triangle simply becomes a smaller triangle

		// Copy appearance info to new triangle
		//out_tri1.col =  in_tri.col;
		//out_tri1.sym = in_tri.sym;

		// The inside point is valid, so keep that...
		out_tri1->p[0] = inside_points[0];

		// but the two new points are at the locations where the 
		// original sides of the triangle (lines) intersect with the plane
		out_tri1->p[1] = Vector_IntersectPlane(plane_p, plane_n, inside_points[0], outside_points[0]);
		out_tri1->p[2] = Vector_IntersectPlane(plane_p, plane_n, inside_points[0], outside_points[1]);

		return 1; // Return the newly formed single triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		// Copy appearance info to new triangles
		//out_tri1.col =  in_tri.col;
		//out_tri1.sym = in_tri.sym;

		//out_tri2.col =  in_tri.col;
		//out_tri2.sym = in_tri.sym;

		// The first triangle consists of the two inside points and a new
		// point determined by the location where one side of the triangle
		// intersects with the plane
		out_tri1->p[0] = inside_points[0];
		out_tri1->p[1] = inside_points[1];
		out_tri1->p[2] = Vector_IntersectPlane(plane_p, plane_n, inside_points[0], outside_points[0]);

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		out_tri2->p[0] = inside_points[1];
		out_tri2->p[1] = out_tri1->p[2];
		out_tri2->p[2] = Vector_IntersectPlane(plane_p, plane_n, inside_points[1], outside_points[0]);

		return 2; // Return two newly formed triangles which form a quad
	}
	return(0);
}

static void	sort_tris(int tris[3][3])
{
	int	s_x;
	int	s_j;
	int	temp[3];

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (tris[s_j][Y] < tris[s_j + 1][Y])
			{
				ft_memcpy(temp, tris[s_j], sizeof(int) * 3);
				ft_memcpy(tris[s_j], tris[s_j + 1], sizeof(int) * 3);
				ft_memcpy(tris[s_j + 1], temp, sizeof(int) * 3);
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

static void	z_fill_sub_tri(int *tris[3], t_sdlcontext sdl, uint32_t clr)
{
	t_bresenham	b[2];

	populate_bresenham(&(b[0]), (t_point){tris[0][X], tris[0][Y]}, (t_point){tris[1][X], tris[1][Y]});
	populate_bresenham(&(b[1]), (t_point){tris[0][X], tris[0][Y]}, (t_point){tris[2][X], tris[2][Y]});
	while (b[0].local.y != tris[1][Y])
	{
		drawline((u_int32_t *)sdl.surface->pixels, b[0].local, b[1].local, clr);
		while (b[0].local.y == b[1].local.y)
			step_bresenham(&(b[0]));
		while (b[1].local.y != b[0].local.y)
			step_bresenham(&(b[1]));
	}
	drawline((u_int32_t *)sdl.surface->pixels, b[0].local, b[1].local, clr);
}

static void	z_fill_tri(int tris[3][3], t_sdlcontext sdl, uint32_t clr)
{
	int	split[3]; 	//Vector that's used to form the subtriangles.
	int	sorted[3][3];	//Stack memory for the sorted triangles
	float	lerp;

	sort_tris(ft_memcpy(sorted, tris, sizeof(int [3][3])));
	lerp = (float)(sorted[1][Y] - sorted[2][Y]) / (float)(sorted[0][Y] - sorted[2][Y]);
	split[X] = sorted[2][X] + (lerp * (sorted[0][X] - sorted[2][X]));
	split[Y] = sorted[1][Y];
	split[Z] = sorted[1][Z];
	z_fill_sub_tri((int *[3]){(int *)&(sorted[0]), (int *)&(sorted[1]), (int *)&split}, sdl, clr);
	z_fill_sub_tri((int *[3]){(int *)&(sorted[2]), (int *)&(sorted[1]), (int *)&split}, sdl, clr);
	drawline((u_int32_t *)sdl.surface->pixels, (t_point){sorted[0][X], sorted[0][Y]}, (t_point){sorted[2][X], sorted[2][Y]}, clr);
}

void move(t_game *game)
{
	t_math	*math;
	float	delta;
	t_quaternion temp = Initquater();;
	t_mat4x4 matcamerarot = Init();

	math = &game->math;
	matcamerarot = Matrix_MakeRotationY(math->fpitch);
	temp = MMatrix_MultiplyVector(matcamerarot, (t_quaternion){1, 0, 0, 1});
	math->vlookdir = temp.v;
	matcamerarot = Matrix_MakeRotationZ(math->fyaw);
	temp = MMatrix_MultiplyVector(matcamerarot, temp);
	math->vlookdir = temp.v;
	delta = game->clock.delta / 1000.0f;

	t_vector3 vforwad = vector3_mul(math->vlookdir, 8.0f);
	t_vector3 vstrafe;

	if ((game->keystate >> KEYS_UPMASK) & 1U)
		math->vcamera = vector3_add(math->vcamera, vforwad);
	if ((game->keystate >> KEYS_DOWNMASK) & 1U)
		math->vcamera = vector3_sub(math->vcamera, vforwad);
	if ((game->keystate >> KEYS_LEFTMASK) & 1U)
	{
		t_mat4x4 test2 = Init();

		test2 = Matrix_MakeRotationZ(RAD90);//dont know
		vstrafe = Matrix_MultiplyVector(test2, Vector_Mul_Vector(math->vlookdir, (t_vector3){1, 1, 0}));
		math->vcamera = vector3_add(math->vcamera, vstrafe);
	}
	if ((game->keystate >> KEYS_RIGHTMASK) & 1U)
	{
		t_mat4x4 test2 = Init();

		test2 = Matrix_MakeRotationZ(RAD90);
		vstrafe = Matrix_MultiplyVector(test2, Vector_Mul_Vector(math->vlookdir, (t_vector3){1, 1, 0}));
		math->vcamera = vector3_sub(math->vcamera, vstrafe);
	}
	math->fyaw = game->player.angle.x;
	math->fpitch = game->player.angle.y;
}

t_trilist *trilist_malloc(t_triangle triref)
{
	t_trilist *trilist;

	trilist = malloc(sizeof(t_trilist));
	trilist->tri = triref;
	trilist->next = NULL;
	return(trilist);
}

static t_trilist *trilist_push_back(t_trilist *head, t_triangle ref)
{
	t_trilist *prev;
	t_trilist *current;

	if (!head)
		return(trilist_malloc(ref));
	current = head;
	while (current)
	{
		prev = current;
		current = current->next;
	}
	prev->next = trilist_malloc(ref);
	return(head);
}

static int isvalidtri(t_triangle triprojected)
{
	int x = 0;
	int y = 0;
	for (int i = 0; i < 3; i++)
	{
		if (triprojected.p[i].v.x > 0 && triprojected.p[i].v.x < WINDOW_W)
			if (triprojected.p[i].v.y > 0 && triprojected.p[i].v.y < WINDOW_H)
				return(1);
	}
	return(0);
}

static void drawline1(uint32_t *pxls, int x0, int y0, int x1, int y1)
{
	int i;
    double x = x1 - x0; 
	double y = y1 - y0; 
	double length = sqrt( x*x + y*y ); 
	double addx = x / length; 
	double addy = y / length; 
	x = x0; 
	y = y0; 
	i = 0;
	while (i < length)
	{ 
		draw(pxls, (t_point){x, y}, INT_MAX); 
		x += addx; 
		y += addy; 
		i++;
	} 
}

static void draw_triangles(t_sdlcontext sdl, t_triangle *triangles, int index, int end)
{
	while (index < end)
	{
		z_fill_tri((int [3][3])
		{
		{triangles[index].p[0].v.x, triangles[index].p[0].v.y, triangles[index].p[0].v.z},
		{triangles[index].p[1].v.x, triangles[index].p[1].v.y, triangles[index].p[1].v.z},
		{triangles[index].p[2].v.x, triangles[index].p[2].v.y, triangles[index].p[2].v.z}},
		sdl, triangles[index].clr);
		drawline(sdl.surface->pixels, (t_point){triangles[index].p[0].v.x, triangles[index].p[0].v.y}, (t_point){triangles[index].p[1].v.x, triangles[index].p[1].v.y}, INT_MAX);
		drawline(sdl.surface->pixels, (t_point){triangles[index].p[1].v.x, triangles[index].p[1].v.y}, (t_point){triangles[index].p[2].v.x, triangles[index].p[2].v.y}, INT_MAX);
		drawline(sdl.surface->pixels, (t_point){triangles[index].p[2].v.x, triangles[index].p[2].v.y}, (t_point){triangles[index].p[0].v.x, triangles[index].p[0].v.y}, INT_MAX);
		index++;
	}
	if (0)
	z_fill_tri((int [3][3])
		{
		{0, 500, 0},
		{500, 500, 0},
		{0, 0, 0}},
		sdl, INT_MAX);
}

static void clipped(int count, t_triangle *triangles_calc, t_sdlcontext sdl)
{
	int i = 0;
	int start = 0;
	int end = 0;

	t_triangle triangles[200];


	while (i < count && 1)
	{
		t_triangle clipped[2];

		triangles[end++] = triangles_calc[i];
		int nnewtriangles = 1;
		for (int p = 0; p < 4; p++)
		{
			int ntristoadd = 0;
			while (nnewtriangles > 0)
			{
				clipped[0] = Inittri();
				clipped[1] = Inittri();
				t_triangle test = Inittri();
				test = triangles[start++];
				nnewtriangles--;
				switch (p)
				{
				case 0: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){0.0f, 1.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				case 1: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, (float)WINDOW_H - 1.0f, 0.0f}, (t_vector3){0.0f, -1.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				case 2: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){1.0f, 0.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				case 3: ntristoadd = Triangle_ClipAgainstPlane((t_vector3){(float)WINDOW_W - 1.0f, 0.0f, 0.0f}, (t_vector3){-1.0f, 0.0f, 0.0f}, &test, &clipped[0], &clipped[1]); break;
				}
				for (int w = 0; w < ntristoadd; w++)
				{
					triangles[end++] = clipped[w];
				}
			}
			nnewtriangles = end - start;
		}
		draw_triangles(sdl, triangles, start, end);
		start = 0;
		end = 0;
		i++;
	}
}

//TODO: just pass the triangle without i
static t_triangle transform_calc(t_mat4x4 matworld, t_triangle *triangles, int i)
{
	t_triangle tritransformed = Inittri();

	tritransformed.p[0] = MMatrix_MultiplyVector(matworld, triangles[i].p[0]);
	tritransformed.p[1] = MMatrix_MultiplyVector(matworld, triangles[i].p[1]);
	tritransformed.p[2] = MMatrix_MultiplyVector(matworld, triangles[i].p[2]);

	return(tritransformed);
}

static t_vector3 normal_calc(t_triangle tritransformed)
{
	t_vector3 normal, line1, line2;

	line1 = vector3_sub(tritransformed.p[1].v, tritransformed.p[0].v);
	line2 = vector3_sub(tritransformed.p[2].v, tritransformed.p[0].v);
	normal = Vector_CrossProduct(line1, line2);
	normal = Vector_Normalise(normal);

	return (normal);
}

static int clippedtriangles(t_triangle tritransformed, t_mat4x4 matview, t_triangle *clipped)
{
	t_triangle triviewed = Inittri();

	triviewed.p[0] = MMatrix_MultiplyVector(matview, tritransformed.p[0]);
	triviewed.p[1] = MMatrix_MultiplyVector(matview, tritransformed.p[1]);
	triviewed.p[2] = MMatrix_MultiplyVector(matview, tritransformed.p[2]);
	return (Triangle_ClipAgainstPlane(
	(t_vector3){0.0f, 0.0f, 0.1f},
	(t_vector3){0.0f, 0.0f, 1.0f},
	&triviewed, &(clipped[0]), &(clipped[1])));
}

static t_triangle triangle_to_screenspace(t_mat4x4 matproj, t_triangle clipped)
{
	t_triangle triprojected = Inittri();

	triprojected.p[0] = MMatrix_MultiplyVector(matproj, clipped.p[0]);
	triprojected.p[1] = MMatrix_MultiplyVector(matproj, clipped.p[1]);
	triprojected.p[2] = MMatrix_MultiplyVector(matproj, clipped.p[2]);
	
	triprojected.p[0].v = vector3_div(triprojected.p[0].v, triprojected.p[0].w);
	triprojected.p[1].v = vector3_div(triprojected.p[1].v, triprojected.p[1].w);
	triprojected.p[2].v = vector3_div(triprojected.p[2].v, triprojected.p[2].w);

	triprojected.p[0].v = vector3_negative(triprojected.p[0].v);
	triprojected.p[1].v = vector3_negative(triprojected.p[1].v);
	triprojected.p[2].v = vector3_negative(triprojected.p[2].v);
	/*
	triprojected.p[0].v.x *= -1.0f;
	triprojected.p[0].v.y *= -1.0f;
	triprojected.p[1].v.x *= -1.0f;
	triprojected.p[1].v.y *= -1.0f;
	triprojected.p[2].v.x *= -1.0f;
	triprojected.p[2].v.y *= -1.0f;
*/
	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	triprojected.p[0].v = vector3_add(triprojected.p[0].v, voffsetview);
	triprojected.p[1].v = vector3_add(triprojected.p[1].v, voffsetview);
	triprojected.p[2].v = vector3_add(triprojected.p[2].v, voffsetview);

	triprojected.p[0].v.x *= 0.5f * (float)SW;
	triprojected.p[0].v.y *= 0.5f * (float)SH;
	triprojected.p[1].v.x *= 0.5f * (float)SW;
	triprojected.p[1].v.y *= 0.5f * (float)SH;
	triprojected.p[2].v.x *= 0.5f * (float)SW;
	triprojected.p[2].v.y *= 0.5f * (float)SH;

	return(triprojected);
}

void engine3d(t_sdlcontext sdl, t_triangle *triangles, int tri_count, t_mat4x4 matproj, t_mat4x4 matworld, t_vector3 vcamera, t_vector3 vlookdir)
{
	t_triangle	triangles_calc[200];
	int			i;
	int			count = 0;
	t_vector3 vtarget;

	vtarget = vector3_add(vcamera, vlookdir);
	t_mat4x4 matcamera = Matrix_PointAt(vcamera, vtarget, (t_vector3){0, 0, 1});
	t_mat4x4 matview = Matrix_QuickInverse(matcamera);
	i = 0;
	while(i < tri_count)
	{
		t_triangle tritransformed = Inittri();
		t_vector3 normal;
		t_vector3 vcameraray;

		tritransformed = transform_calc(matworld, triangles, i);
		normal = normal_calc(tritransformed);
		vcameraray = vector3_sub(tritransformed.p[0].v, vcamera);
		if (Vector_DotProduct(normal, vcameraray) < 0.0f)
		{
			t_triangle clipped[2];
			int nclippedtriangles = clippedtriangles(tritransformed, matview, &clipped[0]);
			for (int n = 0; n < nclippedtriangles; n++)
			{
				triangles_calc[count++] = triangle_to_screenspace(matproj, clipped[n]);
			}
		}
		i++;
	}
	clipped(count, triangles_calc, sdl);
}
