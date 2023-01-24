/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_raycast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 18:03:40 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/24 09:21:56 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "objects.h"
#include "doomnukem.h"
#include "editor_tools.h"

static t_point v3topoint(t_vector3 vec)
{
	return (t_point) {(int)vec.x, (int)vec.y};
}

t_triangle	triangle_to_screenspace(t_render r, t_triangle tri, t_sdlcontext sdl)
{
	
}

bool	triangle_lookedat(t_render r, t_triangle tri, t_sdlcontext sdl)
{
	t_camera	c;
	t_triangle	clipped[2];
	int			clipamount;
	int			i;
	int			p_i;

	c = r.camera;
	tri.p[0] = quaternion_mul_matrix(c.matview, tri.p[0]);
	tri.p[1] = quaternion_mul_matrix(c.matview, tri.p[1]);
	tri.p[2] = quaternion_mul_matrix(c.matview, tri.p[2]);
	clipamount = clip_triangle_against_plane((t_vector3){.z = 0.1f}, vector3_up(), tri, clipped);
	if (clipamount == 0)
		return (false);
	i = 0;
	while (i < clipamount)
	{
		p_i = 0;
		while (p_i < 3)
		{
			clipped[i].p[p_i] = quaternion_mul_matrix(c.matproj, clipped[i].p[p_i]);
			clipped[i].p[p_i].v = vector3_div(clipped[i].p[p_i].v, clipped[i].p[p_i].w);
			clipped[i].p[p_i].v = vector3_negative(clipped[i].p[p_i].v);
			t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
			clipped[i].p[p_i].v = vector3_add(clipped[i].p[p_i].v, voffsetview);
			clipped[i].p[p_i].v.x *= 0.5f * (float)sdl.window_w;
			clipped[i].p[p_i].v.y *= 0.5f * (float)sdl.window_h;
			p_i++;
		}
		/*vector3_to
		drawcircle(sdl, clipped[i].p[0].v)*/
		if (pointtrianglecollision(point_div(sdl.screensize, 2), clipped[i]))
			return (true);
		i++;
	}
	return (false);
}

bool	entity_lookedat(t_editor *ed, t_sdlcontext sdl, t_entity *entity)
{
	int	i;

	i = 0;
	if (entity->obj == NULL)
		return (false);
	while (i < entity->obj->face_count)
	{
		t_triangle	t;
		t.p[0] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[0] - 1]);
		t.p[1] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[1] - 1]);
		t.p[2] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[2] - 1]);
		t.p[0] = transformed_vector3(entity->transform, t.p[0].v);
		t.p[1] = transformed_vector3(entity->transform, t.p[1].v);
		t.p[2] = transformed_vector3(entity->transform, t.p[2].v);
		if (triangle_lookedat(sdl.render, t, sdl))
			return (true);
		i++;
	}
	return (false);
}

t_vector3	*entity_lookedat_vertex(t_editor *ed, t_sdlcontext sdl, t_entity *entity)
{
	int	i;
	int	j;

	i = 0;
	while (i < entity->obj->face_count)
	{
		t_triangle	t;
		t.p[0] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[0] - 1]); //TODO:
		t.p[1] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[1] - 1]);
		t.p[2] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[2] - 1]);
		t.p[0] = transformed_vector3(entity->transform, t.p[0].v);
		t.p[1] = transformed_vector3(entity->transform, t.p[1].v);
		t.p[2] = transformed_vector3(entity->transform, t.p[2].v);
		if (triangle_lookedat(sdl.render, t, sdl))
			return (&entity->obj->vertices[entity->obj->faces[i].v_indices[0] - 1]);
		i++;
	}
	return (NULL);
}

int32_t	entity_lookedat_triangle_index(t_editor *ed, t_sdlcontext sdl, t_entity *entity)
{
	int	i;
	int	j;

	i = 0;
	while (i < entity->obj->face_count)
	{
		t_triangle	t;
		t.p[0] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[0] - 1]); //TODO:
		t.p[1] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[1] - 1]);
		t.p[2] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[2] - 1]);
		t.p[0] = transformed_vector3(entity->transform, t.p[0].v);
		t.p[1] = transformed_vector3(entity->transform, t.p[1].v);
		t.p[2] = transformed_vector3(entity->transform, t.p[2].v);
		if (triangle_lookedat(sdl.render, t, sdl))
			return (i);
		i++;
	}
	return (-1);
}

bool	object_lookedat(t_editor *ed, t_sdlcontext sdl, t_object *obj)
{
	int	i;
	int	j;

	i = 0;
	while (i < obj->face_count)
	{
		t_triangle	t;
		t.p[0] = vector3_to_quaternion(obj->vertices[obj->faces[i].v_indices[0] - 1]);
		t.p[1] = vector3_to_quaternion(obj->vertices[obj->faces[i].v_indices[1] - 1]);
		t.p[2] = vector3_to_quaternion(obj->vertices[obj->faces[i].v_indices[2] - 1]);
		if (triangle_lookedat(sdl.render, t, sdl))
			return (true);
		i++;
	}
	return (false);
}

//	abc = tri to check against
//	w = distance of ray hit
// disable inverse normal hitting by removing fabs

