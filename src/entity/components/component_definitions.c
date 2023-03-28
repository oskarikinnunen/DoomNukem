/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component_definitions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 20:11:13 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 09:17:37 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "components.h"
#include "entity.h"

void	assign_component_none(t_component *component)
{
	component_empty_function_pointers(component);
}

t_component_definition	*get_component_definitions(void)
{
	static t_component_definition	editables[32] = {\
	(t_component_definition){.type = COMP_NONE, .name = "None", \
		.func_assign_component = assign_component_none}, \
	(t_component_definition){.type = COMP_AUDIOSOURCE, .name = "Audiosource", \
		.func_assign_component = assign_component_audiosource}, \
	(t_component_definition){.type = COMP_INTERACTABLE, .name = "Interactable", \
		.func_assign_component = assign_component_interactable}, \
	(t_component_definition){.type = COMP_LIGHT, .name = "Light", \
		.func_assign_component = assign_component_light}, \
	(t_component_definition){.type = COMP_HEALTHPACK, .name = "Supplypack", \
		.func_assign_component = assign_component_healthpack}, \
	(t_component_definition){.type = COMP_NPC, .name = "NPC", \
		.func_assign_component = assign_component_npc}, \
	(t_component_definition){.type = COMP_PLAYERSPAWN, .name = "Playerspawn", \
		.func_assign_component = assign_component_player_spawn}, \
	(t_component_definition){.type = COMP_GUN_PICKUP, .name = "Gun_pickup", \
		.func_assign_component = assign_component_gun_pickup}, \
	(t_component_definition){.type = COMP_AUTO_DOOR, .name = "Auto_door", \
		.func_assign_component = assign_component_auto_door}, \
	(t_component_definition){.type = COMP_STORYEVENT, .name = "Storyevent", \
		.func_assign_component = assign_component_story_event}, \
	(t_component_definition){.type = COMP_BOUTTON, .name = "Boutton", \
		.func_assign_component = assign_component_boutton}};

	return (editables);
}
