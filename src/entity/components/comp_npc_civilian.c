/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_placeholder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/24 11:11:18 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_npc_civilian_update(t_entity *entity, t_world *world)
{
	t_npc_civilian	*npc_civilian;

	npc_civilian = entity->component.data;
	if (npc_civilian == NULL)
		return ;
}

/* Called once per frame after the 3D world has been drawn, use this to draw gizmos/rays/whatever*/
void	comp_npc_civilian_ui_update(t_entity *entity, t_world *world)
{
	t_npc_civilian	*npc_civilian;

	npc_civilian = entity->component.data;
	if (npc_civilian == NULL)
		return ;
}

/* Used to edit component values */
void	comp_npc_civilian_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_npc_civilian	*npc_civilian;
	//entity->particleimages[particleanim.];
	npc_civilian = entity->component.data;
	gui_label("GUI for npc_civilian not implemented", gui);
	if (npc_civilian == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_npc_civilian_loadassets(t_entity *entity, t_world *world)
{
	t_npc_civilian	*npc_civilian;
	npc_civilian = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_npc_civilian_allocate(t_entity *entity, t_world *world)
{
	t_npc_civilian	*npc_civilian;

	entity->component.data = ft_memalloc(sizeof(t_npc_civilian));
	entity->component.data_size = sizeof(t_npc_civilian);
	npc_civilian = (t_npc_civilian *)entity->component.data;
	
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_npc_civilian(t_component *component)
{
	component->data_size = sizeof(t_npc_civilian);
	component->func_allocate = comp_npc_civilian_allocate;
	component->func_update = comp_npc_civilian_update;
	component->func_gui_edit = comp_npc_civilian_gui_edit;
	component->func_ui_update = comp_npc_civilian_ui_update;
	component->func_loadassets = comp_npc_civilian_loadassets;
}