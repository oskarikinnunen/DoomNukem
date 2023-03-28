/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decal.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 17:39:00 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/28 09:16:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

#define DECAL_FOV 0.546302f

static void	set_vertex_positions(
	t_vector3 vertices[4], t_vector3 pos, t_vector3 y, t_vector3 x)
{
	vertices[0] = vector3_sub(vector3_sub(pos, y), x);
	vertices[1] = vector3_sub(vector3_add(pos, y), x);
	vertices[2] = vector3_add(vector3_sub(pos, y), x);
	vertices[3] = vector3_add(vector3_add(pos, y), x);
}

static void	decal_entity_thingies(t_entity *entity)
{
	entity->life_timeable = true;
	entity->life_time = 5000;
	entity->ignore_raycasts = true;
	entity->destroy_obj = true;
}

void	decal(struct s_world *world, t_decal decal)
{
	t_object	*obj;
	t_vector3	new_up;
	t_vector3	new_right;
	float		nearside;
	t_entity	*entity;

	obj = object_plane(world->sdl);
	obj->materials->img = decal.img;
	entity = spawn_entity(world, obj);
	decal_entity_thingies(entity);
	ft_strncpy(entity->object_name.str, "decal", 6);
	if (vector3_dot(decal.normal, world->sdl->render.camera.look_dir) >= 0.0f)
		decal.normal = vector3_negative(decal.normal);
	decal.position = vector3_add(decal.position, decal.normal);
	decal.normal = vector3_negative(decal.normal);
	new_up = vector3_sub((t_vector3){0.01f, 0.0f, 0.99f}, \
	vector3_mul(decal.normal, vector3_dot((t_vector3){0.01f, 0.0f, 0.99f}, \
	decal.normal)));
	new_up = vector3_normalise(new_up);
	new_right = vector3_cross_product(new_up, decal.normal);
	nearside = DECAL_FOV * decal.size;
	set_vertex_positions(obj->vertices, decal.position, \
		vector3_mul(new_up, nearside), vector3_mul(new_right, nearside));
}
