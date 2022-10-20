/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_buttons.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 14:35:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 21:11:08 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "doomnukem.h"

t_tool_button	*get_editor_tool_buttons()
{
	static t_tool_button buttons[2] =
	{
		{WINDOW_W - 50, 10, 40, 40, get_point_tool},
		{WINDOW_W - 50, 50, 40, 40, get_entity_tool}
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
			ed->tool_selected = i + 1;
		}
		i++;
	}
}

void	draw_editor_buttons(t_sdlcontext sdl, uint8_t tool_selected)
{
	static t_tool_button	*buttons;
	static t_img			images[2]; //Make this somehow tied to t_tool_button
	int				i;

	if (buttons == NULL) 
	{
		buttons = get_editor_tool_buttons();
		images[0] = get_image_by_name(sdl, "images/linetool.png");
		images[1] = get_image_by_name(sdl, "images/entitytool.png");
	}
	i = 0;
	while (i < 2)
	{
		
		draw_image(sdl.surface->pixels, buttons[i].rect.position, images[i], buttons[i].rect.size);
		if (i + 1 != tool_selected)
			drawrectangle(sdl.surface->pixels, buttons[i].rect, CLR_GRAY);
		i++;
	}
	if (tool_selected != 0)
		drawrectangle(sdl.surface->pixels, buttons[tool_selected - 1].rect, CLR_TURQ);
}