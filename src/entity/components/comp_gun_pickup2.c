/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_gun_pickup2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 18:01:54 by raho              #+#    #+#             */
/*   Updated: 2023/03/28 10:26:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

// Called once per frame after the 3D world has been drawn
// Use this to draw gizmos/rays/whatever
void	comp_gun_pickup_ui_update(t_entity *entity, t_world *world)
{
	t_gun_pickup	*gun_pickup;

	gun_pickup = entity->component.data;
	if (gun_pickup == NULL)
		return ;
}

/* Used to edit component values */
void	comp_gun_pickup_gui_edit(t_entity *entity,
									t_autogui *gui, t_world *world)
{
	t_gun_pickup	*gun_pickup;

	gun_pickup = entity->component.data;
	gui_label("GUI for gun_pickup not implemented", gui);
	if (gun_pickup == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_gun_pickup_load_assets(t_entity *entity, t_world *world)
{
	t_gun_pickup	*gun_pickup;

	gun_pickup = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_gun_pickup_allocate(t_entity *entity, t_world *world)
{
	t_gun_pickup	*gun_pickup;

	entity->component.data = prot_memalloc(sizeof(t_gun_pickup));
	entity->component.data_size = sizeof(t_gun_pickup);
	gun_pickup = (t_gun_pickup *)entity->component.data;
	gun_pickup->original_z = entity->transform.position.z;
	if (world->app_mode == APPMODE_PLAY)
		entity->ignore_raycasts = true;
	entity->dynamic_lit = true;
}
