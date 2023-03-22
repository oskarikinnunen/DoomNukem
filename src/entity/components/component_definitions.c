/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component_definitions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 20:11:13 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/13 17:19:44 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "components.h"
#include "entity.h"

void	assign_component_none(t_component *component)
{
	component->data = NULL;
	component_empty_function_pointers(component);
}

t_componentdefinition	*get_component_definitions()
{
	static	t_componentdefinition editables[32] =
	{
		(t_componentdefinition){.type = COMP_NONE, .name = "None",
			.func_assign_component = assign_component_none},
		(t_componentdefinition){.type = COMP_AUDIOSOURCE, .name = "Audiosource",
			.func_assign_component = assign_component_audiosource},
		(t_componentdefinition){.type = COMP_INTERACTABLE, .name = "Interactable",
			.func_assign_component = assign_component_interactable},
		(t_componentdefinition){.type = COMP_LIGHT, .name = "Light",
			.func_assign_component = assign_component_light},
		(t_componentdefinition){.type = COMP_HEALTHPACK, .name = "Healthpack",
			.func_assign_component = assign_component_healthpack},
		(t_componentdefinition){.type = COMP_NPC, .name = "NPC",
			.func_assign_component = assign_component_npc},
		(t_componentdefinition){.type = COMP_PLAYERSPAWN, .name = "Playerspawn",
			.func_assign_component = assign_component_playerspawn},
		(t_componentdefinition){.type = COMP_GUN_PICKUP, .name = "Gun_pickup",
			.func_assign_component = assign_component_gun_pickup},
		(t_componentdefinition){.type = COMP_AUTO_DOOR, .name = "Auto_door",
			.func_assign_component = assign_component_auto_door}
	};
	return (editables);
}
