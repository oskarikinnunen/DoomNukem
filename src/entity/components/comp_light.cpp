/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_light.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 20:57:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void	comp_light_allocate(t_entity *entity, t_world *world)
{
	t_mat4x4	matproj;
	t_light		*light;
	int			i;

	entity->component.data = prot_memalloc(sizeof(t_light));
	entity->component.data_size = sizeof(t_light);
	light = (t_light *)entity->component.data;
	light->radius = 300.0f;
	light->cubemap.cameras[0].look_dir = (t_vector3){.x = 1.0f};
	light->cubemap.cameras[1].look_dir = (t_vector3){.y = 1.0f};
	light->cubemap.cameras[2].look_dir = (t_vector3){.x = 0.01f, .z = 0.99f};
	light->cubemap.cameras[3].look_dir = (t_vector3){.x = -1.0f};
	light->cubemap.cameras[4].look_dir = (t_vector3){.y = -1.0f};
	light->cubemap.cameras[5].look_dir = (t_vector3){.x = -0.01f, .z = -0.99f};
	light->cubemap.resolution.x = 2000;
	light->cubemap.resolution.y = 2000;
	light->ignore_self = false;
	light->clr = AMBER_3;
	light->intensity = 1.0f;
	light->origin = (t_vector3){0.0f, -25.0f, -10.0f};
}

void	assign_component_light(t_component *component)
{
	component->data_size = sizeof(t_light);
	component->func_allocate = comp_light_allocate;
}
