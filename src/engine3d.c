/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine3d.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 17:10:21 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/06 17:36:01 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

typedef struct s_mat4x4
{
	float m[4][4];
}	t_mat4x4;

t_mat4x4 matproj;
t_vec3d vcamera;
t_vec3d vlookdir;
float	fyaw;
float	ftheta;

t_vec3d Vector_Add(t_vec3d v1, t_vec3d v2)
{
	return ((t_vec3d){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z});
}

t_vec3d Vector_Sub(t_vec3d v1, t_vec3d v2)
{
	return ((t_vec3d){ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z });
}

t_vec3d Vector_Mul(t_vec3d v1, float k)
{
	return ((t_vec3d){ v1.x * k, v1.y * k, v1.z * k });
}

t_vec3d Vector_Div(t_vec3d v1, float k)
{
	return ((t_vec3d){ v1.x / k, v1.y / k, v1.z / k });
}

float Vector_DotProduct(t_vec3d v1, t_vec3d v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
}

float Vector_Length(t_vec3d v)
{
	return sqrtf(Vector_DotProduct(v, v));
}

t_vec3d Vector_Normalise(t_vec3d v)
{
	float l = Vector_Length(v);
	return((t_vec3d){ v.x / l, v.y / l, v.z / l });
}

t_vec3d Vector_CrossProduct(t_vec3d v1, t_vec3d v2)
{
	t_vec3d v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

t_vec3d Vector_IntersectPlane(t_vec3d plane_p, t_vec3d plane_n, t_vec3d lineStart, t_vec3d lineEnd)
{
	plane_n = Vector_Normalise(plane_n);
	float plane_d = -Vector_DotProduct(plane_n, plane_p);
	float ad = Vector_DotProduct(lineStart, plane_n);
	float bd = Vector_DotProduct(lineEnd, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	t_vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
	t_vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);
	return Vector_Add(lineStart, lineToIntersect);
}

t_vec3d Matrix_MultiplyVector(t_mat4x4 m, t_vec3d i)
{
	t_vec3d v;
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}

t_mat4x4 Matrix_MakeIdentity()
{
	t_mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

t_mat4x4 Matrix_MakeRotationX(float fAngleRad)
{
	t_mat4x4 matrix;
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
	t_mat4x4 matrix;
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
	t_mat4x4 matrix;
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
	t_mat4x4 matrix;
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
	t_mat4x4 matrix;
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
	t_mat4x4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	return matrix;
}

t_mat4x4 Matrix_PointAt(t_vec3d pos, t_vec3d target, t_vec3d up)
{
	// Calculate new forward direction
	t_vec3d newForward = Vector_Sub(target, pos);
	newForward = Vector_Normalise(newForward);

	// Calculate new Up direction
	t_vec3d a = Vector_Mul(newForward, Vector_DotProduct(up, newForward));
	t_vec3d newUp = Vector_Sub(up, a);
	newUp = Vector_Normalise(newUp);

	// New Right direction is easy, its just cross product
	t_vec3d newRight = Vector_CrossProduct(newUp, newForward);

	// Construct Dimensioning and Translation Matrix	
	t_mat4x4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;

}

t_mat4x4 Matrix_QuickInverse(t_mat4x4 m) // Only for Rotation/Translation Matrices
{
	t_mat4x4 matrix;
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
float dist1(t_vec3d p, t_vec3d plane_n, t_vec3d plane_p)
{
	t_vec3d n = Vector_Normalise(p);
	return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
}

int Triangle_ClipAgainstPlane(t_vec3d plane_p, t_vec3d plane_n, t_triangle *in_tri, t_triangle *out_tri1, t_triangle *out_tri2)
{
	// Make sure plane normal is indeed normal
	plane_n = Vector_Normalise(plane_n);


	// Create two temporary storage arrays to classify points either side of plane
	// If dist1ance sign is positive, point lies on "inside" of plane
	t_vec3d inside_points[3];  int nInsidePointCount = 0;
	t_vec3d outside_points[3]; int nOutsidePointCount = 0;

	// Get signed dist1ance of each point in triangle to plane
	float d0 = dist1(in_tri->p[0], plane_n, plane_p);
	float d1 = dist1(in_tri->p[1], plane_n, plane_p);
	float d2 = dist1(in_tri->p[2], plane_n, plane_p);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = in_tri->p[0]; }
	else { outside_points[nOutsidePointCount++] = in_tri->p[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = in_tri->p[1]; }
	else { outside_points[nOutsidePointCount++] = in_tri->p[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = in_tri->p[2]; }
	else { outside_points[nOutsidePointCount++] = in_tri->p[2]; }

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
		out_tri1 = in_tri;

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
}

static void swap(t_triangle *t1, t_triangle *t2)
{
	t_triangle *temp;
	temp = t1;
	t1 = t2;
	t2 = temp;
}

//(t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
static int partition(t_triangle *triangles, int low, int high)
{
	int pivot = (triangles[high].p[0].z + triangles[high].p[1].z + triangles[high].p[2].z) / 3.0;
	int i = (low -1);

	for (int j = low; j < high - 1; j++)
	{
		if (((triangles[j].p[0].z + triangles[j].p[1].z + triangles[j].p[2].z) / 3.0) < pivot)
		{
			i++;
			swap(&triangles[i], &triangles[j]);
		}
	}
	swap(&triangles[i + 1], &triangles[high]);
	return(i + 1);
}

static t_triangle	*sort_triangles(t_triangle *triangles, int low, int high)
{
	if (low < high)
	{
		int pi = partition(triangles, low, high);

		sort_triangles(triangles, low, pi -1);
		sort_triangles(triangles, pi + 1, high);
	}
	return(triangles);
}

typedef struct s_bresenham
{
	int	diff[3];
	int	add[3];
	int	local[3];
	int	error;
}	t_bresenham;

static void	step_bresenham_x(t_bresenham *b, int target[3])
{
	if (b->error * 2 < b->diff[X] && b->local[Y] != target[Y])
	{
		b->local[Y] += b->add[Y];
		b->error += b->diff[X];
	}
}

static void	step_bresenham_y(t_bresenham *b, int target[3])
{
	if (b->error * 2 > b->diff[Y] && b->local[X] != target[X])
	{
		b->local[X] += b->add[X];
		b->error += b->diff[Y];
	}
}

static void	step_bresenham(t_bresenham *b, int target[3])
{
	step_bresenham_x(b, target);
	step_bresenham_y(b, target);
}

static void	populate_bresenham(t_bresenham *b, int *from, int *to)
{
	ft_memcpy(b->local, from, sizeof(int) * 3);
	b->diff[X] = ft_abs(b->local[X] - to[X]);
	b->diff[Y] = -ft_abs(b->local[Y] - to[Y]);
	b->add[X] = 1 - ((b->local[X] > to[X]) * 2);
	b->add[Y] = 1 - ((b->local[Y] > to[Y]) * 2);
	b->error = b->diff[X] + b->diff[Y];
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

static void	z_fill_sub_tri(int *tris[3], t_sdlcontext sdl)
{
	t_bresenham	b[2];

	populate_bresenham(&(b[0]), tris[0], tris[1]);
	populate_bresenham(&(b[1]), tris[0], tris[2]);
	while (b[0].local[Y] != tris[1][Y])
	{
		drawline((u_int32_t *)sdl.surface->pixels, b[0].local, b[1].local, CLR_GRAY);
		while (b[0].local[Y] == b[1].local[Y])
			step_bresenham(&(b[0]), tris[1]);
		while (b[1].local[Y] != b[0].local[Y])
			step_bresenham(&(b[1]), tris[2]);
	}
	drawline((u_int32_t *)sdl.surface->pixels, b[0].local, b[1].local, CLR_GRAY);
}

static void	z_fill_tri(int tris[3][3], t_sdlcontext sdl)
{
	int	split[3]; 	//Vector that's used to form the subtriangles.
	int	sorted[3][3];	//Stack memory for the sorted triangles
	float	lerp;

	sort_tris(ft_memcpy(sorted, tris, sizeof(int [3][3])));
	lerp = (float)(sorted[1][Y] - sorted[2][Y]) / (float)(sorted[0][Y] - sorted[2][Y]);
	split[X] = sorted[2][X] + (lerp * (sorted[0][X] - sorted[2][X]));
	split[Y] = sorted[1][Y];
	split[Z] = sorted[1][Z];
	z_fill_sub_tri((int *[3]){(int *)&(sorted[0]), (int *)&(sorted[1]), (int *)&split}, sdl);
	z_fill_sub_tri((int *[3]){(int *)&(sorted[2]), (int *)&(sorted[1]), (int *)&split}, sdl);
	drawline((u_int32_t *)sdl.surface->pixels, sorted[0], sorted[2], CLR_GRAY);
	//z_draw_line(sorted[0], sorted[2], sdl);
}

void engine3d(t_sdlcontext sdl, t_game *game)
{
	t_triangle	*triangles_calc;
	t_triangle	*triangles;
	int			i;
	int			count = 0;

	triangles_calc = malloc(sizeof(t_triangle) * game->tri_count);
	triangles = game->triangles;
	t_mat4x4	matrotz, matrotx;
	matrotz = Matrix_MakeRotationZ(0);
	matrotx = Matrix_MakeRotationX(0);

	t_mat4x4 mattrans;
	mattrans = Matrix_MakeTranslation(0.0, 0.0, 5.0);

	t_mat4x4 matworld;
	matworld = Matrix_MakeIdentity();
	matworld = Matrix_MultiplyMatrix(matrotz, matrotx);
	matworld = Matrix_MultiplyMatrix(matworld, mattrans);

	t_vec3d vup = (t_vec3d){0, 1, 0};
	t_vec3d vtarget = (t_vec3d){0, 0, 1};
	t_mat4x4 matcamerarot = Matrix_MakeRotationY(fyaw);
	vlookdir = Matrix_MultiplyVector(matcamerarot, vtarget);
	vtarget = Vector_Add(vcamera, vlookdir);
	t_mat4x4 matcamera = Matrix_PointAt(vcamera, vtarget, vup);

	t_mat4x4 matview = Matrix_QuickInverse(matcamera);

	i = 0;
	while(i < game->tri_count)
	{
		t_triangle triprojected, tritransformed, triviewed;

		tritransformed.p[0] = Matrix_MultiplyVector(matworld, triangles[i].p[0]);
		tritransformed.p[1] = Matrix_MultiplyVector(matworld, triangles[i].p[1]);
		tritransformed.p[2] = Matrix_MultiplyVector(matworld, triangles[i].p[2]);

		t_vec3d normal, line1, line2;

		line1 = Vector_Sub(tritransformed.p[1], tritransformed.p[0]);
		line2 = Vector_Sub(tritransformed.p[2], tritransformed.p[0]);

		normal = Vector_CrossProduct(line1, line2);

		normal = Vector_Normalise(normal);

		t_vec3d vcameraray = Vector_Sub(tritransformed.p[0], vcamera);

		if (Vector_DotProduct(normal, vcameraray) < 0.0)
		{
			t_vec3d light_direction = (t_vec3d){0.0, 1.0, -1.0};
			light_direction = Vector_Normalise(light_direction);

			float dp;
			if (0.1 > Vector_DotProduct(light_direction, normal))
				dp = 0.1;
			else
				dp = Vector_DotProduct(light_direction, normal);

			triviewed.p[0] = Matrix_MultiplyVector(matview, tritransformed.p[0]);
			triviewed.p[1] = Matrix_MultiplyVector(matview, tritransformed.p[1]);
			triviewed.p[2] = Matrix_MultiplyVector(matview, tritransformed.p[2]);
			
			int nclippedtriangles = 0;
			t_triangle clipped[2];
			nclippedtriangles = Triangle_ClipAgainstPlane(
				(t_vec3d){0.0, 0.0, 0.1},
				(t_vec3d){0.0, 0.0, 1.0},
				&triviewed, &clipped[0], &clipped[1]);
			for (int n = 0; n < nclippedtriangles; n++)
			{
				triprojected.p[0] = Matrix_MultiplyVector(matproj, clipped[n].p[0]);
				triprojected.p[1] = Matrix_MultiplyVector(matproj, clipped[n].p[1]);
				triprojected.p[2] = Matrix_MultiplyVector(matproj, clipped[n].p[2]);
			
				triprojected.p[0] = Vector_Div(triprojected.p[0], triprojected.p[0].w);
				triprojected.p[1] = Vector_Div(triprojected.p[1], triprojected.p[1].w);
				triprojected.p[2] = Vector_Div(triprojected.p[2], triprojected.p[2].w);

				triprojected.p[0].x *= -1.0;
				triprojected.p[0].y *= -1.0;
				triprojected.p[1].x *= -1.0;
				triprojected.p[1].y *= -1.0;
				triprojected.p[2].x *= -1.0;
				triprojected.p[2].y *= -1.0;

				t_vec3d voffsetview = (t_vec3d){1, 1, 0};
				triprojected.p[0] = Vector_Add(triprojected.p[0], voffsetview);
				triprojected.p[1] = Vector_Add(triprojected.p[1], voffsetview);
				triprojected.p[2] = Vector_Add(triprojected.p[2], voffsetview);
				triprojected.p[0].x *= 0.5 * (float)WINDOW_W;
				triprojected.p[0].y *= 0.5 * (float)WINDOW_H;
				triprojected.p[1].x *= 0.5 * (float)WINDOW_W;
				triprojected.p[1].y *= 0.5 * (float)WINDOW_H;
				triprojected.p[2].x *= 0.5 * (float)WINDOW_W;
				triprojected.p[2].y *= 0.5 * (float)WINDOW_H;
				triangles_calc[count++] = triprojected;
			}
		}
		i++;
	}
	sort_triangles(triangles_calc, 0, count - 1);
	/*int nnewtriangles = 1;
	i = 0;
	while (i < count)
	{
		t_triangle clipped[2];

		int ntristoadd = 0;
		while (nnewtriangles > 0)
		{
			t_triangle test = 
		}
		
	}*/
	i = 0;
	while (i < count)
	{
		z_fill_tri((int [3][3])
		{
		{triangles_calc[i].p[0].x, triangles_calc[i].p[0].y, triangles_calc[i].p[0].z},
		{triangles_calc[i].p[1].x, triangles_calc[i].p[1].y, triangles_calc[i].p[1].z},
		{triangles_calc[i].p[2].x, triangles_calc[i].p[2].y, triangles_calc[i].p[2].z}},
		sdl);
		i++;
	printf("loop");
	}
	free(triangles_calc);
}