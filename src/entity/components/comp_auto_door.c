/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_auto_door.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfum <kfum@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/03 12:26:08 by kfum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_auto_door_update(t_entity *entity, t_world *world)
{
	t_auto_door	*auto_door;

	auto_door = entity->component.data;
	if (vector3_dist(world->player->transform.position, entity->transform.position) < 100.0f)
	{
		entity->transform.position.z = ft_fmovetowards(entity->transform.position.z, 100.0f, 0.02f * world->clock.delta);
	}
	else
	{
		entity->transform.position.z = ft_fmovetowards(entity->transform.position.z, 0.0f, 0.02f * world->clock.delta);
	}
	if (auto_door == NULL)
		return ;
}

/* Called once per frame after the 3D world has been drawn, use this to draw gizmos/rays/whatever*/
void	comp_auto_door_ui_update(t_entity *entity, t_world *world)
{
	t_auto_door	*auto_door;

	auto_door = entity->component.data;
	if (auto_door == NULL)
		return ;
}

/* Used to edit component values */
void	comp_auto_door_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_auto_door	*auto_door;
	//entity->particleimages[particleanim.];
	auto_door = entity->component.data;
	gui_label("GUI for auto_door not implemented", gui);
	if (auto_door == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_auto_door_loadassets(t_entity *entity, t_world *world)
{
	t_auto_door	*auto_door;
	auto_door = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_auto_door_allocate(t_entity *entity, t_world *world)
{
	t_auto_door	*auto_door;

	entity->component.data = ft_memalloc(sizeof(t_auto_door));
	entity->component.data_size = sizeof(t_auto_door);
	auto_door = (t_auto_door *)entity->component.data;
	
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_auto_door(t_component *component)
{
	component->data_size = sizeof(t_auto_door);
	component->func_allocate = comp_auto_door_allocate;
	component->func_update = comp_auto_door_update;
	component->func_gui_edit = comp_auto_door_gui_edit;
	component->func_ui_update = comp_auto_door_ui_update;
	component->func_loadassets = comp_auto_door_loadassets;
}