/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_auto_door.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:45:43 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_auto_door_update(t_entity *entity, t_world *world)
{
	t_auto_door	*auto_door;
	float		dist;
	t_vector3	og_pos;

	auto_door = entity->component.data;
	if (auto_door == NULL)
		return ;
	og_pos = entity->transform.position;
	og_pos.z = auto_door->original_z;
	dist = vector3_sqr_dist(og_pos, world->player->transform.position);
	if (dist < 5000.0f)
		entity->transform.position.z = ft_fmovetowards(\
				entity->transform.position.z, auto_door->original_z + 100.0f, \
				0.05f * world->clock.delta);
	else
		entity->transform.position.z = ft_fmovetowards(\
				entity->transform.position.z, auto_door->original_z, \
				0.05f * world->clock.delta);
}

/* Used to edit component values */
void	comp_auto_door_gui_edit(t_entity *entity,
								t_autogui *gui, t_world *world)
{
	t_auto_door	*auto_door;

	auto_door = entity->component.data;
	gui_label("GUI for auto_door not implemented", gui);
	if (auto_door == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_auto_door_load_assets(t_entity *entity, t_world *world)
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

	entity->component.data = prot_memalloc(sizeof(t_auto_door));
	entity->component.data_size = sizeof(t_auto_door);
	auto_door = (t_auto_door *)entity->component.data;
	auto_door->original_z = entity->transform.position.z;
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_auto_door(t_component *component)
{
	component->data_size = sizeof(t_auto_door);
	component_empty_function_pointers(component);
	component->func_allocate = comp_auto_door_allocate;
	component->func_update = comp_auto_door_update;
	component->func_gui_edit = comp_auto_door_gui_edit;
	component->func_load_assets = comp_auto_door_load_assets;
}
