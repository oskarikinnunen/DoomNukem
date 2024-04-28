/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_buttons1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:41:55 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:15:47 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

bool	gui_button(char *str, t_autogui *gui)
{
	t_rectangle		rect;
	t_button_return	br;

	br.rect = empty_rect();
	br.clicked = false;
	if (gui_should_draw(gui))
	{
		br = autogui_internal_button(str, gui);
	}
	gui_layout(gui, br.rect);
	return (br.clicked);
}

bool	gui_image_button(t_img	*img, t_autogui *gui)
{
	t_rectangle		imgrect;
	bool			ret;

	imgrect = empty_rect();
	ret = false;
	if (gui_should_draw(gui))
	{
		draw_image(*gui->sdl, gui_current_pos(gui), *img, (t_point){32, 32});
		imgrect.size = (t_point){32, 32};
		imgrect.position = gui_current_pos(gui);
		if (collision_point_rectangle(gui->hid->mouse.pos, imgrect))
		{
			print_text(gui->sdl, img->name, gui->hid->mouse.pos);
			if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
				ret = true;
		}
	}
	gui_layout_big(gui, imgrect);
	return (ret);
}

bool	gui_highlighted_button(char *str, t_autogui *gui)
{
	t_rectangle		rect;
	t_button_return	br;

	br.rect = empty_rect();
	br.clicked = false;
	if (gui_should_draw(gui))
	{
		br = autogui_internal_button(str, gui);
	}
	draw_rectangle(*gui->sdl, br.rect, AMBER_3);
	gui_layout(gui, br.rect);
	return (br.clicked);
}

bool	gui_highlighted_button_if(char *str, t_autogui *gui, bool condition)
{
	if (condition)
		return (gui_highlighted_button(str, gui));
	else
		return (gui_button(str, gui));
}

bool	gui_colored_button(char *str, t_autogui *gui, uint32_t color)
{
	t_rectangle		rect;
	t_button_return	br;

	br.rect = empty_rect();
	br.clicked = false;
	if (gui_should_draw(gui))
	{
		br = autogui_internal_colored_button(str, gui, color);
	}
	gui_layout(gui, br.rect);
	return (br.clicked);
}
