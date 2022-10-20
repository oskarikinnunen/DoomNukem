/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_buttons.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 14:35:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 14:00:49 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "doomnukem.h"

t_tool_button	*get_editor_tool_buttons()
{
	static t_tool_button buttons[2] =
	{
		{WINDOW_W - 50, 10, 40, 40, get_point_tool},
		{WINDOW_W - 50, 60, 40, 40, get_entity_tool}
	};

	return	(buttons);
}

//Maybe this should take gamecontext/whatever editor context and change the tool for the context
void	check_tool_change_click(t_point cursor, t_editor *ed)
{
	t_tool_button	*buttons;
	int				i;

	buttons = get_editor_tool_buttons();
	i = 0;
	while (i < 2)
	{
		if (pointrectanglecollision(cursor, buttons[i].rect))
		{
			//buttons[i].tool_get()->change_func(ed);
			ed->tool = buttons[i].tool_get();
			ed->mouse.click_unhandled = false;
		}
		i++;
	}
}

void	draw_editor_buttons(t_sdlcontext sdl)
{
	t_tool_button	*buttons;
	int				i;

	buttons = get_editor_tool_buttons();
	i = 0;
	while (i < 2)
	{
		drawrectangle(sdl.surface->pixels, buttons[i].rect, CLR_GRAY);
		i++;
	}
}