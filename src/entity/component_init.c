/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 10:34:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 09:17:22 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "entity.h"

void	component_empty_function_pointers(t_component *component)
{
	component->func_allocate = NULL;
	component->func_load_assets = NULL;
	component->func_update = NULL;
	component->func_ui_update = NULL;
	component->func_gui_edit = NULL;
}

void	entity_set_component_functions(t_entity *entity, t_world *world)
{
	t_component_definition	*defs;
	int						i;

	defs = get_component_definitions();
	i = 0;
	while (ft_strlen(defs[i].name) > 0)
	{
		if (defs[i].type == entity->component.type)
		{
			if (defs[i].func_assign_component == NULL)
			{
				doomlog(LOG_FATAL, \
"ERROR: component definition doesn't have func_assign_component");
			}
			defs[i].func_assign_component(&entity->component);
			protected_free(entity->component.data);
			entity->component.data = NULL;
			if (entity->component.func_allocate != NULL)
				entity->component.func_allocate(entity, world);
		}
		i++;
	}
}
