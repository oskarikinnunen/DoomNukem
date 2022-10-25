/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/25 13:38:15 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
static void entity_tool_click(t_editor *ed)
{
	printf("entity tool click functionality is not implemented yet.\n");
	ed->mouse.click_unhandled = false;
}

static void	entity_tool_draw(t_editor *ed, t_sdlcontext sdl) //This needs to access editors state, so pass editor here??
{
	return ;
}

static void	entity_tool_update(t_editor *ed) //This needs to access editors state, so pass editor here??
{
	if (ed->mouse.click_unhandled)
		entity_tool_click(ed);
}

t_tool	*get_entity_tool()
{
	static t_tool	tool
	= {
		entity_tool_update, entity_tool_draw
	};

	return (&tool);
}
