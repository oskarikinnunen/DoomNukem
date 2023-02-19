#include "doomnukem.h"


static void    clip_and_render_triangle(t_lighting *lighting, t_point_triangle triangle)
{
	t_point_triangle	clipped[2];
	t_point_triangle	triangles[32]; // could be 10 prob
	int					start;
	int					end;

	start = 0;
	end = 1;
	triangles[0] = triangle;
	int nnewtriangles = 1;
	for (int p = 0; p < 4; p++)
	{
		int ntristoadd = 0;
		while (nnewtriangles > 0)
		{
			t_point_triangle test;
			test = triangles[start++];
			nnewtriangles--;
			switch (p)
			{
			case 0: ntristoadd = point_clip_triangle_against_plane(lighting->screen_edge.min, (t_vector2){0.0f, 1.0f}, test, clipped); break;
			case 1: ntristoadd = point_clip_triangle_against_plane(lighting->screen_edge.max, (t_vector2){0.0f, -1.0f}, test, clipped); break;
			case 2: ntristoadd = point_clip_triangle_against_plane(lighting->screen_edge.min, (t_vector2){1.0f, 0.0f}, test, clipped); break;
			case 3: ntristoadd = point_clip_triangle_against_plane(lighting->screen_edge.max, (t_vector2){-1.0f, 0.0f}, test, clipped); break;
			}
			for (int w = 0; w < ntristoadd; w++)
			{
				triangles[end++] = clipped[w];
			}
		}
		nnewtriangles = end - start;
	}
	while (start < end)
	{
		rasterize_zbuffer(lighting, triangles[start]);
		start++;
	}
}

t_point_triangle triangle_to_screen_point_triangle(t_mat4x4 matproj, t_triangle clipped)
{
	t_triangle			triprojected;
	t_point_triangle	tri;
	int					i;
	t_vector3			voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};

	i = 0;
	while (i < 3)
	{
		triprojected.p[i] = quaternion_mul_matrix(matproj, clipped.p[i]);
		triprojected.t[i] = clipped.t[i];
		tri.t[i].w = 1.0f / triprojected.p[i].w;
		triprojected.p[i].v = vector3_div(triprojected.p[i].v, triprojected.p[i].w);
		triprojected.p[i].v = vector3_negative(triprojected.p[i].v);
		triprojected.p[i].v = vector3_add(triprojected.p[i].v, voffsetview);
		tri.p[i].x = triprojected.p[i].v.x * 640;
		tri.p[i].y = triprojected.p[i].v.y * 640;
		i++;
	}
	tri.clr = clipped.clr;
	return(tri);
}

void render_zbuffer(t_lighting *lighting, t_entity *entity)
{
	t_triangle		clipped[2];
	t_triangle		world_triangle;
	int				nclippedtriangles;
	int				index;
	int				i;

	if (entity->world_triangles == NULL)
	{
		//doomlog(LOG_WARNING, "entity->worldtriangles == null in render_quaternions\n!	printing entity->object_name.str and entity->obj->name");
		//doomlog(LOG_WARNING, entity->object_name.str);
		//doomlog(LOG_WARNING, entity->obj->name);
		return;
	}
	if (point_cmp(entity->occlusion.clip.max, point_zero()) && point_cmp(entity->occlusion.clip.min, point_zero()))
	{
		lighting->screen_edge.max = (t_vector2){1279.0f, 1279.0f};
		lighting->screen_edge.min = vector2_zero();
	}
	else
	{
		lighting->screen_edge.min = point_to_vector2(entity->occlusion.clip.min);
		lighting->screen_edge.max = point_to_vector2(entity->occlusion.clip.max);
	}
	lighting->screen_edge.max = (t_vector2){1279.0f, 1279.0f};
	lighting->screen_edge.min = vector2_zero();
	i = 0;
	while (i < entity->obj->face_count)
	{
		world_triangle = triangle_to_viewspace(entity->world_triangles[i], lighting->camera.matview);
		nclippedtriangles = clip_triangle_against_plane((t_vector3){.z = 0.1f}, (t_vector3){.z = 1.0f}, world_triangle, clipped);
		index = 0;
		while (index < nclippedtriangles) // check backface
		{
			clip_and_render_triangle(lighting, triangle_to_screen_point_triangle(lighting->camera.matproj, clipped[index]));
			index++;
		}
		i++;
	}
}