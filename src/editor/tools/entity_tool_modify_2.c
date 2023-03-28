/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool_modify_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:04:00 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 09:16:49 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tools/entitytool.h" 
#include "objects.h"

void	gui_component(t_entity *entity, t_autogui *gui, t_world *world)
{
	gui_start(gui);
	if (entity->component.func_gui_edit != NULL)
		entity->component.func_gui_edit(entity, gui, world);
	else
		gui_label("!Component doesn't have gui function", gui);
	gui_end(gui);
}

void	gui_entity_mode(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_component_definition	*defs;
	int						i;

	gui->offset.x = 20;
	defs = get_component_definitions();
	i = 0;
	while (ft_strlen(defs[i].name) > 0)
	{
		if (gui_highlighted_button_if(defs[i].name, gui,
				entity->component.type == defs[i].type))
		{
			entity->component.type = defs[i].type;
			entity_set_component_functions(entity, world);
		}
		i++;
	}
	gui->offset.x = 0;
}
