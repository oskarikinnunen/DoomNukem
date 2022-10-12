/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_3d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/12 12:59:11 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"

#define PRINT_DEBUG 0

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

t_vec3d	Initv3()
{
	t_vec3d	v3;
	v3.w = 1.0f;
	v3.x = 0.0f;
	v3.y = 0.0f;
	v3.z = 0.0f;
	return(v3);
}

t_triangle Inittri()
{
	t_triangle t;
	t.p[0] = Initv3();
	t.p[1] = Initv3();
	t.p[2] = Initv3();
	t.clr = CLR_GRAY;
	return(t);
}

t_vec3d Vector_Add(t_vec3d v1, t_vec3d v2)
{
	return ((t_vec3d){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, 1});
}

t_vec3d Vector_Sub(t_vec3d v1, t_vec3d v2)
{
	return ((t_vec3d){ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, 1});
}

t_vec3d Vector_Mul(t_vec3d v1, float k)
{
	return ((t_vec3d){ v1.x * k, v1.y * k, v1.z * k , 1});
}

t_vec3d Vector_Div(t_vec3d v1, float k)
{
	return ((t_vec3d){ v1.x / k, v1.y / k, v1.z / k, 1 });
}

float Vector_DotProduct(t_vec3d v1, t_vec3d v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float Vector_Length(t_vec3d v)
{
	return sqrtf(Vector_DotProduct(v, v));
}

t_vec3d Vector_Normalise(t_vec3d v)
{
	float l = Vector_Length(v);
	return((t_vec3d){ v.x / l, v.y / l, v.z / l , 1});
}

t_vec3d Vector_CrossProduct(t_vec3d v1, t_vec3d v2)
{
	t_vec3d v = Initv3();
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
	return (Vector_Add(lineStart, lineToIntersect));
}

t_vec3d Matrix_MultiplyVector(t_mat4x4 m, t_vec3d i)
{
	t_vec3d v = Initv3();
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
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
float dist1(t_vec3d p, t_vec3d plane_n, t_vec3d plane_p) // rename
{
	t_vec3d n = Vector_Normalise(p);
	return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
}

static void printf_matrix(t_mat4x4 m)
{
	if (PRINT_DEBUG == 0)
		return;
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

static void printf_v3(t_vec3d v3)
{
	if (PRINT_DEBUG == 0)
		return;
	printf("vec3d x is %f, y is %f, z is %f, w is %f\n", v3.x, v3.y, v3.z, v3.w);
}

void printf_tri(t_triangle tri)
{
	if (PRINT_DEBUG == 0)
		return;
	printf("triangle start\n");
	printf_v3(tri.p[0]);
	printf_v3(tri.p[1]);
	printf_v3(tri.p[2]);
	printf("triangle ends\n");
}

int Triangle_ClipAgainstPlane(t_vec3d plane_p, t_vec3d plane_n, t_triangle *in_tri, t_triangle *out_tri1, t_triangle *out_tri2)
{
	// Make sure plane normal is indeed normal
	plane_n = Vector_Normalise(plane_n);

	// Create two temporary storage arrays to classify points either side of plane
	// If dist1ance sign is positive, point lies on "inside" of plane
	t_vec3d inside_points[3];  int nInsidePointCount = 0;
	t_vec3d outside_points[3]; int nOutsidePointCount = 0;

	for (int e = 0; e < 3; e++)
	{
		inside_points[e] = Initv3();
		outside_points[e] = Initv3();
	}
	// Get signed dist1ance of each point in triangle to plane
	float d0 = dist1(in_tri->p[0], plane_n, plane_p);
	float d1 = dist1(in_tri->p[1], plane_n, plane_p);
	float d2 = dist1(in_tri->p[2], plane_n, plane_p);

	//printf(" d0, %f, d1 %f, d2 %f\n", d0, d1, d2);
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
		out_tri1->clr = CLR_PRPL;

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		out_tri2->p[0] = inside_points[1];
		out_tri2->p[1] = out_tri1->p[2];
		out_tri2->p[2] = Vector_IntersectPlane(plane_p, plane_n, inside_points[1], outside_points[0]);
		out_tri2->clr = CLR_TURQ;

		return 2; // Return two newly formed triangles which form a quad
	}
	return(0);
}

static void swap(t_triangle *t1, t_triangle *t2)
{
	t_triangle *temp;
	temp = t1;
	t1 = t2;
	t2 = temp;
}

static int partition(t_triangle *triangles, int low, int high)
{
	float pivot = (triangles[high].p[0].z + triangles[high].p[1].z + triangles[high].p[2].z) / 3.0f;
	int i = (low -1);

	for (int j = low; j < high - 1; j++)
	{
		if (((triangles[j].p[0].z + triangles[j].p[1].z + triangles[j].p[2].z) / 3.0f) < pivot)
		{
			i++;
			swap(&triangles[i], &triangles[j]);
		}
	}
	swap(&triangles[i + 1], &triangles[high]);
	return(i + 1);
}

static void sort_triangles(t_triangle *triangles, int low, int high)
{
	if (low < high)
	{
		int pi = partition(triangles, low, high);

		sort_triangles(triangles, low, pi -1);
		sort_triangles(triangles, pi + 1, high);
	}
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

	populate_bresenham(&(b[0]), tris[0], tris[1]);
	populate_bresenham(&(b[1]), tris[0], tris[2]);
	while (b[0].local[Y] != tris[1][Y])
	{
		drawline((u_int32_t *)sdl.surface->pixels, b[0].local, b[1].local, clr);
		while (b[0].local[Y] == b[1].local[Y])
			step_bresenham(&(b[0]));
		while (b[1].local[Y] != b[0].local[Y])
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
	drawline((u_int32_t *)sdl.surface->pixels, sorted[0], sorted[2], clr);
	//z_draw_line(sorted[0], sorted[2], sdl);
}

static void move(t_game *game)
{
	t_math	*math;
	float	delta;

	delta = game->clock.delta / 1000.0f;
	math = &game->math;
	t_vec3d vforwad = Vector_Mul(math->vlookdir, 8.0f);

	if ((game->keystate >> KEYS_UPMASK) & 1U)
		math->vcamera = Vector_Add(math->vcamera, vforwad);
	if ((game->keystate >> KEYS_DOWNMASK) & 1U)
		math->vcamera = Vector_Sub(math->vcamera, vforwad);

	if ((game->keystate >> KEYS_ARROWRIGHT) & 1U)
		math->fyaw += 2.0f * delta;
	if ((game->keystate >> KEYS_ARROWLEFT) & 1U)
		math->fyaw -= 2.0f * delta;
	if ((game->keystate >> KEYS_ARROWUP) & 1U)
		math->fpitch += 2.0f * delta;
	if ((game->keystate >> KEYS_ARROWDOWN) & 1U)
		math->fpitch -= 2.0f * delta;

	//math->fyaw = game->player.angle[X];
	//math->fpitch = game->player.angle[Y];
	printf("yaw %f pitch %f\n", math->fyaw, math->fpitch);
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

static void clipped(int count, t_triangle *triangles_calc, t_game *game, t_sdlcontext sdl)
{
	int i = 0;
	int counter = 0;
	while (i < count && 1 == 1)
	{
		t_triangle clipped[2];
		t_trilist	*trilist;
		t_trilist	*head;

	/*	if ((triangles_calc[i].p->x > WINDOW_W - 1|| triangles_calc[i].p->x < 0) && (triangles_calc[i].p->y > WINDOW_H - 1 || triangles_calc[i].p->y < 0))
		{
			i++;
			continue;
		}*/
		trilist = trilist_malloc(triangles_calc[i]);
		if (!trilist)
			exit(0);
		head = trilist;
		int nnewtriangles = 1;
		int front = 0;
		for (int p = 0; p < 4; p++)
		{
			t_trilist *lenlist;
			int ntristtoadd = 0;
			while (trilist)
			{
				clipped[0] = Inittri();
				clipped[1] = Inittri();
				t_triangle test = Inittri();
				trilist = head;
				test = trilist->tri;
				nnewtriangles--;
				switch (p)
				{
				case 0: ntristtoadd = Triangle_ClipAgainstPlane((t_vec3d){0.0f, 0.0f, 0.0f, 1.0f}, (t_vec3d){0.0f, 1.0f, 0.0f, 1.0f}, &test, &clipped[0], &clipped[1]); break;
				case 1: ntristtoadd = Triangle_ClipAgainstPlane((t_vec3d){0.0f, (float)WINDOW_H - 1.0f, 0.0f, 1.0f}, (t_vec3d){0.0f, -1.0f, 0.0f, 1.0f}, &test, &clipped[0], &clipped[1]); break;
				case 2: ntristtoadd = Triangle_ClipAgainstPlane((t_vec3d){0.0f, 0.0f, 0.0f, 1.0f}, (t_vec3d){1.0f, 0.0f, 0.0f, 1.0f}, &test, &clipped[0], &clipped[1]); break;
				case 3: ntristtoadd = Triangle_ClipAgainstPlane((t_vec3d){(float)WINDOW_W - 1.0f, 0.0f, 0.0f, 1.0f}, (t_vec3d){-1.0f, 0.0f, 0.0f, 1.0f}, &test, &clipped[0], &clipped[1]); break;
				}
				for (int w = 0; w < ntristtoadd; w++)
				{
					head = trilist_push_back(head, clipped[w]);
					trilist = trilist->next;
				}
				trilist = trilist->next;
			}
		}
		trilist = head;
		while (trilist)
		{
			t_trilist *temp;
			z_fill_tri((int [3][3])
			{
			{trilist->tri.p[0].x, trilist->tri.p[0].y, trilist->tri.p[0].z},
			{trilist->tri.p[1].x, trilist->tri.p[1].y, trilist->tri.p[1].z},
			{trilist->tri.p[2].x, trilist->tri.p[2].y, trilist->tri.p[2].z}},
			sdl, trilist->tri.clr);
			drawline(sdl.surface->pixels, (int [2]){trilist->tri.p[0].x, trilist->tri.p[0].y}, (int [2]){trilist->tri.p[1].x, trilist->tri.p[1].y}, CLR_PRPL);
			drawline(sdl.surface->pixels, (int [2]){trilist->tri.p[1].x, trilist->tri.p[1].y}, (int [2]){trilist->tri.p[2].x, trilist->tri.p[2].y}, CLR_PRPL);
			drawline(sdl.surface->pixels, (int [2]){trilist->tri.p[2].x, trilist->tri.p[2].y}, (int [2]){trilist->tri.p[0].x, trilist->tri.p[0].y}, CLR_PRPL);
			temp = trilist;
			trilist = trilist->next;
			free(temp);
			counter++;
		}
		i++;
	}
	printf("counter is %d\n", counter);
	printf("fps is %f\n", 1000.0f / game->clock.delta);
	printf("loopcomplete\n\n");
	free(triangles_calc);
}

void engine3d(t_sdlcontext sdl, t_game *game)
{
	move(game);
	t_triangle	*triangles_calc;
	t_triangle	*triangles;
	t_math		*math;
	int			i;
	int			count = 0;

	math = &game->math;
	math->matproj = Init();
	math->matproj = Matrix_MakeProjection(90.0f, (float)WINDOW_H / (float)WINDOW_W, 2.0f, 1000.0f);
	triangles_calc = malloc(sizeof(t_triangle) * 200);
	triangles = math->triangles;

	t_mat4x4 matrotz = Init();
	t_mat4x4 matrotx = Init();
	matrotz = Matrix_MakeRotationZ(0);
	matrotx = Matrix_MakeRotationX(1.57079633f * 3);

	t_mat4x4 mattrans = Init();
	mattrans = Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);

	t_mat4x4 matworld = Init();
	matworld = Matrix_MakeIdentity();
	matworld = Matrix_MultiplyMatrix(matrotz, matrotx);
	matworld = Matrix_MultiplyMatrix(matworld, mattrans);

	t_vec3d vup = (t_vec3d){0, 1, 0, 1};
	t_vec3d vtarget = (t_vec3d){0, 0, 1, 1};
	t_mat4x4 matcamerarot = Init();
	matcamerarot = Matrix_MakeRotationY(math->fyaw);
	math->vlookdir = Matrix_MultiplyVector(matcamerarot, vtarget);

	vtarget = (t_vec3d){0, 1, 0, 1};
	matcamerarot = Init();
	matcamerarot = Matrix_MakeRotationX(math->fpitch);
	math->vlookdir = Vector_Add(math->vlookdir, Matrix_MultiplyVector(matcamerarot, vtarget));

	vtarget = Vector_Add(math->vcamera, math->vlookdir);

	t_mat4x4 matcamera = Matrix_PointAt(math->vcamera, vtarget, vup);
	t_mat4x4 matview = Matrix_QuickInverse(matcamera);
	i = 0;
	while(i < math->tri_count)
	{
		t_triangle triprojected, tritransformed, triviewed;

		triprojected = Inittri();
		tritransformed = Inittri();
		triviewed = Inittri();
		tritransformed.p[0] = Matrix_MultiplyVector(matworld, triangles[i].p[0]);
		tritransformed.p[1] = Matrix_MultiplyVector(matworld, triangles[i].p[1]);
		tritransformed.p[2] = Matrix_MultiplyVector(matworld, triangles[i].p[2]);

		t_vec3d normal, line1, line2;

		line1 = Vector_Sub(tritransformed.p[1], tritransformed.p[0]);
		line2 = Vector_Sub(tritransformed.p[2], tritransformed.p[0]);
		normal = Vector_CrossProduct(line1, line2);
		normal = Vector_Normalise(normal);
		t_vec3d vcameraray = Vector_Sub(tritransformed.p[0], math->vcamera);
		if (Vector_DotProduct(normal, vcameraray) < 0.0f || 1 == 0)
		{
			t_vec3d light_direction = (t_vec3d){0.0f, 1.0f, -1.0f, 1};
			light_direction = Vector_Normalise(light_direction);

			float dp;
			if (0.1f > Vector_DotProduct(light_direction, normal))
				dp = 0.1f;
			else
				dp = Vector_DotProduct(light_direction, normal);

			triviewed.p[0] = Matrix_MultiplyVector(matview, tritransformed.p[0]);
			triviewed.p[1] = Matrix_MultiplyVector(matview, tritransformed.p[1]);
			triviewed.p[2] = Matrix_MultiplyVector(matview, tritransformed.p[2]);
			
			int nclippedtriangles = 0;
			t_triangle clipped[2];
			clipped[0] = Inittri();
			clipped[1] = Inittri();
			nclippedtriangles = Triangle_ClipAgainstPlane(
				(t_vec3d){0.0f, 0.0f, 10.1f, 1},
				(t_vec3d){0.0f, 0.0f, 1.0f, 1},
				&triviewed, &clipped[0], &clipped[1]);
			for (int n = 0; n < nclippedtriangles; n++)
			{
				triprojected.p[0] = Matrix_MultiplyVector(math->matproj, clipped[n].p[0]);
				triprojected.p[1] = Matrix_MultiplyVector(math->matproj, clipped[n].p[1]);
				triprojected.p[2] = Matrix_MultiplyVector(math->matproj, clipped[n].p[2]);
			
				triprojected.p[0] = Vector_Div(triprojected.p[0], triprojected.p[0].w);
				triprojected.p[1] = Vector_Div(triprojected.p[1], triprojected.p[1].w);
				triprojected.p[2] = Vector_Div(triprojected.p[2], triprojected.p[2].w);

				triprojected.p[0].x *= -1.0f;
				triprojected.p[0].y *= -1.0f;
				triprojected.p[1].x *= -1.0f;
				triprojected.p[1].y *= -1.0f;
				triprojected.p[2].x *= -1.0f;
				triprojected.p[2].y *= -1.0f;

				t_vec3d voffsetview = (t_vec3d){1.0f, 1.0f, 0.0f, 1.0f};
				triprojected.p[0] = Vector_Add(triprojected.p[0], voffsetview);
				triprojected.p[1] = Vector_Add(triprojected.p[1], voffsetview);
				triprojected.p[2] = Vector_Add(triprojected.p[2], voffsetview);

				triprojected.p[0].x *= 0.5f * (float)WINDOW_W;
				triprojected.p[0].y *= 0.5f * (float)WINDOW_H;
				triprojected.p[1].x *= 0.5f * (float)WINDOW_W;
				triprojected.p[1].y *= 0.5f * (float)WINDOW_H;
				triprojected.p[2].x *= 0.5f * (float)WINDOW_W;
				triprojected.p[2].y *= 0.5f * (float)WINDOW_H;
				triangles_calc[count++] = triprojected;
			}
		}
		i++;
	}
	//sort_triangles(triangles_calc, 0, count - 1);
	clipped(count, triangles_calc, game, sdl);
}