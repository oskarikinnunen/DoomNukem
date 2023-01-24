/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component_definitions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 20:11:13 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/24 06:45:45 by vlaine           ###   ########.fr       */
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
		(t_componentdefinition){.type = COMP_MECHASUIT, .name = "Mechasuit",
			.func_assign_component = assign_component_mechasuit},
		(t_componentdefinition){.type = COMP_NPC, .name = "NPC",
			.func_assign_component = assign_component_npc}
	};
	return (editables);
}