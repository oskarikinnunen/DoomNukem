/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 10:34:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/23 07:01:10 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "entity.h"

/*void	comp_light_ui_update(t_entity *entity, t_world *world)
{
	static	t_img	*icon;
	t_pointlight	*pointlight;

	if (icon == NULL)
		icon = get_image_by_name(*world->sdl, "light_icon");
	pointlight = entity->component.data;
	if (pointlight == NULL)
	{
		render_ball(world->sdl, entity->transform.position, 20.0f, CLR_RED);
		return ;
	}
	draw_entity_icon(entity, icon, world);
	//render_ball(world->sdl, entity->transform.position, pointlight->radius, AMBER_4);
	float dist = vector3_sqr_dist(entity->transform.position, world->player->transform.position);
	if (dist < inter->radius * 100.0f && !entity->occlusion.is_occluded)
		print_text_boxed(world->sdl, "[E] interact", point_div(world->sdl->screensize, 2));
}*/

void	component_empty_function_pointers(t_component	*component)
{
	component->func_allocate = NULL;
	component->func_loadassets = NULL;
	component->func_update = NULL;
	component->func_ui_update = NULL;
	component->func_gui_edit = NULL;
	//render_gizmo2d()
}

void	component_init(t_entity	*entity)
{
	
	/*if (entity->component.type == pft_none)
	{
		entity->component.func_update = NULL;
		entity->component.func_ui_update = NULL;
	}
	if (entity->component.type == pft_audiosource)
	{
		assign_component_audiosource(&entity->component);
	}
	if (entity->component.type == pft_interactable)
	{
		entity->component.func_update = comp_interactable_update;
		entity->component.func_ui_update = comp_interactable_ui_update;
	}
	if (entity->component.type == pft_light)
	{
		entity->component.func_update = NULL;
		entity->component.func_ui_update = comp_light_ui_update;
		t_pointlight *light = entity->component.data;
		light->done = true;
	}*/
}

void	entity_set_component(t_entity *entity, t_componenttype type, t_world *world)
{
	t_componentdefinition	*defs;
	int						i;

	defs = get_component_definitions();
	i = 0;
	while (ft_strlen(defs[i].name) > 0)
	{
		if (defs[i].type == type)
		{
			if (defs[i].func_assign_component == NULL)
			{
				printf("ERROR: component definition doesn't have func_assign_component");
				exit(0);
			}
			defs[i].func_assign_component(&entity->component);
			entity->component.type = type;
			protected_free(entity->component.data);
			entity->component.data = NULL;
			if (entity->component.func_allocate != NULL)
				entity->component.func_allocate(entity, world);
		}
		i++;
	}
}