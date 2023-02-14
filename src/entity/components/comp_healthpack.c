/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_healthpack.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/24 11:11:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_healthpack_update(t_entity *entity, t_world *world)
{
	t_healthpack	*healthpack;

	healthpack = entity->component.data;
	if (healthpack == NULL)
		return ;
}

/* Called once per frame after the 3D world has been drawn, use this to draw gizmos/rays/whatever*/
void	comp_healthpack_ui_update(t_entity *entity, t_world *world)
{
	t_healthpack	*healthpack;

	healthpack = entity->component.data;
	if (healthpack == NULL)
		return ;
}

/* Used to edit component values */
void	comp_healthpack_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_healthpack	*healthpack;
	//entity->particleimages[particleanim.];
	healthpack = entity->component.data;
	gui_label("GUI for healthpack not implemented", gui);
	if (healthpack == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_healthpack_loadassets(t_entity *entity, t_world *world)
{
	t_healthpack	*healthpack;
	healthpack = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_healthpack_allocate(t_entity *entity, t_world *world)
{
	t_healthpack	*healthpack;

	entity->component.data = ft_memalloc(sizeof(t_healthpack));
	entity->component.data_size = sizeof(t_healthpack);
	healthpack = (t_healthpack *)entity->component.data;
	
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_healthpack(t_component *component)
{
	component->data_size = sizeof(t_healthpack);
	component->func_allocate = comp_healthpack_allocate;
	component->func_update = comp_healthpack_update;
	component->func_gui_edit = comp_healthpack_gui_edit;
	component->func_ui_update = comp_healthpack_ui_update;
	component->func_loadassets = comp_healthpack_loadassets;
}