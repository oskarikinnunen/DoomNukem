/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_raycast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 18:03:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/21 17:23:16 by vlaine           ###   ########.fr       */
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
	tri.p[0] = quaternion_mul_matrix(c.matworld, tri.p[0]);
	tri.p[1] = quaternion_mul_matrix(c.matworld, tri.p[1]);
	tri.p[2] = quaternion_mul_matrix(c.matworld, tri.p[2]);
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
		if (triangle_lookedat(ed->render, t, sdl))
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
		if (triangle_lookedat(ed->render, t, sdl))
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
		if (triangle_lookedat(ed->render, t, sdl))
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
		if (triangle_lookedat(ed->render, t, sdl))
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
	result.x = ft_clampf(result.x, 0.0f, 2500.0f);
	result.y = ft_clampf(result.y, 0.0f, 2500.0f);
	result.z = 0.0f;
	return (result);
}
