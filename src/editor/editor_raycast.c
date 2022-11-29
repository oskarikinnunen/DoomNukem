/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_raycast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 18:03:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/21 16:02:46 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "objects.h"
#include "doomnukem.h"

static t_point v3topoint(t_vector3 vec)
{
	return (t_point) {(int)vec.x, (int)vec.y};
}

static bool	tri_lookedat(t_render r, t_triangle tri, t_sdlcontext sdl)
{
	t_triangle	clipped[2];
	int			clipamount;
	t_triangle	final[3];
	int			i;
	int			p_i;

	tri.p[0] = quaternion_mul_matrix(r.matworld, tri.p[0]);
	tri.p[1] = quaternion_mul_matrix(r.matworld, tri.p[1]);
	tri.p[2] = quaternion_mul_matrix(r.matworld, tri.p[2]);
	tri.p[0] = quaternion_mul_matrix(r.matview, tri.p[0]);
	tri.p[1] = quaternion_mul_matrix(r.matview, tri.p[1]);
	tri.p[2] = quaternion_mul_matrix(r.matview, tri.p[2]);
	clipamount = triangle_clipagainstplane((t_vector3){0.0f, 0.0f, 0.1f}, vector3_up(), &tri, clipped);
	if (clipamount == 0)
		return (false);
	i = 0;
	while (i < clipamount)
	{
		p_i = 0;
		while (p_i < 3)
		{
			clipped[i].p[p_i] = quaternion_mul_matrix(r.matproj, clipped[i].p[p_i]);
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
	int	j;

	i = 0;
	while (i < entity->obj->face_count)
	{
		t_triangle	t;
		t.p[0] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[0] - 1]);
		t.p[1] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[1] - 1]);
		t.p[2] = vector3_to_quaternion(entity->obj->vertices[entity->obj->faces[i].v_indices[2] - 1]);
		t.p[0] = transformed_vector3(entity->transform, t.p[0].v);
		t.p[1] = transformed_vector3(entity->transform, t.p[1].v);
		t.p[2] = transformed_vector3(entity->transform, t.p[2].v);
		if (tri_lookedat(ed->render, t, sdl))
			return (true);
		i++;
	}
	return (false);
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
		if (tri_lookedat(ed->render, t, sdl))
			return (true);
		i++;
	}
	return (false);
}

t_vector3	raycast(t_editor *ed)
{
	t_vector3	result;
	t_vector3	rayforward;
	int			iter;
	float		dist;

	result = ed->position;
	iter = 0;
	dist = sqrt(powf(ed->position.z, 2.0f) + powf(sin(ft_clampf(ed->angle.y, -RAD90, -0.1f)), 2.0f)); //not correct lol
	rayforward = ed->forward;
	rayforward.z = ft_clampf(rayforward.z, -1.0f, -0.001f);
	while (result.z > 0.0f)
	{
		result = vector3_movetowards(result, rayforward, dist);
		iter++;
	}
	result.x = ft_clampf(result.x, 0.0f, 1000.0f);
	result.y = ft_clampf(result.y, 0.0f, 1000.0f);
	result.z = 0.0f;
	return (result);
}
