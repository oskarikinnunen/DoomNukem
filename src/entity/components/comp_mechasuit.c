/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_mechasuit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/19 12:27:44 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_mechasuit_update(t_entity *entity, t_world *world)
{
	t_mechasuit	*mechasuit;

	mechasuit = entity->component.data;
	if (mechasuit == NULL)
		return ;
}

/* Called once per frame after the 3D world has been drawn, use this to draw gizmos/rays/whatever*/
void	comp_mechasuit_ui_update(t_entity *entity, t_world *world)
{
	t_mechasuit	*mechasuit;

	mechasuit = entity->component.data;
	if (mechasuit == NULL)
		return ;
}

/* Used to edit component values */
void	comp_mechasuit_gui_edit(t_component *component, t_autogui *gui)
{
	t_mechasuit	*mechasuit;
	//entity->particleimages[particleanim.];
	mechasuit = component->data;
	gui_label("GUI for mechasuit not implemented", gui);
	if (mechasuit == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_mechasuit_loadassets(t_entity *entity, t_world *world)
{
	t_mechasuit	*mechasuit;
	mechasuit = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_mechasuit_allocate(t_entity *entity, t_world *world)
{
	t_mechasuit	*mechasuit;

	entity->component.data = ft_memalloc(sizeof(t_mechasuit));
	entity->component.data_size = sizeof(t_mechasuit);
	mechasuit = (t_mechasuit *)entity->component.data;
	
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_mechasuit(t_component *component)
{
	component->data_size = sizeof(t_mechasuit);
	component->func_allocate = comp_mechasuit_allocate;
	component->func_update = comp_mechasuit_update;
	component->func_gui_edit = comp_mechasuit_gui_edit;
	component->func_ui_update = comp_mechasuit_ui_update;
	component->func_loadassets = comp_mechasuit_loadassets;
}