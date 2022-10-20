/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_buttons.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 14:35:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 20:42:26 by okinnune         ###   ########.fr       */
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
	static t_tool_button	*buttons;
	static t_img			images[2]; //Make this somehow tied to t_tool_button
	int				i;

	if (buttons == NULL) 
	{
		buttons = get_editor_tool_buttons();
		images[0] = get_image_by_name(sdl, "images/wall2.png");
		images[1] = get_image_by_name(sdl, "images/wall2.png");
	}
		
	i = 0;
	while (i < 2)
	{
		drawrectangle(sdl.surface->pixels, buttons[i].rect, CLR_GRAY);
		draw_image(sdl.surface->pixels, buttons[i].rect.position, images[i], point_add(buttons[i].rect.size, (t_point){1, 2}));
		i++;
	}
}