static bool raycast_tri(t_ray r, t_vector3_tri tri, float *dist)
{
	t_vector3	e1;
	t_vector3	e2;
	t_vector3	n;
	t_vector3	result;
	bool		result_bool;

	e1 = vector3_sub(tri.b, tri.a);
	e2 = vector3_sub(tri.c, tri.a);
	n = vector3_crossproduct(e1, e2);
	float det, invdet;
	det = -vector3_dot(r.dir, n);
	invdet = 1.0f / det;
	t_vector3 ao = vector3_sub(r.origin, tri.a);
	t_vector3 dao = vector3_crossproduct(ao, r.dir);
	result.x = vector3_dot(e2, dao) * invdet;
	result.y = -vector3_dot(e1, dao) * invdet;
	result.z = vector3_dot(ao, n) * invdet;
	result_bool = (*dist > result.z &&
		fabs(det) >= 1e-6 && result.z >= -0.01f && result.x >= 0.0f
		&&	result.y >= 0.0f && (result.x + result.y) <= 1.0f);
	if (result_bool)
	{
		*dist = result.z;
	}
	return(result_bool);
}

t_vector3_tri	worldspace_tri(t_entity *entity, int index)
{
	t_vector3_tri	tri;
	t_object		*object;

	object = entity->obj;
	
	tri.a = object->vertices[object->faces[index].v_indices[0] - 1];
	tri.b = object->vertices[object->faces[index].v_indices[1] - 1];
	tri.c = object->vertices[object->faces[index].v_indices[2] - 1];
	tri.a = transformed_vector3(entity->transform, tri.a).v;
	tri.b = transformed_vector3(entity->transform, tri.b).v;
	tri.c = transformed_vector3(entity->transform, tri.c).v;
	return (tri);
}

bool raycast_entity(t_ray r, t_raycastinfo *info, t_entity *entity)
{
	int			i;
	bool		hit;
	
	i = 0;
	hit = false;
	if (entity->ignore_raycasts)
		return (hit);
	while (i < entity->obj->face_count)
	{
		t_vector3_tri	tri;

		tri = worldspace_tri(entity, i);
		if (raycast_tri(r, tri, &info->distance))
		{
			info->hit_pos = vector3_add(r.origin, vector3_mul(r.dir, info->distance));
			info->hit_entity = entity;
			hit = true;
		}
		i++;
	}
	return (hit);
}

bool	vector_is_in_triangle(t_vector3 vec, t_vector3_tri tri)
{
	t_ray	ray;
	t_raycastinfo	info;

	ft_bzero(&info, sizeof(info));
	ray.origin = vec;
	ray.dir = (t_vector3){.z = -1.0f};
	if (raycast_tri(ray, tri, &info.distance))
		return (true);
	return (false);
}

bool raycast_plane(t_ray r, t_raycastinfo *info, float plane_z)
{
	t_vector3_tri	tri;
	t_raycastinfo	internal_info;
	bool			hit;

	ft_bzero(&internal_info, sizeof(t_raycastinfo));
	internal_info.distance = 100000.0f; //TODO: inf?
	r.dir = vector3_normalise(r.dir);
	tri.a = (t_vector3){.z = plane_z};
	tri.b = (t_vector3){50000.0f, 0.0f, plane_z};
	tri.c = (t_vector3){0.0f, 50000.0f, plane_z};
	if (raycast_tri(r, tri, &internal_info.distance))
	{
		internal_info.hit_pos =
			vector3_add(r.origin, vector3_mul(r.dir, internal_info.distance));
		hit = true;
	}
	//tri.c = (t_vector3){10000.0f, 0.0f, plane_z}; //Do the other triangle
	if (info != NULL)
		*info = internal_info;
	return (hit);
	//raycas
	//raycast_tri(r, )
}

bool raycast_new(t_ray r, t_raycastinfo *info, t_world *world)
{
	int				i;
	int				found;
	t_entitycache	*cache;
	t_raycastinfo	internal_info;
	t_entity		*ent;
	bool			hit;

	hit = false;
	cache = &world->entitycache;
	ft_bzero(&internal_info, sizeof(t_raycastinfo));
	internal_info.distance = 100000.0f; //TODO: inf?
	r.dir = vector3_normalise(r.dir);
	i = 0;
	found = 0;
	while (found < cache->existing_entitycount
		&& i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status != es_free && !ent->hidden)
		{
			if (raycast_entity(r, &internal_info, ent))
				hit = true;
			found++;
		}
		i++;
	}
	/*if (!hit)
		hit = raycast_plane(r, &internal_info, 0.0f);*/
	if (info != NULL)
		*info = internal_info;
	return (hit);
}


t_vector3	raycast_DEPRECATED(t_editor *ed) //TODO: deprecate
{
	t_vector3	result;
	t_vector3	rayforward;
	int			iter;
	float		dist;

	result = ed->player.transform.position;
	iter = 0;
	dist = sqrt(powf(result.z, 2.0f) + powf(sin(ft_clampf(ed->player.transform.rotation.y, -RAD90, -0.1f)), 2.0f)); //not correct lol
	rayforward = lookdirection((t_vector2){ed->player.transform.rotation.x, ed->player.transform.rotation.y});
	rayforward.z = ft_clampf(rayforward.z, -1.0f, -0.001f);
	while (result.z > 0.0f)
	{
		result = vector3_movetowards(result, rayforward, dist);
		iter++;
	}
	result.x = ft_clampf(result.x, 0.0f, 3000.0f);
	result.y = ft_clampf(result.y, 0.0f, 3000.0f);
	result.z = 0.0f;
	return (result);
}
