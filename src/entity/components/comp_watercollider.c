/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_watercollider.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/24 11:03:09 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_watercollider_update(t_entity *entity, t_world *world)
{
	t_watercollider	*watercollider;

	watercollider = entity->component.data;
	if (watercollider == NULL)
		return ;
}

/* Called once per frame after the 3D world has been drawn, use this to draw gizmos/rays/whatever*/
void	comp_watercollider_ui_update(t_entity *entity, t_world *world)
{
	t_watercollider	*watercollider;

	watercollider = entity->component.data;
	if (watercollider == NULL)
		return ;
}

/* Used to edit component values */
void	comp_watercollider_gui_edit(t_component *component, t_autogui *gui)
{
	t_watercollider	*watercollider;
	//entity->particleimages[particleanim.];
	watercollider = component->data;
	gui_label("GUI for watercollider not implemented", gui);
	if (watercollider == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_watercollider_loadassets(t_entity *entity, t_world *world)
{
	t_watercollider	*watercollider;
	watercollider = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_watercollider_allocate(t_entity *entity, t_world *world)
{
	t_watercollider	*watercollider;

	entity->component.data = ft_memalloc(sizeof(t_watercollider));
	entity->component.data_size = sizeof(t_watercollider);
	watercollider = (t_watercollider *)entity->component.data;
	
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_watercollider(t_component *component)
{
	component->data_size = sizeof(t_watercollider);
	component->func_allocate = comp_watercollider_allocate;
	component->func_update = comp_watercollider_update;
	component->func_gui_edit = comp_watercollider_gui_edit;
	component->func_ui_update = comp_watercollider_ui_update;
	component->func_loadassets = comp_watercollider_loadassets;
}