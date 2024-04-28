/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_player_spawn1.c                               :+:      :+:    :+:   */
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
void	comp_playerspawn_update(t_entity *entity, t_world *world)
{
	t_playerspawn	*playerspawn;

	playerspawn = entity->component.data;
	if (playerspawn == NULL)
		return ;
}

// Called once per frame after the 3D world has been drawn
// Use this to draw gizmos/rays/whatever
void	comp_playerspawn_ui_update(t_entity *entity, t_world *world)
{
	t_playerspawn	*playerspawn;

	playerspawn = entity->component.data;
	if (playerspawn == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_playerspawn_load_assets(t_entity *entity, t_world *world)
{
	t_playerspawn	*playerspawn;

	playerspawn = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_playerspawn_allocate(t_entity *entity, t_world *world)
{
	t_playerspawn	*playerspawn;

	entity->component.data = prot_memalloc(sizeof(t_playerspawn));
	entity->component.data_size = sizeof(t_playerspawn);
	playerspawn = (t_playerspawn *)entity->component.data;
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_player_spawn(t_component *component)
{
	component->data_size = 0;
	component_empty_function_pointers(component);
}
